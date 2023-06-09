#include <string>
#include <fstream>
#include "../symbols/token.hpp"
#include "../symbols/token-type.hpp"
#include "code-gen.hpp"
#include <memory>
#include <stdexcept>
#include <cstdio>

CodeGen::CodeGen(std::string asmFileName)
{
    this->mem_count = 0x10000;
    this->tmp_count = 0x0;
    this->labl_count = 1;
    this->programFilep = pfp;
    this->programFile.open(pfp);
}
CodeGen::~CodeGen()
{
    this->programFile.close();
}

template <typename... Args>
std::string format(const std::string &format, Args... args)
{
    // Determina o tamanho do buffer necessário
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;

    // Se o tamanho for negativo, lança uma exceção
    if (size_s <= 0)
    {
        throw std::runtime_error("Erro durante a formatação.");
    }
    auto size = static_cast<size_t>(size_s);

    // Usa make_unique para criar o buffer
    auto buf = std::make_unique<char[]>(size);

    // Formata a string
    std::snprintf(buf.get(), size, format.c_str(), args...);

    // Retorna a string formatada
    return std::string(buf.get(), buf.get() + size - 1);
}

// cria novo temporário
long CodeGen::NewTmp(Token *t)
{
    long addr = this->tmp_count;
    if (t->getTokenType() == TOKEN_TYPE_STRING)
    {
        this->tmp_count += this->string_size;
    }
    else if (t->getTokenType() == TOKEN_TYPE_CHAR || t->getTokenType() == TOKEN_TYPE_BOOLEAN)
    {
        this->tmp_count += this->char_size;
    }
    else if (t->getTokenType() == TOKEN_TYPE_INTEGER || t->getTokenType() == TOKEN_TYPE_REAL)
    {
        this->tmp_count += this->number_size;
    }
    return addr;
}
void CodeGen::end()
{
    this->programFile << "mov rax, 60 ;chamada de saída\n";
    this->programFile << "mov rdi, 0 ;código de saida sem erros\n";
    this->programFile << "syscall ;chama o kernel\n";
}
void CodeGen::startData()
{
    this->programFile << "section .data\n";
}
void CodeGen::startText()
{
    this->programFile << "section .text\n";
}
void CodeGen::startProgram()
{
    this->startData();
    this->programFile << "M:\n";
    this->programFile << "\t resb 10000h ;temporários\n";
}
void CodeGen::DeclareVariable(Token *t)
{
    t->getTokenAddr(this->mem_count);
    if (t->getTokenType() == TOKEN_TYPE_CHAR)
    {
        this->mem_count += this->char_size;
        this->programFile << "\t resb 1"
                 << "\n";
    }
    else if (t->getTokenType() == TOKEN_TYPE_INTEGER || t->getTokenType() == TOKEN_TYPE_REAL)
    {
        this->mem_count += this->number_size;
        this->programFile << "\t resd 1"
                 << "\n";
    }
    else if (t->getTokenType() == TOKEN_TYPE_STRING)
    {
        this->mem_count += this->string_size;
        this->programFile << "\t resb 100h"
                 << "\n";
    }
}
void CodeGen::DeclareConst(Token *t, Token *constant)
{
    t->setTokenAddr(this->mem_count);
    if (t->getTokenType() == TOKEN_TYPE_CHAR)
    {
        this->mem_count += constant->getTokeSize();
        this->programFile << format("db %s 1", constant.getLexeme()) << "\n";
    }
    else if (t->getTokenType() == TOKEN_TYPE_INTEGER || t->getTokenType() == TOKEN_TYPE_REAL)
    {
        this->mem_count += constant->getTokeSize();
        this->programFile << format("dd %s", constant.getLexeme()) << "\n";
    }
    else if (t->getTokenType() == TOKEN_TYPE_STRING)
    {
        this->mem_count += constant->getTokeSize();
        this->programFile << format("db %s, 0", constant.getLexeme()) << "\n";
    }
}
// guarda valor de uma constante em um temporário
void CodeGen::storeConstOnTmp(Token *t,Token *constant)
{
    if (constant->getTokenType() == TOKEN_TYPE_REAL)
    {
        t->setTokenAddr(this->mem_count);
        this->mem_count += constant->getTokeSize();
        this->startData();
        this->programFile << format("dd %s", constant.getLexeme()) << "\n";
        this->startText();
    }
    else if (constant->getTokenType() == TOKEN_TYPE_STRING)
    {
        t->setTokenAddr(this->mem_count - this->string_size + constant->getLexeme().size() - 1);
        this->mem_count += constant->getTokeSize();
        this->startData();
        this->programFile << format("db %s,0", constant.getLexeme()) << "\n";
        this->startText();
    }
    else if (constant->getTokenType() == TOKEN_TYPE_CHAR)
    {
        long addr = this->NewTmp(constant);
        t->setTokenAddr(addr);
        this->programFile << format("mov al, %s", constant.getLexeme()) << "\n";
        this->programFile << format("mov [qword M+%ld], al", addr) << "\n";
    }
    else if (constant->getTokenType() == TOKEN_TYPE_INTEGER)
    {
        long addr = this->NewTmp(constant);
        t->setTokenAddr(addr);
        this->programFile << format("mov eax, %s", constant.getLexeme()) << "\n";
        this->programFile << format("mov [qword M+%ld], eax", addr) << "\n";
    }
}
// comando de atribuição
void CodeGen::atributionCommand(Token *id, Token *exp)
{
    if (id->getTokenType() == TOKEN_TYPE_INTEGER)
    {
        this->programFile << format("mov eax,[qword M+%ld]", exp->getTokenAddr()) << "\n";
        this->programFile << format("mov [qword M+%ld], eax", id->getTokenAddr()) << "\n";
    }
    else if (id->getTokenType() == TOKEN_TYPE_REAL)
    {
        this->programFile << format("mov xmm0,[qword M+%ld]", exp->getTokenAddr()) << "\n";
        this->programFile << format("mov [qword M+%ld], xmm0", id->getTokenAddr()) << "\n";
    }
    else if (id->getTokenType() == TOKEN_TYPE_CHAR || id->getTokenType() == TOKEN_TYPE_BOOLEAN)
    {
        this->programFile << format("mov al,[qword M+%ld]", exp->getTokenAddr()) << "\n";
        this->programFile << format("mov [qword M+%ld], al", id->getTokenAddr()) << "\n";
    }
}
// inverte valor da expressão
void CodeGen::invertExpression(Token *exp)
{
    long tmpAddr = this->NewTmp(exp);
    if (exp->getTokenType() == TOKEN_TYPE_INTEGER)
    {
        this->programFile << format("mov eax, [qword M+%ld]", exp->getTokenAddr()) << "\n";
        this->programFile << "neg eax"
                 << "\n";
        exp->setTokenAddr(tmpAddr);
        this->programFile << format("mov [qword M+%ld], eax", tmpAddr) << "\n";
    }
    else if (exp->getTokenType() == TOKEN_TYPE_REAL)
    {
        this->programFile << format("movss xmm0, [qword M+%ld]", exp->getTokenAddr()) << "\n";
        this->programFile << "mov rax, -1"
                 << "\n";
        this->programFile << "cvtsi2ss xmm1, rax"
                 << "\n";
        this->programFile << "mulss xmm0, xmm1"
                 << "\n";
        exp->setTokenAddr(tmpAddr);
        this->programFile << format("movss [qword M+%ld], xmm0", tmpAddr) << "\n";
    }
}
// Operação de soma
void Codegen::sumOperation(Token *op1, Token *op2)
{
    if (op1->getTokenType() == op2->getTokenType())
    {
        if (op1->getTokenType() == TOKEN_TYPE_INTEGER)
        {
            this->programFile << format("mov eax, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("mov ebx, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "add eax,ebx";
            long tmpAddr = this->NewTmp(op1);
            op1->setTokenAddr(tmpAddr);
            this->programFile << format("mov [qword M+%ld], eax", tmpAddr) << "\n";
        }
        else
        {
            this->programFile << format("movss xmm0, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("movss xmm1, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "addss xmm0,xmm1";
            long tmpAddr = this->NewTmp(op1);
            op1->setTokenAddr(tmpAddr);
            this->programFile << format("movss [qword M+%ld], xmm0", tmpAddr) << "\n";
        }
    }
    else
    {
        if (op1->getTokenType() == TOKEN_TYPE_REAL)
        {
            this->programFile << format("mov xmm0, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("mov rax, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "cvtsi2ss xmm1, rax"
                     << "\n";
        }
        else
        {
            this->programFile << format("mov rax, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("mov xmm1, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "cvtsi2ss xmm0, rax"
                     << "\n";
        }
        this->programFile << "addss xmm0, xmm1"
                 << "\n";
        long tmpAddr = this->NewTmp(op1);
        op1->setTokenAddr(tmpAddr);
        this->programFile << format("movss [qword M+%ld], xmm0", tmpAddr) << "\n";
    }
}
// Operação de subtração
void Codegen::subOperation()
{
    if (op1->getTokenType() == op2->getTokenType())
    {
        if (op1->getTokenType() == TOKEN_TYPE_INTEGER)
        {
            this->programFile << format("mov eax, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("mov ebx, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "sub eax,ebx";
            long tmpAddr = this->NewTmp(op1);
            op1->setTokenAddr(tmpAddr);
            this->programFile << format("mov [qword M+%ld], eax", tmpAddr) << "\n";
        }
        else
        {
            this->programFile << format("movss xmm0, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("movss xmm1, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "subss xmm0,xmm1";
            long tmpAddr = this->NewTmp(op1);
            op1->setTokenAddr(tmpAddr);
            this->programFile << format("movss [qword M+%ld], xmm0", tmpAddr) << "\n";
        }
    }
    else
    {
        if (op1->getTokenType() == TOKEN_TYPE_REAL)
        {
            this->programFile << format("mov xmm0, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("mov rax, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "cvtsi2ss xmm1, rax"
                     << "\n";
        }
        else
        {
            this->programFile << format("mov rax, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("mov xmm1, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "cvtsi2ss xmm0, rax"
                     << "\n";
        }
        this->programFile << "subss xmm0, xmm1"
                 << "\n";
        long tmpAddr = this->NewTmp(op1);
        op1->setTokenAddr(tmpAddr);
        this->programFile << format("movss [qword M+%ld], xmm0", tmpAddr) << "\n";
    }
}
// Operação de multiplicação
void Codegen::multiplyOperation(Token *op1, Token *op2)
{
    if (op1->getTokenType() == op2->getTokenType())
    {
        if (op1->getTokenType() == TOKEN_TYPE_INTEGER)
        {
            this->programFile << format("mov eax, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("mov ebx, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "imul eax,ebx";
            long tmpAddr = this->NewTmp(op1);
            op1->setTokenAddr(tmpAddr);
            this->programFile << format("mov [qword M+%ld], eax", tmpAddr) << "\n";
        }
        else
        {
            this->programFile << format("movss xmm0, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("movss xmm1, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "mulss xmm0,xmm1";
            long tmpAddr = this->NewTmp(op1);
            op1->setTokenAddr(tmpAddr);
            this->programFile << format("movss [qword M+%ld], xmm0", tmpAddr) << "\n";
        }
    }
    else
    {
        if (op1->getTokenType() == TOKEN_TYPE_REAL)
        {
            this->programFile << format("mov xmm0, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("mov rax, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "cvtsi2ss xmm1, rax"
                     << "\n";
        }
        else
        {
            this->programFile << format("mov rax, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("mov xmm1, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "cvtsi2ss xmm0, rax"
                     << "\n";
        }
        this->programFile << "mulss xmm0, xmm1"
                 << "\n";
        long tmpAddr = this->NewTmp(op1);
        op1->setTokenAddr(tmpAddr);
        this->programFile << format("movss [qword M+%ld], xmm0", tmpAddr) << "\n";
    }
}
// Operaçãp de divisão
void Codegen::divideOperation(Token *op1, Token *op2)
{
    if (op1->getTokenType() == op2->getTokenType())
    {
        if (op1->getTokenType() == TOKEN_TYPE_INTEGER)
        {
            this->programFile << format("mov eax, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("mov ebx, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "idiv eax,ebx";
            long tmpAddr = this->NewTmp(op1);
            op1->setTokenAddr(tmpAddr);
            this->programFile << format("mov [qword M+%ld], eax", tmpAddr) << "\n";
        }
        else
        {
            this->programFile << format("movss xmm0, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("movss xmm1, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "divss xmm0,xmm1";
            long tmpAddr = this->NewTmp(op1);
            op1->setTokenAddr(tmpAddr);
            this->programFile << format("movss [qword M+%ld], xmm0", tmpAddr) << "\n";
        }
    }
    else
    {
        if (op1->getTokenType() == TOKEN_TYPE_REAL)
        {
            this->programFile << format("mov xmm0, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("mov rax, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "cvtsi2ss xmm1, rax"
                     << "\n";
        }
        else
        {
            this->programFile << format("mov rax, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("mov xmm1, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "cvtsi2ss xmm0, rax"
                     << "\n";
        }
        this->programFile << "divss xmm0, xmm1"
                 << "\n";
        long tmpAddr = this->NewTmp(op1);
        op1->setTokenAddr(tmpAddr);
        this->programFile << format("movss [qword M+%ld], xmm0", tmpAddr) << "\n";
    }
}
// negação da expressão
void CodeGen::negExpression(Token *exp)
{
    long tmpAddr = this->NewTmp(exp);
    this->programFile << format("mov al, [qword M+%ld]", exp->getTokenAddr()) << "\n";
    this->programFile << "neg al"
             << "\n";
    this->programFile << "add al,1"
             << "\n";
    exp->setTokenAddr(tmpAddr);
    this->programFile << format("mov [qword M+%ld], al", tmpAddr) << "\n";
}
// converte valor inteiro para real
void CodeGen::cvtToReal(Token *t)
{
    this->programFile << format("mov rax, [qword M+%ld]", t->getTokenAddr()) << "\n";
    long tmpAddr = this->NewTmp(t);
    t->setTokenAddr(tmpAddr);
    this->programFile << "cvtsi2ss xmm0, rax"
             << "\n";
    this->programFile << format("movss [qword M+%d], xmm0", tmpAddr);
}
// converte valor real para inteiro
void CodeGen::cvtToInt(Token *t)
{
    this->programFile << format("mov xmm0, [qword M+%ld]", t->getTokenAddr()) << "\n";
    long tmpAddr = this->NewTmp(t);
    t->setTokenAddr(tmpAddr);
    this->programFile << "cvtsi2ss rax, xmm0"
             << "\n";
    this->programFile << format("mov [qword M+%d], rax", tmpAddr);
}
//Escreve no terminal
void CodeGen::write(Token *t)
{
    long bufferAddr;
    int label1;
    int label2;
    int label3;
    int label4;
    int label5;
    if (t->getTokenType() == TOKEN_TYPE_STRING)
    {
        bufferAddr = t->getTokenAddr();
        this->programFile << (format("mov rsi, M+%d", bufferAddr));
        this->programFile << "mov rdx, 100h";
        this->programFile <<"mov rax, 1";
        this->programFile << "mov rdi, 1";
        this->programFile << "syscall";
    }else if (t->getTokenType() == TOKEN_TYPE_CHAR)
    {
        bufferAddr = t->getTokenAddr();
        this->programFile << format("mov rsi, M+%d", bufferAddr)<<"\n";
        this->programFile << "mov rdx, 1"<<"\n";
        this->programFile << "mov rax, 1"<<"\n";
        this->programFile << "mov rdi, 1"<<"\n";
        this->programFile << "syscall";
    }else if (t->getTokenType() == TOKEN_TYPE_REAL)
    {
        bufferAddr = this->tmp_count;
        this->tmp_count += this->string_size;
        label1 = newLabel();
        label2 = newLabel();
        label3 = newLabel();
        label4 = newLabel();
        label5 = newLabel();

        this->programFile << format("movss xmm0, [qword M+%ld]", t->get_endereco())<<"\n";
        this->programFile << format("mov rsi, M+ %ld", buffer_address)<<"\n";
        this->programFile << "mov rcx, 0 "<<"\n";
        this->programFile << "mov rdi, 6 "<<"\n";
        this->programFile << "mov rbx, 10 "<<"\n";
        this->programFile << "cvtsi2ss xmm2, rbx "<<"\n";
        this->programFile << "subss xmm1, xmm1 " <<"\n";
        this->programFile << "comiss xmm0, xmm1 "<<"\n";
        this->programFile << format("jae l%d", label1)<<"\n";
        this->programFile << "mov dl, '-' "<<"\n";
        this->programFile << "mov [rsi], dl"<<"\n";
        this->programFile <<"mov rdx, -1 ;Carrega -1 em RDX"<<"\n";
        this->programFile <<"cvtsi2ss xmm1, rdx ;Converte para real"<<"\n";
        this->programFile <<"mulss xmm0, xmm1 ;Toma módulo"<<"\n";
        this->programFile <<"add rsi, 1 ;incrementa índice"<<"\n";
        this->programFile <<format("l%d:", label1),1<<"\n";
        this->programFile <<"roundss xmm1, xmm0, 0b0011 ;parte inteira xmm1"<<"\n";
        this->programFile <<"subss xmm0, xmm1"<<"\n";
        this->programFile <<"cvtss2si rax, xmm1"<<"\n";
        this->programFile <<format("l%d:", label2),1<<"\n";
        this->programFile <<"add rcx, 1"<<"\n";
        this->programFile <<"cdq"<<"\n";
        this->programFile <<"idiv ebx"<<"\n";
        this->programFile <<"push dx"<<"\n";
        this->programFile <<"cmp eax, 0"<<"\n";
        this->programFile <<format("jne l%d", label2)<<"\n";
        this->programFile <<"sub rdi, rcx"<<"\n";
        this->programFile <<format("l%d:", label3),1<<"\n";
        this->programFile <<"pop dx"<<"\n";
        this->programFile <<"add dl, '0'"<<"\n";
        this->programFile <<"mov [rsi], dl"<<"\n";
        this->programFile <<"add rsi, 1"<<"\n";
        this->programFile <<"sub rcx, 1"<<"\n";
        this->programFile <<"cmp rcx, 0"<<"\n";
        this->programFile <<format("jne l%d", label3)<<"\n";
        this->programFile <<"mov dl, '.'"<<"\n";
        this->programFile <<"mov [rsi], dl"<<"\n";
        this->programFile <<"add rsi, 1"<<"\n";
        this->programFile <<format("l%d:", label4),1<<"\n";
        this->programFile <<"cmp rdi, 0"<<"\n";
        this->programFile <<format("jle l%d", label5)<<"\n";
        this->programFile <<"mulss xmm0,xmm2"<<"\n";
        this->programFile <<"roundss xmm1,xmm0,0b0011"<<"\n";
        this->programFile <<"subss xmm0,xmm1"<<"\n";
        this->programFile <<"cvtss2si rdx, xmm1"<<"\n";
        this->programFile <<"add dl, '0'"<<"\n";
        this->programFile <<"mov [rsi], dl"<<"\n";
        this->programFile <<"add rsi, 1"<<"\n";
        this->programFile <<"sub rdi, 1"<<"\n";
        this->programFile <<format("jmp l%d", label4)<<"\n";
        this->programFile <<format("l%d:", label5),1<<"\n";
        this->programFile <<"mov dl, 0"<<"\n";
        this->programFile <<"mov [rsi], dl"<<"\n";
        this->programFile <<"mov rdx, rsi"<<"\n";
        this->programFile <<format("mov rbx, M+%ld", buffer_address)<<"\n";
        this->programFile <<"sub rdx, rbx"<<"\n";
        this->programFile <<format("mov rsi, M+%ld", buffer_address)<<"\n";
        this->programFile <<"mov rax, 1"<<"\n";
        this->programFile <<"mov rdi, 1"<<"\n";
        this->programFile <<"syscall"<<"\n";
    }
    else if(t->getTokenType() == TOKEN_TYPE_INTEGER)
    {
        bufferAddr = this->tmp_count;
        this->tmp_count += this->string_size;
        label1 = new_label();
        label2 = new_label();
        label3 = new_label();

        this->programFile <<format("mov eax, [qword M+%ld]", t->get_endereco())<<"\n";
        this->programFile <<format("mov rsi, M+%ld", buffer_address)<<"\n";
        this->programFile <<"mov rcx, 0"<<"\n";
        this->programFile <<"mov rdi, 0"<<"\n";
        this->programFile <<"cmp eax, 0"<<"\n";
        this->programFile <<format("jge l%d",label1)<<"\n";
        this->programFile <<"mov bl, '-' "<<"\n";
        this->programFile <<"mov [rsi], bl"<<"\n";
        this->programFile <<"add rsi, 1"<<"\n";
        this->programFile <<"add rdi, 1"<<"\n";
        this->programFile <<"neg eax"<<"\n";
        this->programFile <<format("l%ld:",label1),1<<"\n";
        this->programFile <<"mov ebx, 10"<<"\n";
        this->programFile <<format("l%ld:",label2),1<<"\n";
        this->programFile <<"add rcx, 1"<<"\n";
        this->programFile <<"cdq"<<"\n";
        this->programFile <<"idiv ebx"<<"\n";
        this->programFile <<"push dx"<<"\n";
        this->programFile <<"cmp eax, 0"<<"\n";
        this->programFile <<format("jne l%d", label2)<<"\n";
        this->programFile <<"add rdi,rcx"<<"\n";
        this->programFile <<format("l%ld:",label3),1<<"\n";
        this->programFile <<"pop dx"<<"\n";
        this->programFile <<"add dl, '0'"<<"\n";
        this->programFile <<"mov [rsi], dl"<<"\n";
        this->programFile <<"add rsi, 1"<<"\n";
        this->programFile <<"sub rcx, 1"<<"\n";
        this->programFile <<"cmp rcx, 0"<<"\n";
        this->programFile <<format("jne l%d", label3)<<"\n";
        this->programFile <<format("mov rsi, M+%ld",buffer_address)<<"\n";
        this->programFile <<"mov rax, 1"<<"\n";
        this->programFile <<"mov rdi, 1"<<"\n";
        this->programFile <<"syscall"<<"\n";
    }
}
//
void CodeGen::writeLine()
{
    this->programFile <<format("mov rsi, M+%ld", buffer_address)<<"\n";
    this->programFile <<"mov rax, 1"<<"\n";
    this->programFile <<"mov rdi, 1"<<"\n";
    this->programFile <<"syscall"<<"\n";
}