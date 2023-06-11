#include <string>
#include <fstream>
#include "../symbols/token.hpp"
#include "../symbols/token-type.hpp"
#include "code-gen.hpp"
#include <memory>
#include <stdexcept>
#include <cstdio>

//Construtor
CodeGen::CodeGen(std::string asmFileName)
{
    this->mem_count = 0x10000;
    this->tmp_count = 0x0;
    this->label_counter = 0;
    this->asmFileName = asmFileName;
    this->programFile.open(asmFileName);
    startProgram();
}
//Destrutor (encerra o fluxo de dados para o arquivo Assembly)
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
//encerra o programa
void CodeGen::end()
{
    this->programFile << "\tmov rax, 60 ;chamada de saída\n";
    this->programFile << "\tmov rdi, 0 ;código de saida sem erros\n";
    this->programFile << "\tsyscall ;chama o kernel\n";
}
//cria nova seção de dados
void CodeGen::startData()
{
    this->programFile << "section .data\n";
}
//cria nova seção de codigo
void CodeGen::startText()
{
    this->programFile << "section .text\n";
}
//inicializa o programa
void CodeGen::startProgram()
{
    this->programFile << "global _start"<<"\n";
    this->startData();
    this->programFile << "M:\n";
    this->programFile << "\tresb 10000h ;temporários\n";
    this->startText();
    this->programFile << "_start:"<<"\n";
}
//comando de declaração de variável
void CodeGen::DeclareVariable(Token *t)
{
    t->setTokenAddr(this->mem_count);
    if (t->getTokenType() == TOKEN_TYPE_CHAR || t->getTokenType()== TOKEN_TYPE_BOOLEAN)
    {
        this->mem_count += this->char_size;
        this->programFile << "\tresb 1"
                 << "\n";
    }
    else if (t->getTokenType() == TOKEN_TYPE_INTEGER || t->getTokenType() == TOKEN_TYPE_REAL)
    {
        this->mem_count += this->number_size;
        this->programFile << "\tresd 1"
                 << "\n";
    }
    else if (t->getTokenType() == TOKEN_TYPE_STRING)
    {
        this->mem_count += this->string_size;
        this->programFile << "\tresb 100h"
                 << "\n";
    }
}
//comando de declaração de constante (final)
void CodeGen::DeclareConst(Token *t, Token *constant)
{
    t->setTokenAddr(this->mem_count);
    if (t->getTokenType() == TOKEN_TYPE_CHAR || t->getTokenType()== TOKEN_TYPE_BOOLEAN)
    {
        this->mem_count += this->char_size;
        this->programFile << format("\tdb \'%s\' 1", constant->getLexeme().c_str()) << "\n";
    }
    else if (t->getTokenType() == TOKEN_TYPE_INTEGER || t->getTokenType() == TOKEN_TYPE_REAL)
    {
        this->mem_count += this->number_size;
        this->programFile << format("\tdd %s", constant->getLexeme().c_str()) << "\n";
    }
    else if (t->getTokenType() == TOKEN_TYPE_STRING)
    {
        t->setTokenSize(constant->getLexeme().size());
        this->mem_count += constant->getLexeme().size();
        this->programFile << format("\tdb \'%s\', %d", constant->getLexeme().c_str(),constant->getLexeme().size()) << "\n";
    }
}
// guarda valor de uma constante em um temporário
void CodeGen::storeConstOnTmp(Token *t,Token *constant)
{
    if (constant->getTokenType() == TOKEN_TYPE_REAL)
    {
        t->setTokenAddr(this->mem_count);
        this->mem_count += this->number_size;
        this->startData();
        this->programFile << format("dd %s", constant->getLexeme().c_str()) << "\n";
        this->startText();
    }
    else if (constant->getTokenType() == TOKEN_TYPE_STRING)
    {
        t->setTokenAddr(this->mem_count);
        t->setTokenSize(constant->getLexeme().size());
        this->mem_count += constant->getLexeme().size();
        this->startData();
        this->programFile << format("db \'%s\', %d", constant->getLexeme().c_str(),constant->getLexeme().size()) << "\n";
        this->startText();
    }
    else if (constant->getTokenType() == TOKEN_TYPE_CHAR)
    {
        long addr = this->NewTmp(constant);
        t->setTokenAddr(addr);
        this->programFile << format("mov al, \'%s\'", constant->getLexeme().c_str()) << "\n";
        this->programFile << format("mov [qword M+%ld], al", addr) << "\n";
    }
    else if (constant->getTokenType() == TOKEN_TYPE_INTEGER)
    {
        long addr = this->NewTmp(constant);
        t->setTokenAddr(addr);
        this->programFile << format("mov eax, %s", constant->getLexeme().c_str()) << "\n";
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
        this->programFile << format("movss xmm0,[qword M+%ld]", exp->getTokenAddr()) << "\n";
        this->programFile << format("movss [qword M+%ld], xmm0", id->getTokenAddr()) << "\n";
    }
    else if (id->getTokenType() == TOKEN_TYPE_CHAR || id->getTokenType() == TOKEN_TYPE_BOOLEAN)
    {
        this->programFile << format("mov al,[qword M+%ld]", exp->getTokenAddr()) << "\n";
        this->programFile << format("mov [qword M+%ld], al", id->getTokenAddr()) << "\n";
    }else if(id->getTokenType() == TOKEN_TYPE_STRING){
        id->setTokenSize(exp->getTokeSize());
        int label1 = newLabel();
        int label2 = newLabel();
        this->programFile << format("mov rsi, qword M+%ld",id->getTokenAddr())<<"\n";
        this->programFile << format("mov rdi, qword M+%ld",exp->getTokenAddr())<<"\n";
        this->programFile << format("Rot%d:",label1)<<"\n";
        this->programFile << "mov al, [rdi]\n";
        this->programFile << "mov [rsi], al\n";
        this->programFile << "cmp al, '' \n";
        this->programFile << format("je Rot%d",label2)<<"\n";
        this->programFile <<"add rdi, 1\n";
        this->programFile <<"add rsi, 1\n";
        this->programFile << format("jmp Rot%d",label1)<<"\n";
        this->programFile << format("Rot%d:",label2)<<"\n";
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
void CodeGen::sumOperation(Token *op1, Token *op2)
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
void CodeGen::subOperation(Token *op1, Token *op2)
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
// Operação de or
void CodeGen::orOperation(Token *op1,Token *op2){
    this->programFile << format("mov eax, [qword M+%ld]", op1->getTokenAddr())<<"\n";
    this->programFile << format("mov ebx, [qword M+%ld]", op1->getTokenAddr())<<"\n";
    this->programFile << format("mov ecx, [qword M+%ld]", op2->getTokenAddr())<<"\n";
    this->programFile << "imul ecx"<<"\n";
    this->programFile << "add ebx, ecx";
    this->programFile << "sub ebx, eax";

    long tmpAddr = this->tmp_count;
    this->tmp_count += this->char_size;
    op1->setTokenAddr(tmpAddr);
    this->programFile << format("mov [qword M+%ld],ebx",tmpAddr)<<"\n";
} 
// Operação de multiplicação
void CodeGen::multiplyOperation(Token *op1, Token *op2)
{
    if (op1->getTokenType() == op2->getTokenType())
    {
        if (op1->getTokenType() == TOKEN_TYPE_INTEGER)
        {
            this->programFile << format("mov eax, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("mov ebx, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "imul ebx";
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
// Operação de divisão
void CodeGen::divideOperation(Token *op1, Token *op2)
{
    if (op1->getTokenType() == op2->getTokenType())
    {
        if (op1->getTokenType() == TOKEN_TYPE_INTEGER)
        {
            this->programFile << format("mov eax, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("mov ebx, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "idiv ebx";
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
//Operação de mod
void CodeGen::modOperation(Token *op1,Token *op2){
    this->programFile << format("mov eax, [qword M+%ld]", op1->getTokenAddr())<<"\n";
    this->programFile << format("mov ebx, [qword M+%ld]", op1->getTokenAddr())<<"\n";
    this->programFile << "idiv ebx"<<"\n";
    long tmpAddr = NewTmp(op1);
    op1->setTokenAddr(tmpAddr);
    this->programFile << format("mov [qword M+%ld], edx", tmpAddr)<<"\n";

}
//Operação de and
void CodeGen::andOperation(){

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
/**
 * @brief Escreve no terminal
*/
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
        this->programFile << format("\tmov rsi, M+%ld", bufferAddr)<<"\n";
        this->programFile << format("\tmov rdx, %d",t->getTokeSize())<<"\n";
        this->programFile <<"\tmov rax, 1"<<"\n";
        this->programFile << "\tmov rdi, 1"<<"\n";
        this->programFile << "\tsyscall"<<"\n";
    }else if (t->getTokenType() == TOKEN_TYPE_CHAR)
    {
        bufferAddr = t->getTokenAddr();
        this->programFile << format("mov rsi, M+%ld", bufferAddr)<<"\n";
        this->programFile << "mov rdx, 1"<<"\n";
        this->programFile << "mov rax, 1"<<"\n";
        this->programFile << "mov rdi, 1"<<"\n";
        this->programFile << "syscall"<<"\n";
    }else if (t->getTokenType() == TOKEN_TYPE_REAL)
    {
        bufferAddr = this->tmp_count;
        this->tmp_count += this->string_size;
        label1 = newLabel();
        label2 = newLabel();
        label3 = newLabel();
        label4 = newLabel();
        label5 = newLabel();

        this->programFile << format("movss xmm0, [qword M+%ld]", t->getTokenAddr())<<"\n";
        this->programFile << format("mov rsi, M+%ld", bufferAddr)<<"\n";
        this->programFile << "mov rcx, 0"<<"\n";
        this->programFile << "mov rdi, 6"<<"\n";
        this->programFile << "mov rbx, 10 "<<"\n";
        this->programFile << "cvtsi2ss xmm2, rbx"<<"\n";
        this->programFile << "subss xmm1, xmm1" <<"\n";
        this->programFile << "comiss xmm0, xmm1"<<"\n";
        this->programFile << format("jae Rot%d",label1)<<"\n";
        this->programFile << "mov dl, '-'"<<"\n";
        this->programFile << "mov [rsi], dl"<<"\n";
        this->programFile <<"mov rdx, -1"<<"\n";
        this->programFile <<"cvtsi2ss xmm1, rdx"<<"\n";
        this->programFile <<"mulss xmm0, xmm1"<<"\n";
        this->programFile <<"add rsi, 1"<<"\n";
        this->programFile <<format("Rot%d:", label1)<<"\n";
        this->programFile <<"roundss xmm1, xmm0, 0b0011"<<"\n";
        this->programFile <<"subss xmm0, xmm1"<<"\n";
        this->programFile <<"cvtss2si rax, xmm1"<<"\n";
        this->programFile <<format("Rot%d:", label2)<<"\n";
        this->programFile <<"add rcx, 1"<<"\n";
        this->programFile <<"cdq"<<"\n";
        this->programFile <<"idiv ebx"<<"\n";
        this->programFile <<"push dx"<<"\n";
        this->programFile <<"cmp eax, 0"<<"\n";
        this->programFile <<format("jne Rot%d", label2)<<"\n";
        this->programFile <<"sub rdi, rcx"<<"\n";
        this->programFile <<format("Rot%d:", label3)<<"\n";
        this->programFile <<"pop dx"<<"\n";
        this->programFile <<"add dl, '0'"<<"\n";
        this->programFile <<"mov [rsi], dl"<<"\n";
        this->programFile <<"add rsi, 1"<<"\n";
        this->programFile <<"sub rcx, 1"<<"\n";
        this->programFile <<"cmp rcx, 0"<<"\n";
        this->programFile <<format("jne Rot%d", label3)<<"\n";
        this->programFile <<"mov dl, '.'"<<"\n";
        this->programFile <<"mov [rsi], dl"<<"\n";
        this->programFile <<"add rsi, 1"<<"\n";
        this->programFile <<format("Rot%d:", label4)<<"\n";
        this->programFile <<"cmp rdi, 0"<<"\n";
        this->programFile <<format("jle Rot%d", label5)<<"\n";
        this->programFile <<"mulss xmm0,xmm2"<<"\n";
        this->programFile <<"roundss xmm1,xmm0,0b0011"<<"\n";
        this->programFile <<"subss xmm0,xmm1"<<"\n";
        this->programFile <<"cvtss2si rdx, xmm1"<<"\n";
        this->programFile <<"add dl, '0'"<<"\n";
        this->programFile <<"mov [rsi], dl"<<"\n";
        this->programFile <<"add rsi, 1"<<"\n";
        this->programFile <<"sub rdi, 1"<<"\n";
        this->programFile <<format("jmp Rot%d", label4)<<"\n";
        this->programFile <<format("Rot%d:", label5)<<"\n";
        this->programFile <<"mov dl, 0"<<"\n";
        this->programFile <<"mov [rsi], dl"<<"\n";
        this->programFile <<"mov rdx, rsi"<<"\n";
        this->programFile <<format("mov rbx, M+%ld", bufferAddr)<<"\n";
        this->programFile <<"sub rdx, rbx"<<"\n";
        this->programFile <<format("mov rsi, M+%ld", bufferAddr)<<"\n";
        this->programFile <<"mov rax, 1"<<"\n";
        this->programFile <<"mov rdi, 1"<<"\n";
        this->programFile <<"syscall"<<"\n";
    }
    else if(t->getTokenType() == TOKEN_TYPE_INTEGER)
    {
        bufferAddr = this->tmp_count;
        this->tmp_count += this->string_size;
        label1 = newLabel();
        label2 = newLabel();
        label3 = newLabel();

        this->programFile <<format("mov eax, [qword M+%ld]", t->getTokenAddr())<<"\n";
        this->programFile <<format("mov rsi, M+%ld", bufferAddr)<<"\n";
        this->programFile <<"mov rcx, 0"<<"\n";
        this->programFile <<"mov rdi, 0"<<"\n";
        this->programFile <<"cmp eax, 0"<<"\n";
        this->programFile <<format("jge Rot%d",label1)<<"\n";
        this->programFile <<"mov bl, '-' "<<"\n";
        this->programFile <<"mov [rsi], bl"<<"\n";
        this->programFile <<"add rsi, 1"<<"\n";
        this->programFile <<"add rdi, 1"<<"\n";
        this->programFile <<"neg eax"<<"\n";
        this->programFile <<format("Rot%d:",label1)<<"\n";
        this->programFile <<"mov ebx, 10"<<"\n";
        this->programFile <<format("Rot%d:",label2)<<"\n";
        this->programFile <<"add rcx, 1"<<"\n";
        this->programFile <<"cdq"<<"\n";
        this->programFile <<"idiv ebx"<<"\n";
        this->programFile <<"push dx"<<"\n";
        this->programFile <<"cmp eax, 0"<<"\n";
        this->programFile <<format("jne Rot%d", label2)<<"\n";
        this->programFile <<"add rdi,rcx"<<"\n";
        this->programFile <<format("Rot%d:",label3)<<"\n";
        this->programFile <<"pop dx"<<"\n";
        this->programFile <<"add dl, '0'"<<"\n";
        this->programFile <<"mov [rsi], dl"<<"\n";
        this->programFile <<"add rsi, 1"<<"\n";
        this->programFile <<"sub rcx, 1"<<"\n";
        this->programFile <<"cmp rcx, 0"<<"\n";
        this->programFile <<format("jne Rot%d", label3)<<"\n";
        this->programFile <<format("mov rsi, M+%ld",bufferAddr)<<"\n";
        this->programFile << "mov rdx, 1"<<"\n";
        this->programFile <<"mov rax, 1"<<"\n";
        this->programFile <<"mov rdi, 1"<<"\n";
        this->programFile <<"syscall"<<"\n";
    }
}
/**
 * @brief Escreve no terminal e quebra a linha
*/
void CodeGen::writeln(Token *t){}

/**
 * @brief Montagem de codigo para leitura do teclado
 */
void CodeGen::readln(Token *t)
{
    if (t->getTokenType() == TOKEN_TYPE_STRING)
    {
        writeInProgramFile(format("mov rsi, M + %ld", t->getTokenAddr()));
        writeInProgramFile("rdx, 100h");
        writeInProgramFile("mov, rax, 0");
        writeInProgramFile("mov rdi, 0");
        writeInProgramFile("syscall");
        writeInProgramFile((format("mov byte [M+%ld+rax-1], 0", t->getTokenAddr())));
    }
    else if (t->getTokenType() == TOKEN_TYPE_CHAR)
    {
        writeInProgramFile(format("mov rsi, M + %ld", t->getTokenAddr()));
        writeInProgramFile("mov rdx, 1");
        writeInProgramFile("mov rax, 0");
        writeInProgramFile("mov rdi, 0");
        writeInProgramFile("syscall");
    }
    else if (t->getTokenType() == TOKEN_TYPE_INTEGER)
    {
        int label1 = newLabel();
        int label2 = newLabel();
        int label3 = newLabel();
        
    }
    else if (t->getTokenType() == TOKEN_TYPE_REAL)
    {
    }
}

long CodeGen::newTmpByTokenType(TokenType tt)
{
    long point = this->tmp_count;
    if (tt == TOKEN_TYPE_CHAR)
    {
        this->tmp_count += this->char_size;
    }
    else if (tt == TOKEN_TYPE_STRING)
    {
        this->tmp_count += this->string_size;
    }
    else if (tt == TOKEN_TYPE_BOOLEAN)
    {
        this->tmp_count += this->char_size;
    }
    else if (tt == TOKEN_TYPE_INTEGER)
    {
        this->tmp_count += this->number_size;
    }
    else if (tt == TOKEN_TYPE_REAL)
    {
        this->tmp_count += this->number_size;
    }
    return point;
}
/**
 * @brief escreve dentro da variavel para o arquivo
 */
void CodeGen::writeInProgramFile(std::string s){
 this->programFile << s << "\n";
}
/**
 * @brief uma declaração condicional 

*/
void CodeGen::initCondition(Token *t, int startLabel) {
    writeInProgramFile(format("mov al, [qword M + %ld]", t->getTokenAddr()));
    writeInProgramFile("cmp al, 0");
    writeInProgramFile(format("je Rot%d", startLabel));
}

/**
 * @brief Finaliza um bloco de código (bloco condicional)
 */
void CodeGen::finalizeBlock( int startLabel, int endLabel) {
    writeInProgramFile(format("jmp Rot%d", startLabel));
    writeInProgramFile(format("Rot%d:", endLabel));
}

/**
 * @brief Finaliza uma cadeia condicional (if/else)
 */
void CodeGen::finalizeConditionalChain(bool onElse, int startLabel, int endLabel) {
    onElse?
        writeInProgramFile(format("Rot%d:", endLabel)) // Se estamos terminando um bloco else, marque com o rótulo de fim
    : 
        writeInProgramFile(format("Rot%d:", startLabel)); // Se estamos terminando um bloco if, marque com o rótulo de início
}
/**
 * @brief Escreve um rótulo no programa
 */
void CodeGen::writeRot(int rot){
    writeInProgramFile(format("Rot%d:",rot));
}
/**
 * @brief Escreve um jump no programa
 */
void CodeGen::writeJump(int rot){
    writeInProgramFile(format("jmp Rot%d",rot));
}
/**
 * @brief Compara a expressão do comando de for
 * pula para fora do loop em caso da expressão ser falsa
 */
void CodeGen::compareForExpression(Token *exp,int rot){
    writeInProgramFile(format("mov al, [qword M+%ld]",exp->getTokenAddr()));
    writeInProgramFile("cmp al, 1");
    writeInProgramFile(format("jne Rot%d",rot));
}
/**
 * @brief Move o elemento de uma posição de um vetor para um temporário
*/
void CodeGen::vectorAccess(Token *id, Token *exp, Token *t){
    long tmpAddr = NewTmp(t);
    t->setTokenAddr(tmpAddr);
    writeInProgramFile(format("mov rcx, qword M+%ld",id->getTokenAddr()));
    writeInProgramFile(format("mov ebx, [qword M+%ld]",exp->getTokenAddr()));
    if(t->getTokenType() == TOKEN_TYPE_INTEGER || t->getTokenType() == TOKEN_TYPE_REAL){
        writeInProgramFile("add ebx, ebx");
        writeInProgramFile("add ebx, ebx");
    }else if(t->getTokenType() == TOKEN_TYPE_STRING){
        writeInProgramFile("mov eax, 100h");
        writeInProgramFile("imul ebx");
        writeInProgramFile("mov ebx,eax ");
    }
    writeInProgramFile("add ecx, ebx");
    writeInProgramFile("mov ecx, [rcx]");
    writeInProgramFile(format("mov [qword M+%ld], eax",tmpAddr));
}
/**
 * @brief Operadores relacionais
*/
void CodeGen::charRelacionalOperator(Token *op1, Token *op2, TokenID op){
    int label1=this->newLabel();
    int label2=this->newLabel();

    writeInProgramFile(format("mov al, [qword M+%ld]", op1->getTokenAddr()));
    writeInProgramFile(format("mov bl, [qword M+%ld]", op2->getTokenAddr()));
    writeInProgramFile("cmp al,bl");
    
    if(op==TOKEN_ID_EQUALS){
        writeInProgramFile(format("je Rot%d",label1));
    }else if(op==TOKEN_ID_LESS_THAN){
        writeInProgramFile(format("jl Rot%d",label1));
    }else if(op==TOKEN_ID_LESS_EQUAL_TO){
        writeInProgramFile(format("jle Rot%d",label1));
    }else if(op==TOKEN_ID_GREATER_THEN){
        writeInProgramFile(format("jg Rot%d",label1));
    }else if(op==TOKEN_ID_GREATER_EQUAL_TO){
        writeInProgramFile(format("jge Rot%d",label1));
    }else if(op==TOKEN_ID_DIFFERENT){
        writeInProgramFile(format("jne Rot%d",label1));
    }

    long tmpAddr = this->NewTmp(op1);
    writeInProgramFile(format("mov cl, 0",tmpAddr));
    writeInProgramFile(format("mov [qword M+%ld], cl",tmpAddr));
    writeInProgramFile(format("jmp Rot%d",label2));
    writeInProgramFile(format("Rot%d",label1));
    writeInProgramFile(format("mov cl, 1",tmpAddr));
    writeInProgramFile(format("mov [qword M+%ld], cl",tmpAddr));
    writeInProgramFile(format("Rot%d",label2));

    op1->setTokenAddr(tmpAddr);
    op1->setTokenType(TOKEN_TYPE_BOOLEAN);
}


/**
 * @brief Montagem de codigo para leitura do teclado
 */
void CodeGen::readln(Token *t)
{
    if (t->getTokenType() == TOKEN_TYPE_STRING)
    {
        writeInProgramFile(format("mov rsi, M + %ld", t->getTokenAddr()));
        writeInProgramFile("rdx, 100h");
        writeInProgramFile("mov, rax, 0");
        writeInProgramFile("mov rdi, 0");
        writeInProgramFile("syscall");
        writeInProgramFile((format("mov byte [M+%ld+rax-1], 0", t->getTokenAddr())));
    }
    else if (t->getTokenType() == TOKEN_TYPE_CHAR)
    {
        writeInProgramFile(format("mov rsi, M + %ld", t->getTokenAddr()));
        writeInProgramFile("mov rdx, 1");
        writeInProgramFile("mov rax, 0");
        writeInProgramFile("mov rdi, 0");
        writeInProgramFile("syscall");
    }
    else if (t->getTokenType() == TOKEN_TYPE_INTEGER)
    {
        int label0 = newLabel();
        int label1 = newLabel();
        int label2 = newLabel();
        int label3 = newLabel();

        long tmpbuff = newTmpByTokenType(TOKEN_TYPE_STRING);
        t->setTokenAddr(tmpbuff);

        // Leitura da String
        writeInProgramFile(format("mov rsi, M + %ld", t->getTokenAddr()));
        writeInProgramFile("rdx, 100h");
        writeInProgramFile("mov, rax, 0");
        writeInProgramFile("mov rdi, 0");
        writeInProgramFile("syscall");
        writeInProgramFile((format("mov byte [M+%ld+rax-1], 0", t->getTokenAddr())));

        writeInProgramFile("mov eax, 0                  ;acumulador");
        writeInProgramFile("mov ebx, 0                  ;caractere");
        writeInProgramFile("mov ecx, 10                 ;base 10");
        writeInProgramFile("mov dx, 1                   ;sinal");

        writeInProgramFile(format("mov rsi, M + %ld", t->getTokenAddr()));
        writeInProgramFile("mov bl,[rsi]                ;carrega caractere");
        writeInProgramFile("cmp bl, '-'                 ;sinal?");
        writeInProgramFile(format("jne Rot%d                    ;se dif -, salta", label0));
        writeInProgramFile("mov dx, -1                  ;senão, armazena -");
        writeInProgramFile("add rsi, 1                  ;inc. ponteiro string");
        writeInProgramFile("mov bl, [rsi]               ;carrega caractere");

        // Rotulo 0
        writeInProgramFile(format("Rot%d:", label0));
        writeInProgramFile("push dx                     ;empilha sinal");
        writeInProgramFile("mov edx, 0                  ;reg. multiplicação");

        // Rotulo 1
        writeInProgramFile(format("Rot%d:", label1));
        writeInProgramFile("cmp bl, 0Ah                 ;verifica fim string");
        writeInProgramFile(format("je Rot%d                    ;salta se fim string", label2));
        writeInProgramFile("imul ecx                    ;mult. eax por 10");
        writeInProgramFile("sub bl, '0'                 ;converte caractere");
        writeInProgramFile("add eax, ebx                ;soma valor caractere");
        writeInProgramFile("add rsi, 1                  ;incrementa base");
        writeInProgramFile("mov bl, [rsi]               ;carrega caractere");
        writeInProgramFile(format("jmp Rot%d                    ;se dif -, salta", label1));

        // ROtulo 2
        writeInProgramFile(format("Rot%d:", label2));
        writeInProgramFile("pop cx                      ;desempilha sinal");
        writeInProgramFile("cmp cx, 0");
        writeInProgramFile(format("jg Rot%d", label3));
        writeInProgramFile("neg eax                     ;mult. sinal");

        // Rotulo 3
        writeInProgramFile(format("Rot%d:", label3));
    }
    else if (t->getTokenType() == TOKEN_TYPE_REAL)
    {
        int label0 = newLabel();
        int label1 = newLabel();
        int label2 = newLabel();
        int label3 = newLabel();

        long tmpbuff = newTmpByTokenType(TOKEN_TYPE_STRING);
        t->setTokenAddr(tmpbuff);

        // Leitura da String
        writeInProgramFile(format("mov rsi, M + %ld", t->getTokenAddr()));
        writeInProgramFile("rdx, 100h");
        writeInProgramFile("mov, rax, 0");
        writeInProgramFile("mov rdi, 0");
        writeInProgramFile("syscall");
        writeInProgramFile((format("mov byte [M+%ld+rax-1], 0", t->getTokenAddr())));

        writeInProgramFile("mov rax, 0                  ;acumul. parte int.");
        writeInProgramFile("subss xmm0,xmm0             ;acumul. parte frac.");
        writeInProgramFile("mov rbx, 0                  ;caractere");
        writeInProgramFile("mov rcx, 10                 ;base 10");
        writeInProgramFile("cvtsi2ss xmm3,rcx           ;base 10");
        writeInProgramFile("movss xmm2,xmm3             ;potência de 10");
        writeInProgramFile("mov rdx, 1                  ;sinal");
        writeInProgramFile(format("mov rsi, M + %ld", t->getTokenAddr()));

        writeInProgramFile("mov bl,[rsi]                ;carrega caractere");
        writeInProgramFile("cmp bl, '-'                 ;sinal?");
        writeInProgramFile(format("jne Rot%d                    ;se dif -, salta", label0));
        writeInProgramFile("mov rdx, -1                 ;senão, armazena -");
        writeInProgramFile("add rsi, 1                  ;inc. ponteiro string");
        writeInProgramFile("mov bl, [rsi]               ;carrega caractere");

        // Rotulo 0
        writeInProgramFile(format("Rot%d:", label0));
        writeInProgramFile("push rdx                     ;empilha sinal");
        writeInProgramFile("mov rdx, 0                  ;reg. multiplicação");

        // Rotulo 1
        writeInProgramFile(format("Rot%d:", label1));
        writeInProgramFile("cmp bl, 0Ah                     ;verifica fim string");
        writeInProgramFile(format("je Rot%d", label2));
        writeInProgramFile("cmp bl, '.'                     ;senão verifica ponto");
        writeInProgramFile(format("je Rot%d", label3));
        writeInProgramFile("imul ecx                        ;mult. eax por 10");
        writeInProgramFile("sub bl, '0'                     ;converte caractere");
        writeInProgramFile("add eax, ebx                    ;soma valor caractere");
        writeInProgramFile("add rsi, 1                      ;incrementa base");
        writeInProgramFile("mov bl, [rsi]                   ;carrega caractere");
        writeInProgramFile(format("jmp Rot%d", label1));
        
        // Rotulo 3
        writeInProgramFile(format("Rot%d:", label3));
        writeInProgramFile(";calcula parte fracionária em xmm0");
        writeInProgramFile("add rsi, 1                      ;inc. ponteiro string");
        writeInProgramFile("cmp bl, 0Ah                     ;*verifica fim string");
        writeInProgramFile("cmp bl, 0Ah                     ;*verifica fim string");
        writeInProgramFile(format("je Rot%d", label2));
        writeInProgramFile("sub bl, '0'                     ;converte caractere");
        writeInProgramFile("cvtsi2ss xmm1,rbx               ;conv real");
        writeInProgramFile("divss xmm1,xmm2                 ;transf. casa decimal");
        writeInProgramFile("addss xmm0,xmm1                 ;soma acumul.");
        writeInProgramFile("mulss xmm2,xmm3                 ;atualiza potência");
        writeInProgramFile(format("jmp Rot%d", label3));


        // Rotulo 2
        writeInProgramFile(format("Rot%d:", label2));
        writeInProgramFile("cvtsi2ss xmm1,rax               ;conv parte inteira");
        writeInProgramFile("addss xmm0,xmm1                 ;soma parte frac.");
        writeInProgramFile("pop rcx                         ;desempilha sinal");
        writeInProgramFile("cvtsi2ss xmm1,rcx               ;conv sinal");
        writeInProgramFile("mulss xmm0,xmm1                 ;mult. sinal");

    }
    else if (t->getTokenType() == TOKEN_TYPE_BOOLEAN)
    {
    }
}
