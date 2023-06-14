#include <string>
#include <fstream>
#include "../symbols/token.hpp"
#include "../symbols/token-type.hpp"
#include "code-gen.hpp"
#include <memory>
#include <stdexcept>
#include <cstdio>
#include <iostream>

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
/**
 * @brief Cria um temporário a partir do token type
*/
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
//encerra o programa
void CodeGen::end()
{   
    this->programFile <<";fim do programa"<<"\n";
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
    this->programFile << ";Declaração de Variável\n";
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
    else if (t->getTokenType() == TOKEN_TYPE_STRING) /*need fix*/
    {
        this->mem_count += this->string_size;
        this->programFile << "\tresb 100h"
                 << "\n";
    }
}
//comando de declaração de constante (final)
void CodeGen::DeclareConst(Token *t, Token *constant)
{   
    this->programFile << ";Declaração de constante(final)\n";
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
    else if (t->getTokenType() == TOKEN_TYPE_STRING) /*need fix*/
    {
        t->setTokenSize(constant->getLexeme().size());
        this->mem_count += constant->getLexeme().size()+1;
        this->programFile << format("\tdb \"%s\", %d", constant->getLexeme().c_str(),constant->getLexeme().size()) << "\n";
    }
}
// comando de declaração de vetor
void CodeGen::DeclareVet(Token *t, Token *size){
    this->programFile << ";Declaração de vetor\n";
    t->setTokenAddr(this->mem_count);
    if (t->getTokenType() == TOKEN_TYPE_CHAR || t->getTokenType()== TOKEN_TYPE_BOOLEAN)
    {
        this->mem_count += (this->char_size * std::stoi(size->getLexeme()));
        this->programFile << format("\tresb %d",std::stoi(size->getLexeme()))<< "\n";
    }
    else if (t->getTokenType() == TOKEN_TYPE_INTEGER || t->getTokenType() == TOKEN_TYPE_REAL)
    {
        this->mem_count += (this->number_size * std::stoi(size->getLexeme()));
        this->programFile << format("\tresd %d",std::stoi(size->getLexeme()))<< "\n";
    }
}
// guarda valor de uma constante em um temporário
void CodeGen::storeConstOnTmp(Token *t,Token *constant)
{
    this->programFile <<";guardando valor constante "<<constant->getLexeme().c_str()<< " em temporário\n";

    if (constant->getTokenType() == TOKEN_TYPE_REAL)
    {
        t->setTokenAddr(this->mem_count);
        this->mem_count += this->number_size;
        this->startData();
        this->programFile << format("\tdd %s", constant->getLexeme().c_str()) << "\n";
        this->startText();
    }
    else if (constant->getTokenType() == TOKEN_TYPE_STRING)
    {
        t->setTokenAddr(this->mem_count);
        t->setTokenSize(constant->getLexeme().size());
        this->mem_count += constant->getLexeme().size()+1;
        this->startData();
        this->programFile << format("\tdb \"%s\", %d", constant->getLexeme().c_str(),constant->getLexeme().size()) << "\n";
        this->startText();
    }
    else if (constant->getTokenType() == TOKEN_TYPE_CHAR)
    {
        long addr = this->NewTmp(constant);
        t->setTokenAddr(addr);
        this->startText();
        this->programFile << format("\tmov al, \'%s\'", constant->getLexeme().c_str())<<"\n";
        this->programFile << format("\tmov [qword M+%ld], al", addr) << "\n";
    }
    else if (constant->getTokenType() == TOKEN_TYPE_INTEGER)
    {
        long addr = this->NewTmp(constant);
        t->setTokenAddr(addr);
        this->startText();
        this->programFile << format("\tmov eax, %s", constant->getLexeme().c_str())<<"\n";
        this->programFile << format("\tmov [qword M+%ld], eax", addr) << "\n";
    }
    else if(constant->getTokenType() == TOKEN_TYPE_BOOLEAN)
    {
        long addr = this->NewTmp(constant);
        t->setTokenAddr(addr);
        this->startText();
        if(constant->getTokenid()==TOKEN_ID_TRUE){
            this->programFile << "\tmov al, 1"<<"\n";
            this->programFile << format("\tmov [qword M+%ld], al", addr) << "\n";
        }else{
            this->programFile << "\tmov al, 0"<<"\n";
            this->programFile << format("\tmov [qword M+%ld], al", addr) << "\n";
        }
    }
}
// comando de atribuição
void CodeGen::atributionCommand(Token *id, Token *exp)
{
    this->programFile<<";Comando de atribuição\n";
    this->startText();
    if (id->getTokenType() == TOKEN_TYPE_INTEGER)
    {
        this->programFile << format("\tmov eax,[qword M+%ld]", exp->getTokenAddr()) << "\n";
        this->programFile << format("\tmov [qword M+%ld], eax", id->getTokenAddr()) << "\n";
    }
    else if (id->getTokenType() == TOKEN_TYPE_REAL)
    {
        this->programFile << format("\tmovss xmm0,[qword M+%ld]", exp->getTokenAddr()) << "\n";
        this->programFile << format("\tmovss [qword M+%ld], xmm0", id->getTokenAddr()) << "\n";
    }
    else if (id->getTokenType() == TOKEN_TYPE_CHAR)
    {
        if(id->getTokenClass()==TOKEN_CLASS_VETOR){
            id->setTokenSize(exp->getTokeSize());
            int label1 = newLabel();
            int label2 = newLabel();
            this->programFile << format("\tmov rsi, qword M+%ld",id->getTokenAddr())<<"\n";
            this->programFile << format("\tmov rdi, qword M+%ld",exp->getTokenAddr())<<"\n";
            this->programFile << format("Rot%d:",label1)<<"\n";
            this->programFile << "\tmov al, [rdi]\n";
            this->programFile << "\tmov [rsi], al\n";
            this->programFile << "\tcmp al, 0\n";
            this->programFile << format("\tje Rot%d",label2)<<"\n";
            this->programFile <<"\tadd rdi, 1\n";
            this->programFile <<"\tadd rsi, 1\n";
            this->programFile << format("\tjmp Rot%d",label1)<<"\n";
            this->programFile << format("Rot%d:",label2)<<"\n";
        }else{
            this->programFile << format("\tmov al,[qword M+%ld]", exp->getTokenAddr()) << "\n";
            this->programFile << format("\tmov [qword M+%ld], al", id->getTokenAddr()) << "\n";
        }
    }else if(id->getTokenType() == TOKEN_TYPE_STRING){
        id->setTokenSize(exp->getTokeSize());
        int label1 = newLabel();
        int label2 = newLabel();
        this->programFile << format("\tmov rsi, qword M+%ld",id->getTokenAddr())<<"\n";
        this->programFile << format("\tmov rdi, qword M+%ld",exp->getTokenAddr())<<"\n";
        this->programFile << format("Rot%d:",label1)<<"\n";
        this->programFile << "\tmov al, [rdi]\n";
        this->programFile << "\tmov [rsi], al\n";
        this->programFile << "\tcmp al, 0\n";
        this->programFile << format("\tje Rot%d",label2)<<"\n";
        this->programFile <<"\tadd rdi, 1\n";
        this->programFile <<"\tadd rsi, 1\n";
        this->programFile << format("\tjmp Rot%d",label1)<<"\n";
        this->programFile << format("Rot%d:",label2)<<"\n";
    }else if (id->getTokenType() == TOKEN_TYPE_BOOLEAN)
    {
        if (exp->getTokenType() == TOKEN_TYPE_BOOLEAN)
        {
            this->programFile << format("\tmov al,[qword M+%ld]", exp->getTokenAddr()) << "\n";
            this->programFile << format("\tmov [qword M+%ld], al", id->getTokenAddr()) << "\n";
        }
        else
        {
            if (exp->getLexeme() == "True")
            {
            this->programFile << "\tmov al, 1" << "\n";
            this->programFile << format("\tmov [qword M+%ld], al", id->getTokenAddr()) << "\n";
            }
            else
            {
            this->programFile << "\tmov al, 0" << "\n";
            this->programFile << format("\tmov [qword M+%ld], al", id->getTokenAddr()) << "\n";
            }
        }
    }
}
//Comando de atribuição para vetor
void CodeGen::vetAtribution(Token *id, Token *pos, Token *exp){
    this->programFile<<";Comando de atribuição de vetor\n";
    if(id->getTokenType() == TOKEN_TYPE_INTEGER || id->getTokenType() == TOKEN_TYPE_REAL){
        writeInProgramFile(format("\tmov rcx, qword M+%ld",id->getTokenAddr()));
        writeInProgramFile(format("\tmov ebx, [qword M+%ld]",pos->getTokenAddr()));
        writeInProgramFile("\tadd ebx, ebx");
        writeInProgramFile("\tadd ebx, ebx");
        
        writeInProgramFile("\tadd ecx, ebx");
        writeInProgramFile(format("\tmov eax, [qword M+%ld]",exp->getTokenAddr()));
        writeInProgramFile("\tmov [rcx], eax");
    }else if(id->getTokenType()==TOKEN_TYPE_BOOLEAN || id->getTokenType()==TOKEN_TYPE_CHAR){
        writeInProgramFile(format("\tmov rcx, qword M+%ld",id->getTokenAddr()));
        writeInProgramFile(format("\tmov ebx, [qword M+%ld]",pos->getTokenAddr()));

        writeInProgramFile("\tadd ecx, ebx");
        writeInProgramFile(format("\tmov al, [qword M+%ld]",exp->getTokenAddr()));
        writeInProgramFile("\tmov [rcx], al");
    }
}
// inverte valor da expressão
void CodeGen::invertExpression(Token *exp)
{
    long tmpAddr = this->NewTmp(exp);
    if (exp->getTokenType() == TOKEN_TYPE_INTEGER)
    {
        this->programFile << format("\tmov ebx, [qword M+%ld]", exp->getTokenAddr()) << "\n";
        this->programFile << "\tmov eax, -1"<<"\n";
        this->programFile << "\timul ebx"<< "\n";
        exp->setTokenAddr(tmpAddr);
        this->programFile << format("\tmov [qword M+%ld], eax", tmpAddr) << "\n";
    }
    else if (exp->getTokenType() == TOKEN_TYPE_REAL)
    {
        this->programFile << format("\tmovss xmm0, [qword M+%ld]", exp->getTokenAddr()) << "\n";
        this->programFile << "\tmov rax, -1"<< "\n";
        this->programFile << "\tcvtsi2ss xmm1, rax"<< "\n";
        this->programFile << "\tmulss xmm0, xmm1"<< "\n";
        exp->setTokenAddr(tmpAddr);
        this->programFile << format("\tmovss [qword M+%ld], xmm0", tmpAddr) << "\n";
    }
}
// Operação de soma
void CodeGen::sumOperation(Token *op1, Token *op2)
{
    if (op1->getTokenType() == op2->getTokenType())
    {
        if (op1->getTokenType() == TOKEN_TYPE_INTEGER)
        {
            this->programFile << format("\tmov eax, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("\tmov ebx, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "\tadd eax,ebx"<<"\n";
            long tmpAddr = this->NewTmp(op1);
            op1->setTokenAddr(tmpAddr);
            this->programFile << format("\tmov [qword M+%ld], eax", tmpAddr) << "\n";
        }
        else
        {
            this->programFile << format("\tmovss xmm0, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("\tmovss xmm1, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "\taddss xmm0,xmm1"<<"\n";
            long tmpAddr = this->NewTmp(op1);
            op1->setTokenAddr(tmpAddr);
            this->programFile << format("\tmovss [qword M+%ld], xmm0", tmpAddr) << "\n";
        }
    }
    else
    {
        if (op1->getTokenType() == TOKEN_TYPE_REAL)
        {
            this->programFile << format("\tmovss xmm0, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("\tmov eax, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "\tcdqe"<<"\n";
            this->programFile << "\tcvtsi2ss xmm1, rax"<< "\n";
        }
        else
        {
            this->programFile << format("\tmov eax, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << "\tcdqe"<<"\n";
            this->programFile << format("\tmovss xmm1, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "\tcvtsi2ss xmm0, rax"<< "\n";
        }
        this->programFile << "\taddss xmm0, xmm1"<< "\n";
        long tmpAddr = this->NewTmp(op1);
        op1->setTokenAddr(tmpAddr);
        this->programFile << format("\tmovss [qword M+%ld], xmm0", tmpAddr) << "\n";
    }
}
// Operação de subtração
void CodeGen::subOperation(Token *op1, Token *op2)
{
    if (op1->getTokenType() == op2->getTokenType())
    {
        if (op1->getTokenType() == TOKEN_TYPE_INTEGER)
        {
            this->programFile << format("\tmov eax, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("\tmov ebx, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "\tsub eax,ebx"<<"\n";
            long tmpAddr = this->NewTmp(op1);
            op1->setTokenAddr(tmpAddr);
            this->programFile << format("\tmov [qword M+%ld], eax", tmpAddr) << "\n";
        }
        else
        {
            this->programFile << format("\tmovss xmm0, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("\tmovss xmm1, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "\tsubss xmm0,xmm1"<<"\n";
            long tmpAddr = this->NewTmp(op1);
            op1->setTokenAddr(tmpAddr);
            this->programFile << format("\tmovss [qword M+%ld], xmm0", tmpAddr) << "\n";
        }
    }
    else
    {
        if (op1->getTokenType() == TOKEN_TYPE_REAL)
        {
            this->programFile << format("\tmovss xmm0, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("\tmov eax, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "\tcdqe"<<"\n";
            this->programFile << "\tcvtsi2ss xmm1, rax"<< "\n";
        }
        else
        {
            this->programFile << format("\tmov eax, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << "\tcdqe"<<"\n";
            this->programFile << format("\tmovss xmm1, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "\tcvtsi2ss xmm0, rax" << "\n";
        }
        this->programFile << "\tsubss xmm0, xmm1"<< "\n";
        long tmpAddr = this->NewTmp(op1);
        op1->setTokenAddr(tmpAddr);
        this->programFile << format("\tmovss [qword M+%ld], xmm0", tmpAddr) << "\n";
    }
}
// Operação de or
void CodeGen::orOperation(Token *op1,Token *op2){
    this->programFile << format("\tmov eax, [qword M+%ld]", op1->getTokenAddr())<<"\n";
    this->programFile << format("\tmov ebx, [qword M+%ld]", op1->getTokenAddr())<<"\n";
    this->programFile << format("\tmov ecx, [qword M+%ld]", op2->getTokenAddr())<<"\n";
    this->programFile << "\timul ecx"<<"\n";
    this->programFile << "\tadd ebx, ecx"<<"\n";
    this->programFile << "\tsub ebx, eax"<<"\n";

    long tmpAddr = this->tmp_count;
    this->tmp_count += this->char_size;
    op1->setTokenAddr(tmpAddr);
    this->programFile << format("\tmov [qword M+%ld],ebx",tmpAddr)<<"\n";
} 
// Operação de multiplicação
void CodeGen::multiplyOperation(Token *op1, Token *op2)
{
    if (op1->getTokenType() == op2->getTokenType())
    {
        if (op1->getTokenType() == TOKEN_TYPE_INTEGER)
        {
            this->programFile << format("\tmov eax, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("\tmov ebx, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "\timul ebx"<<"\n";
            long tmpAddr = this->NewTmp(op1);
            op1->setTokenAddr(tmpAddr);
            this->programFile << format("\tmov [qword M+%ld], eax", tmpAddr) << "\n";
        }
        else
        {
            this->programFile << format("\tmovss xmm0, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("\tmovss xmm1, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "\tmulss xmm0,xmm1"<<"\n";
            long tmpAddr = this->NewTmp(op1);
            op1->setTokenAddr(tmpAddr);
            this->programFile << format("\tmovss [qword M+%ld], xmm0", tmpAddr) << "\n";
        }
    }
    else
    {
        if (op1->getTokenType() == TOKEN_TYPE_REAL)
        {
            this->programFile << format("\tmovss xmm0, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("\tmov eax, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "\tcdqe"<<"\n";
            this->programFile << "\tcvtsi2ss xmm1, rax"<< "\n";
        }
        else
        {
            this->programFile << format("\tmov eax, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << "\tcdqe"<<"\n";
            this->programFile << format("\tmovss xmm1, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "\tcvtsi2ss xmm0, rax"<< "\n";
        }
        this->programFile << "\tmulss xmm0, xmm1"<< "\n";
        long tmpAddr = this->NewTmp(op1);
        op1->setTokenAddr(tmpAddr);
        this->programFile << format("\tmovss [qword M+%ld], xmm0", tmpAddr) << "\n";
    }
}
// Operação de divisão
void CodeGen::divideOperation(Token *op1, Token *op2)
{
    if (op1->getTokenType() == op2->getTokenType())
    {
        if (op1->getTokenType() == TOKEN_TYPE_INTEGER)
        {
            this->programFile << format("\tmov eax, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("\tmov ebx, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "\tidiv ebx"<<"\n";
            long tmpAddr = this->NewTmp(op1);
            op1->setTokenAddr(tmpAddr);
            this->programFile << format("\tmov [qword M+%ld], eax", tmpAddr) << "\n";
        }
        else
        {
            this->programFile << format("\tmovss xmm0, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("\tmovss xmm1, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "\tdivss xmm0,xmm1"<<"\n";
            long tmpAddr = this->NewTmp(op1);
            op1->setTokenAddr(tmpAddr);
            this->programFile << format("\tmovss [qword M+%ld], xmm0", tmpAddr) << "\n";
        }
    }
    else
    {
        if (op1->getTokenType() == TOKEN_TYPE_REAL)
        {
            this->programFile << format("\tmovss xmm0, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << format("\tmov eax, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "\tcdqe"<<"\n";
            this->programFile << "\tcvtsi2ss xmm1, rax"<< "\n";
        }
        else
        {
            this->programFile << format("\tmov eax, [qword M+%ld]", op1->getTokenAddr()) << "\n";
            this->programFile << "\tcdqe"<<"\n";
            this->programFile << format("\tmovss xmm1, [qword M+%ld]", op2->getTokenAddr()) << "\n";
            this->programFile << "\tcvtsi2ss xmm0, rax" << "\n";
        }
        this->programFile << "\tdivss xmm0, xmm1" << "\n";
        long tmpAddr = this->NewTmp(op1);
        op1->setTokenAddr(tmpAddr);
        this->programFile << format("\tmovss [qword M+%ld], xmm0", tmpAddr) << "\n";
    }
}
//Operação de mod
void CodeGen::modOperation(Token *op1,Token *op2){
    this->programFile << format("\tmov eax, [qword M+%ld]", op1->getTokenAddr())<<"\n";
    this->programFile << "\tcdq"<<"\n";
    this->programFile << format("\tmov ebx, [qword M+%ld]", op2->getTokenAddr())<<"\n";
    this->programFile << "\tidiv ebx"<<"\n";
    long tmpAddr = NewTmp(op1);
    op1->setTokenAddr(tmpAddr);
    this->programFile << format("\tmov [qword M+%ld], edx", tmpAddr)<<"\n";

}
//Operação de and
void CodeGen::andOperation(Token *op1,Token *op2){
    long tmpAddr = newTmpByTokenType(TOKEN_TYPE_BOOLEAN);
    
    writeInProgramFile(format("\tmov eax, [qword M + %d]", op2->getTokenAddr()));
    writeInProgramFile(format("\tmov ebx, [qword M + %ld]", op1->getTokenAddr()));
    writeInProgramFile("\timul ebx");
    op2->setTokenAddr(tmpAddr);
    writeInProgramFile(format("\tmov [qword M + %ld], eax", op2->getTokenAddr()));
}
//Define qual a operação a ser escrita
void CodeGen::defineOperation(Token *op1,Token *op2, Token *operation){
    if(operation->getTokenid()==TOKEN_ID_ADDITION){
        this->sumOperation(op1,op2);
    }else if(operation->getTokenid()==TOKEN_ID_SUBTRACTION){
        this->subOperation(op1,op2);
    }else if(operation->getTokenid()==TOKEN_ID_OR){
        this->orOperation(op1,op2);
    }else if(operation->getTokenid()==TOKEN_ID_MULTIPLICATION){
        this->multiplyOperation(op1,op2);
    }else if(operation->getTokenid()==TOKEN_ID_DIVISION){
        this->divideOperation(op1,op2);
    }else if(operation->getTokenid()==TOKEN_ID_MODULO){
        this->modOperation(op1,op2);
    }else if(operation->getTokenid()==TOKEN_ID_AND){
        this->andOperation(op1,op2);
    }
}
// negação da expressão
void CodeGen::negExpression(Token *exp)
{
    long tmpAddr = this->NewTmp(exp);
    this->programFile << format("\tmov al, [qword M+%ld]", exp->getTokenAddr()) << "\n";
    this->programFile << "\tneg al"<< "\n";
    this->programFile << "\tadd al,1"<< "\n";
    exp->setTokenAddr(tmpAddr);
    this->programFile << format("\tmov [qword M+%ld], al", tmpAddr) << "\n";
}
// converte valor inteiro para real
void CodeGen::cvtToReal(Token *t)
{
    this->programFile <<";conversão de inteiro para real"<<"\n";
    this->programFile << format("\tmov eax, [qword M+%ld]", t->getTokenAddr()) << "\n";
    this->programFile << "\tcdqe"<<"\n";
    long tmpAddr = this->NewTmp(t);
    t->setTokenAddr(tmpAddr);
    this->programFile << "\tcvtsi2ss xmm0, rax"<< "\n";
    this->programFile << format("\tmovss [qword M+%d], xmm0", tmpAddr)<<"\n";
}
// converte valor real para inteiro
void CodeGen::cvtToInt(Token *t)
{
    this->programFile << format("\tmovss xmm0, [qword M+%ld]", t->getTokenAddr()) << "\n";
    long tmpAddr = this->NewTmp(t);
    t->setTokenAddr(tmpAddr);
    this->programFile << "\tcvtss2si rax, xmm0"<< "\n";
    this->programFile << format("\tmov [qword M+%d], rax", tmpAddr);
}
/**
 * @brief Escreve no terminal
*/
void CodeGen::write(Token *t)
{
    this->programFile << ";Escrevendo na saida do terminal"<<"\n";
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
    }else if (t->getTokenType() == TOKEN_TYPE_CHAR){
        if(t->getTokeSize()>0){
            bufferAddr = t->getTokenAddr();
            this->programFile << format("\tmov rsi, M+%ld", bufferAddr)<<"\n";
            this->programFile << format("\tmov rdx, %d",t->getTokeSize())<<"\n";
            this->programFile <<"\tmov rax, 1"<<"\n";
            this->programFile << "\tmov rdi, 1"<<"\n";
            this->programFile << "\tsyscall"<<"\n";
        }else{
            bufferAddr = t->getTokenAddr();
            this->programFile << format("mov rsi, M+%ld", bufferAddr)<<"\n";
            this->programFile << "\tmov rdx, 1"<<"\n";
            this->programFile << "\tmov rax, 1"<<"\n";
            this->programFile << "\tmov rdi, 1"<<"\n";
            this->programFile << "\tsyscall"<<"\n";
        }
    }
    else if(t->getTokenType() == TOKEN_TYPE_BOOLEAN)
    {
        bufferAddr = t->getTokenAddr();
        this->programFile << format("mov rsi, M+%ld", bufferAddr)<<"\n";
        this->programFile << "\tmov rdx, 1"<<"\n";
        this->programFile << "\tmov rax, 1"<<"\n";
        this->programFile << "\tmov rdi, 1"<<"\n";
        this->programFile << "\tsyscall"<<"\n";
    }else if (t->getTokenType() == TOKEN_TYPE_REAL)
    {
        bufferAddr = this->tmp_count;
        this->tmp_count += this->string_size;
        label1 = newLabel();
        label2 = newLabel();
        label3 = newLabel();
        label4 = newLabel();
        label5 = newLabel();

        this->programFile << format("\tmovss xmm0, [qword M+%ld]", t->getTokenAddr())<<"\n";
        this->programFile << format("\tmov rsi, M+%ld", bufferAddr)<<"\n";
        this->programFile << "\tmov rcx, 0"<<"\n";
        this->programFile << "\tmov rdi, 6"<<"\n";
        this->programFile << "\tmov rbx, 10 "<<"\n";
        this->programFile << "\tcvtsi2ss xmm2, rbx"<<"\n";
        this->programFile << "\tsubss xmm1, xmm1" <<"\n";
        this->programFile << "\tcomiss xmm0, xmm1"<<"\n";
        this->programFile << format("\tjae Rot%d",label1)<<"\n";
        this->programFile << "\tmov dl, '-'"<<"\n";
        this->programFile << "\tmov [rsi], dl"<<"\n";
        this->programFile <<"\tmov rdx, -1"<<"\n";
        this->programFile <<"\tcvtsi2ss xmm1, rdx"<<"\n";
        this->programFile <<"\tmulss xmm0, xmm1"<<"\n";
        this->programFile <<"\tadd rsi, 1"<<"\n";
        this->programFile <<format("\tRot%d:", label1)<<"\n";
        this->programFile <<"\troundss xmm1, xmm0, 0b0011"<<"\n";
        this->programFile <<"\tsubss xmm0, xmm1"<<"\n";
        this->programFile <<"\tcvtss2si rax, xmm1"<<"\n";
        this->programFile <<format("\tRot%d:", label2)<<"\n";
        this->programFile <<"\tadd rcx, 1"<<"\n";
        this->programFile <<"\tcdq"<<"\n";
        this->programFile <<"\tidiv ebx"<<"\n";
        this->programFile <<"\tpush dx"<<"\n";
        this->programFile <<"\tcmp eax, 0"<<"\n";
        this->programFile <<format("\tjne Rot%d", label2)<<"\n";
        this->programFile <<"\tsub rdi, rcx"<<"\n";
        this->programFile <<format("\tRot%d:", label3)<<"\n";
        this->programFile <<"\tpop dx"<<"\n";
        this->programFile <<"\tadd dl, '0'"<<"\n";
        this->programFile <<"\tmov [rsi], dl"<<"\n";
        this->programFile <<"\tadd rsi, 1"<<"\n";
        this->programFile <<"\tsub rcx, 1"<<"\n";
        this->programFile <<"\tcmp rcx, 0"<<"\n";
        this->programFile <<format("\tjne Rot%d", label3)<<"\n";
        this->programFile <<"\tmov dl, '.'"<<"\n";
        this->programFile <<"\tmov [rsi], dl"<<"\n";
        this->programFile <<"\tadd rsi, 1"<<"\n";
        this->programFile <<format("\tRot%d:", label4)<<"\n";
        this->programFile <<"\tcmp rdi, 0"<<"\n";
        this->programFile <<format("\tjle Rot%d", label5)<<"\n";
        this->programFile <<"\tmulss xmm0,xmm2"<<"\n";
        this->programFile <<"\troundss xmm1,xmm0,0b0011"<<"\n";
        this->programFile <<"\tsubss xmm0,xmm1"<<"\n";
        this->programFile <<"\tcvtss2si rdx, xmm1"<<"\n";
        this->programFile <<"\tadd dl, '0'"<<"\n";
        this->programFile <<"\tmov [rsi], dl"<<"\n";
        this->programFile <<"\tadd rsi, 1"<<"\n";
        this->programFile <<"\tsub rdi, 1"<<"\n";
        this->programFile <<format("\tjmp Rot%d", label4)<<"\n";
        this->programFile <<format("\tRot%d:", label5)<<"\n";
        this->programFile <<"\tmov dl, 0"<<"\n";
        this->programFile <<"\tmov [rsi], dl"<<"\n";
        this->programFile <<"\tmov rdx, rsi"<<"\n";
        this->programFile <<format("\tmov rbx, M+%ld", bufferAddr)<<"\n";
        this->programFile <<"\tsub rdx, rbx"<<"\n";
        this->programFile <<format("\tmov rsi, M+%ld", bufferAddr)<<"\n";
        this->programFile <<"\tmov rax, 1"<<"\n";
        this->programFile <<"\tmov rdi, 1"<<"\n";
        this->programFile <<"\tsyscall"<<"\n";
    }
    else if(t->getTokenType() == TOKEN_TYPE_INTEGER)
    {
        bufferAddr = this->tmp_count;
        this->tmp_count += this->string_size;
        label1 = newLabel();
        label2 = newLabel();
        label3 = newLabel();

        this->programFile <<format("\tmov eax, [qword M+%ld]", t->getTokenAddr())<<"\n";
        this->programFile <<format("\tmov rsi, M+%ld", bufferAddr)<<"\n";
        this->programFile <<"\tmov rcx, 0"<<"\n";
        this->programFile <<"\tmov rdi, 0"<<"\n";
        this->programFile <<"\tcmp eax, 0"<<"\n";
        this->programFile <<format("\tjge Rot%d",label1)<<"\n";
        this->programFile <<"\tmov bl, '-' "<<"\n";
        this->programFile <<"\tmov [rsi], bl"<<"\n";
        this->programFile <<"\tadd rsi, 1"<<"\n";
        this->programFile <<"\tadd rdi, 1"<<"\n";
        this->programFile <<"\tneg eax"<<"\n";
        this->programFile <<format("Rot%d:",label1)<<"\n";
        this->programFile <<"\tmov ebx, 10"<<"\n";
        this->programFile <<format("Rot%d:",label2)<<"\n";
        this->programFile <<"\tadd rcx, 1"<<"\n";
        this->programFile <<"\tcdq"<<"\n";
        this->programFile <<"\tidiv ebx"<<"\n";
        this->programFile <<"\tpush dx"<<"\n";
        this->programFile <<"\tcmp eax, 0"<<"\n";
        this->programFile <<format("\tjne Rot%d", label2)<<"\n";
        this->programFile <<"\tadd rdi,rcx"<<"\n";
        this->programFile <<format("Rot%d:",label3)<<"\n";
        this->programFile <<"\tpop dx"<<"\n";
        this->programFile <<"\tadd dl, '0'"<<"\n";
        this->programFile <<"\tmov [rsi], dl"<<"\n";
        this->programFile <<"\tadd rsi, 1"<<"\n";
        this->programFile <<"\tsub rcx, 1"<<"\n";
        this->programFile <<"\tcmp rcx, 0"<<"\n";
        this->programFile <<format("\tjne Rot%d", label3)<<"\n";
        this->programFile <<format("\tmov rsi, M+%ld",bufferAddr)<<"\n";
        this->programFile <<"\tmov rax, 1"<<"\n";
        this->programFile <<"\tmov rdi, 1"<<"\n";
        this->programFile <<"\tsyscall"<<"\n";
    }
}
/**
 * @brief Escreve uma quebra de linha no terminal
*/
void CodeGen::writeln(){
    long line_break = newTmpByTokenType(TOKEN_TYPE_CHAR);
    this->programFile << "\tmov al, 0ah"<<"\n";
    this->programFile << format("\tmov [qword M+%ld], al", line_break)<<"\n";
    this->programFile << format("\tmov rsi, M+%ld", line_break)<<"\n";
    this->programFile << "\tmov rdx, 1"<<"\n";
    this->programFile << "\tmov rax, 1"<<"\n";
    this->programFile << "\tmov rdi, 1"<<"\n";
    this->programFile << "\tsyscall"<<"\n";
}
/**
 * @brief uma declaração condicional 

*/
void CodeGen::initCondition(Token *t, int startLabel) {
    writeInProgramFile(format("\tmov al, [qword M + %ld]", t->getTokenAddr()));
    writeInProgramFile("\tcmp al, 0");
    writeInProgramFile(format("\tje Rot%d", startLabel));
}
/**
 * @brief Finaliza um bloco de código (bloco condicional)
 */
void CodeGen::finalizeBlock( int startLabel, int endLabel) {
    writeInProgramFile(format("\tjmp Rot%d", startLabel));
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
    writeInProgramFile(format("\tjmp Rot%d",rot));
}
/**
 * @brief Compara a expressão do comando de for
 * pula para fora do loop em caso da expressão ser falsa
 */
void CodeGen::compareForExpression(Token *exp,int rot){
    writeInProgramFile(format("\tmov al, [qword M+%ld]",exp->getTokenAddr()));
    writeInProgramFile("\tcmp al, 1");
    writeInProgramFile(format("\tjne Rot%d",rot));
}
/**
 * @brief Move o elemento de uma posição de um vetor para um temporário
*/
void CodeGen::vectorAccess(Token *id, Token *exp, Token *t){
    this->programFile << ";acesso ao vetor"<<"\n";
    long tmpAddr = NewTmp(t);
    writeInProgramFile(format("\tmov rcx, qword M+%ld",id->getTokenAddr()));
    writeInProgramFile(format("\tmov ebx, [qword M+%ld]",exp->getTokenAddr()));
    if(id->getTokenType() == TOKEN_TYPE_INTEGER || id->getTokenType() == TOKEN_TYPE_REAL){
        writeInProgramFile("\tadd ebx, ebx");
        writeInProgramFile("\tadd ebx, ebx");

        writeInProgramFile("\tadd ecx, ebx");
        writeInProgramFile("\tmov ecx, [rcx]");
        t->setTokenAddr(tmpAddr);
        writeInProgramFile(format("\tmov [qword M+%ld], ecx",tmpAddr));
    }else if(id->getTokenType() == TOKEN_TYPE_CHAR||id->getTokenType() == TOKEN_TYPE_BOOLEAN){
        writeInProgramFile("\tmov eax, 100h");
        writeInProgramFile("\timul ebx");
        writeInProgramFile("\tmov ebx,eax ");

        writeInProgramFile("\tadd ecx, ebx");
        writeInProgramFile("\tmov cl, [rcx]");
        t->setTokenAddr(tmpAddr);
        writeInProgramFile(format("\tmov [qword M+%ld], cl",tmpAddr));
    }
}
/**
 * @brief Operadores relacionais
*/
void CodeGen::RelacionalOperator(Token *op1, Token *op2, TokenID op){
    int label1=this->newLabel();
    int label2=this->newLabel();

    if(op1->getTokenType() == TOKEN_TYPE_CHAR || op1->getTokenType() == TOKEN_TYPE_BOOLEAN){
        writeInProgramFile(format("\tmov al, [qword M+%ld]", op1->getTokenAddr()));
        writeInProgramFile(format("\tmov bl, [qword M+%ld]", op2->getTokenAddr()));
        writeInProgramFile("\tcmp al,bl");
        if(op==TOKEN_ID_EQUALS){
            writeInProgramFile(format("\tje Rot%d",label1));
        }else if(op==TOKEN_ID_LESS_THAN){
            writeInProgramFile(format("\tjl Rot%d",label1));
        }else if(op==TOKEN_ID_LESS_EQUAL_TO){
            writeInProgramFile(format("\tjle Rot%d",label1));
        }else if(op==TOKEN_ID_GREATER_THEN){
            writeInProgramFile(format("\tjg Rot%d",label1));
        }else if(op==TOKEN_ID_GREATER_EQUAL_TO){
            writeInProgramFile(format("\tjge Rot%d",label1));
        }else if(op==TOKEN_ID_DIFFERENT){
            writeInProgramFile(format("\tjne Rot%d",label1));
        }
    }else if(op1->getTokenType() == TOKEN_TYPE_INTEGER){
        writeInProgramFile(format("\tmov eax, [qword M+%ld]", op1->getTokenAddr()));
        writeInProgramFile(format("\tmov ebx, [qword M+%ld]", op2->getTokenAddr()));
        writeInProgramFile("\tcmp eax, ebx");

        if(op==TOKEN_ID_EQUALS){
            writeInProgramFile(format("\tje Rot%d",label1));
        }else if(op==TOKEN_ID_LESS_THAN){
            writeInProgramFile(format("\tjl Rot%d",label1));
        }else if(op==TOKEN_ID_LESS_EQUAL_TO){
            writeInProgramFile(format("\tjle Rot%d",label1));
        }else if(op==TOKEN_ID_GREATER_THEN){
            writeInProgramFile(format("\tjg Rot%d",label1));
        }else if(op==TOKEN_ID_GREATER_EQUAL_TO){
            writeInProgramFile(format("\tjge Rot%d",label1));
        }else if(op==TOKEN_ID_DIFFERENT){
            writeInProgramFile(format("\tjne Rot%d",label1));
        }
    }else if(op1->getTokenType() == TOKEN_TYPE_REAL){
        writeInProgramFile(format("\tmovss xmm0, [qword M+%ld]", op1->getTokenAddr()));
        writeInProgramFile(format("\tmovss xmm1, [qword M+%ld]", op2->getTokenAddr()));
        writeInProgramFile("\tcomiss xmm0, xmm1");

        if(op==TOKEN_ID_EQUALS){
            writeInProgramFile(format("\tje Rot%d",label1));
        }else if(op==TOKEN_ID_LESS_THAN){
            writeInProgramFile(format("\tjb Rot%d",label1));
        }else if(op==TOKEN_ID_LESS_EQUAL_TO){
            writeInProgramFile(format("\tjbe Rot%d",label1));
        }else if(op==TOKEN_ID_GREATER_THEN){
            writeInProgramFile(format("\tja Rot%d",label1));
        }else if(op==TOKEN_ID_GREATER_EQUAL_TO){
            writeInProgramFile(format("\tjae Rot%d",label1));
        }else if(op==TOKEN_ID_DIFFERENT){
            writeInProgramFile(format("\tjne Rot%d",label1));
        }
    }

    long tmpAddr = this->newTmpByTokenType(TOKEN_TYPE_BOOLEAN);
    writeInProgramFile(format("\tmov cl, 0",tmpAddr));
    writeInProgramFile(format("\tmov [qword M+%ld], cl",tmpAddr));
    writeInProgramFile(format("\tjmp Rot%d",label2));
    writeInProgramFile(format("Rot%d:",label1));
    writeInProgramFile(format("\tmov cl, 1",tmpAddr));
    writeInProgramFile(format("\tmov [qword M+%ld], cl",tmpAddr));
    writeInProgramFile(format("Rot%d:",label2));

    op1->setTokenAddr(tmpAddr);
    op1->setTokenType(TOKEN_TYPE_BOOLEAN);
}
/**
 * @brief Montagem de codigo para leitura do teclado
 */
void CodeGen::readln(Token *t){
    writeInProgramFile(";Leitura do teclado");
    if (t->getTokenType() == TOKEN_TYPE_STRING)
    {
        long tmpbuff = newTmpByTokenType(TOKEN_TYPE_STRING);

        writeInProgramFile(format("\tmov rsi, M + %ld", tmpbuff));
        writeInProgramFile("\tmov rdx, 100h");
        writeInProgramFile("\tmov rax, 0");
        writeInProgramFile("\tmov rdi, 0");
        writeInProgramFile("\tsyscall");

        t->setTokenSize(256);
        int label1 = newLabel();
        int label2 = newLabel();
        this->programFile << format("\tmov rsi, qword M+%ld",t->getTokenAddr())<<"\n";
        this->programFile << format("\tmov rdi, qword M+%ld",tmpbuff)<<"\n";
        this->programFile << format("Rot%d:",label1)<<"\n";
        this->programFile << "\tmov al, [rdi]\n";
        this->programFile << "\tmov [rsi], al\n";
        this->programFile << "\tcmp al, 0\n";
        this->programFile << format("\tje Rot%d",label2)<<"\n";
        this->programFile <<"\tadd rdi, 1\n";
        this->programFile <<"\tadd rsi, 1\n";
        this->programFile << format("\tjmp Rot%d",label1)<<"\n";
        this->programFile << format("Rot%d:",label2)<<"\n";
    }
    else if (t->getTokenType() == TOKEN_TYPE_CHAR)
    {
        long tmpbuff = newTmpByTokenType(TOKEN_TYPE_CHAR);
        
        writeInProgramFile(format("\tmov rsi, M + %ld", tmpbuff));
        writeInProgramFile("\tmov rdx, 1");
        writeInProgramFile("\tmov rax, 0");
        writeInProgramFile("\tmov rdi, 0");
        writeInProgramFile("\tsyscall");

        writeInProgramFile(format("\tmov al,[qword M+%ld]",tmpbuff));
        writeInProgramFile(format("\tmov [qword M+%ld], al", t->getTokenAddr()));
    }
    else if (t->getTokenType() == TOKEN_TYPE_INTEGER)
    {
        int label0 = newLabel();
        int label1 = newLabel();
        int label2 = newLabel();
        int label3 = newLabel();

        long tmpbuff = newTmpByTokenType(TOKEN_TYPE_STRING);

        // Leitura da String
        writeInProgramFile(format("\tmov rsi, M+%ld", tmpbuff));
        writeInProgramFile("\tmov rdx, 100h");
        writeInProgramFile("\tmov rax, 0");
        writeInProgramFile("\tmov rdi, 0");
        writeInProgramFile("\tsyscall");

        writeInProgramFile("\tmov eax, 0");
        writeInProgramFile("\tmov ebx, 0");
        writeInProgramFile("\tmov ecx, 10");
        writeInProgramFile("\tmov dx, 1");

        writeInProgramFile(format("\tmov rsi, M+%ld", tmpbuff));
        writeInProgramFile("\tmov bl,[rsi]");
        writeInProgramFile("\tcmp bl, '-'");
        writeInProgramFile(format("\tjne Rot%d", label0));
        writeInProgramFile("\tmov dx, -1");
        writeInProgramFile("\tadd rsi, 1");
        writeInProgramFile("\tmov bl, [rsi]");

        // Rotulo 0
        writeInProgramFile(format("Rot%d:", label0));
        writeInProgramFile("\tpush dx");
        writeInProgramFile("\tmov edx, 0");

        // Rotulo 1
        writeInProgramFile(format("Rot%d:", label1));
        writeInProgramFile("\tcmp bl, 0Ah");
        writeInProgramFile(format("\tje Rot%d", label2));
        writeInProgramFile("\timul ecx");
        writeInProgramFile("\tsub bl, '0'");
        writeInProgramFile("\tadd eax, ebx");
        writeInProgramFile("\tadd rsi, 1");
        writeInProgramFile("\tmov bl, [rsi]");
        writeInProgramFile(format("\tjmp Rot%d", label1));

        // ROtulo 2
        writeInProgramFile(format("Rot%d:", label2));
        writeInProgramFile("\tpop cx");
        writeInProgramFile("\tcmp cx, 0");
        writeInProgramFile(format("\tjg Rot%d", label3));
        writeInProgramFile("\tneg eax");

        // Rotulo 3
        writeInProgramFile(format("\tRot%d:", label3));

        writeInProgramFile(format("\tmov [qword M+%ld], eax",t->getTokenAddr()));
    }
    else if (t->getTokenType() == TOKEN_TYPE_REAL)
    {
        int label0 = newLabel();
        int label1 = newLabel();
        int label2 = newLabel();
        int label3 = newLabel();

        long tmpbuff = newTmpByTokenType(TOKEN_TYPE_STRING);

        // Leitura da String
        writeInProgramFile(format("\tmov rsi, M+%ld", tmpbuff));
        writeInProgramFile("\tmov rdx, 100h");
        writeInProgramFile("\tmov rax, 0");
        writeInProgramFile("\tmov rdi, 0");
        writeInProgramFile("\tsyscall");

        writeInProgramFile("\tmov rax, 0");
        writeInProgramFile("\tsubss xmm0,xmm0");
        writeInProgramFile("\tmov rbx, 0");
        writeInProgramFile("\tmov rcx, 10");
        writeInProgramFile("\tcvtsi2ss xmm3,rcx");
        writeInProgramFile("\tmovss xmm2,xmm3");
        writeInProgramFile("\tmov rdx, 1");
        writeInProgramFile(format("\tmov rsi, M+%ld", tmpbuff));

        writeInProgramFile("\tmov bl,[rsi]");
        writeInProgramFile("\tcmp bl, '-'");
        writeInProgramFile(format("\tjne Rot%d", label0));
        writeInProgramFile("\tmov rdx, -1");
        writeInProgramFile("\tadd rsi, 1");
        writeInProgramFile("\tmov bl, [rsi]");

        // Rotulo 0
        writeInProgramFile(format("Rot%d:", label0));
        writeInProgramFile("\tpush rdx");
        writeInProgramFile("\tmov rdx, 0");

        // Rotulo 1
        writeInProgramFile(format("Rot%d:", label1));
        writeInProgramFile("\tcmp bl, 0Ah");
        writeInProgramFile(format("\tje Rot%d", label2));
        writeInProgramFile("\tcmp bl, '.'");
        writeInProgramFile(format("\tje Rot%d", label3));
        writeInProgramFile("\timul ecx");
        writeInProgramFile("\tsub bl, '0'");
        writeInProgramFile("\tadd eax, ebx");
        writeInProgramFile("\tadd rsi, 1");
        writeInProgramFile("\tmov bl, [rsi]");
        writeInProgramFile(format("\tjmp Rot%d", label1));
        
        // Rotulo 3
        writeInProgramFile(format("Rot%d:", label3));
        writeInProgramFile(";calcula parte fracionária em xmm0");
        writeInProgramFile("\tadd rsi, 1");
        writeInProgramFile("\tmov bl, [rsi]");
        writeInProgramFile("\tcmp bl, 0Ah");
        writeInProgramFile(format("\tje Rot%d", label2));
        writeInProgramFile("\tsub bl, '0'");
        writeInProgramFile("\tcvtsi2ss xmm1,rbx");
        writeInProgramFile("\tdivss xmm1,xmm2");
        writeInProgramFile("\taddss xmm0,xmm1");
        writeInProgramFile("\tmulss xmm2,xmm3");
        writeInProgramFile(format("\tjmp Rot%d", label3));


        // Rotulo 2
        writeInProgramFile(format("Rot%d:", label2));
        writeInProgramFile("\tcvtsi2ss xmm1,rax");
        writeInProgramFile("\taddss xmm0,xmm1");
        writeInProgramFile("\tpop rcx");
        writeInProgramFile("\tcvtsi2ss xmm1,rcx");
        writeInProgramFile("\tmulss xmm0,xmm1");

        writeInProgramFile(format("\tmovss [qword M+%ld], xmm0",t->getTokenAddr()));

    }
    else if (t->getTokenType() == TOKEN_TYPE_BOOLEAN)
    {
    }
}
