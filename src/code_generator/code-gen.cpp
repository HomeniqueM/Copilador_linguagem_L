#include <string>
#include <fstream>
#include "../symbols/token.cpp"
#include "../symbols/token-type.cpp"


class CodeGen{
    private :
        std:string pfp;
        std:ofstream pf;
        long mem_count;
        long tmp_count;
        int labl_count;

        const int char_size=0x1;
        const int number_size=0x4;
        const int string_size=0x100;

    public:
        CodeGen(std:string pfp = "program.asm");
        ~CodeGen();
        void end();
        void startData();
        void startText();
        void startProgram();
        void DeclareVariable(Token t);
        void DeclareConst(Token t, Token constant);
        long NewTmp();
        void storeConstOnTmp(Token constant);
    
}
CodeGen::CodeGen(std:string pfp){
    this->mem_count=0x10000;
    this->tmp_count=0x0;
    this->labl_count=1;
    this->pfp = pfp;
    this->pf.open(pfp);
}
CodeGen::~CodeGen(){
    this->pf.close();
}
//cria novo temporário
long CodeGen::NewTmp(Token t){
    long addr = this->tmp_count;
    if(t.getTokenType()==TOKEN_TYPE_STRING){
        this->tmp_count+=this->string_size;
    }else if(t.getTokenType()==TOKEN_TYPE_CHAR||t.getTokenType()==TOKEN_TYPE_BOOLEAN){
        this->tmp_count+=this->char_size;
    }else if(t.getTokenType()==TOKEN_TYPE_INTEGER||t.getTokenType()==TOKEN_TYPE_REAL){
        this->tmp_count+=this->number_size;
    }
    return addr;
}
void CodeGen::end(){
    this->pf << "mov rax, 60 ;chamada de saída\n";
    this->pf << "mov rdi, 0 ;código de saida sem erros\n";
    this->pf << "syscall ;chama o kernel\n";
}
void CodeGen::startData(){
    this->pf << "section .data\n";
}
void CodeGen::startText(){
    this->pf << "section .text\n";
}
void CodeGen::startProgram(){
    this->startData();
    this->pf << "M:\n";
    this->pf << "\t resb 10000h ;temporários\n";
}
void CodeGen::DeclareVariable(Token t){
    if(t.getTokenType()==TOKEN_TYPE_CHAR){
        this->mem_count+=this->char_size; 
        this->pf <<"\t resb 1"<<"\n";
    }else if(t.getTokenType()==TOKEN_TYPE_INTEGER||t.getTokenType()==TOKEN_TYPE_REAL){
        this->mem_count+=this->number_size;
        this->pf <<"\t resd 1"<<"\n";
    }else if(t.getTokenType()==TOKEN_TYPE_STRING){
        this->mem_count+=this->string_size;
        this->pf <<"\t resb 100h"<<"\n";
    }
}
void CodeGen::DeclareConst(Token t, Token constant){
    if(t.getTokenType()==TOKEN_TYPE_CHAR){
        this->mem_count+=constant.getTokeSize();
        this->pf << format("db %s 1", constant.getLexeme())<<"\n";
    }else if(t.getTokenType()==TOKEN_TYPE_INTEGER||t.getTokenType()==TOKEN_TYPE_REAL){
        this->mem_count+=constant.getTokesize();
        this->pf << format("dd %s", constant.getLexeme())<<"\n";
    }else if(t.getTokenType()==TOKEN_TYPE_STRING){
        this->mem_count+=constant.getTokeSize();
        this->pf << format("db %s, 0", constant.getLexeme())<<"\n";
    }
}
//guarda valor de uma constante em um temporário
void CodeGen::storeConstOnTmp(Token constant){
    if(constant.getTokenType()==TOKEN_TYPE_REAL){
        this->mem_count+=constant.getTokeSize();
        this->startData();
        this->pf << format("dd %s", constant.getLexeme())<<"\n";
        this->startText();
    }else if(constant.getTokenType()==TOKEN_TYPE_STRING){
        this->mem_count+=constant.getTokeSize();
        this->startData();
        this->pf << format("db %s,0", constant.getLexeme())<<"\n";
        this->startText();
    }else if(constant.getTokenType()==TOKEN_TYPE_CHAR){
        long addr=this->NewTmp(constant);
        this->pf << format("mov al, %s", constant.getLexeme())<<"\n";
        this->pf << format("mov [qword M+%ld], al",addr)<<"\n";
    }else if(constant.getTokenType()==TOKEN_TYPE_INTEGER){
        long addr=this->NewTmp(constant);
        this->pf << format("mov eax, %s", constant.getLexeme())<<"\n";
        this->pf << format("mov [qword M+%ld], eax",addr)<<"\n";
    }
}
//comando de atribuição
void CodeGen::atributionCommand(Token id, token exp){
    if(id.getTokenType==TOKEN_TYPE_INTEGER){
        this->pf << format("mov eax,[qword M+%ld]",exp.getAddr())<<"\n";
        this->pf << format("mov [qword M+%ld], eax",id.getAddr())<<"\n";
    }else if(id.getTokenType==TOKEN_TYPE_REAL){
        this->pf << format("mov xmm0,[qword M+%ld]",exp.getAddr())<<"\n";
        this->pf << format("mov [qword M+%ld], xmm0",id.getAddr())<<"\n";
    }else if(id.getTokenType==TOKEN_TYPE_CHAR){
        this->pf << format("mov al,[qword M+%ld]",exp.getAddr())<<"\n";
        this->pf << format("mov [qword M+%ld], al",id.getAddr())<<"\n";
    }
}
//inverte valor da expressão
void CodeGen::invertExpression(Token exp){
    long tmpAddr=this->NewTmp(exp);
    if(f1.getTokenType==TOKEN_TYPE_INTEGER){
        this->pf << format("mov eax, [qword M+%ld]",exp.getAddr())<<"\n";
        this->pf << "neg eax"<<"\n";
        this->pf << format("mov [qword M+%ld], eax",tmpAddr)<<"\n";
    }else if(f1.getTokenType==TOKEN_TYPE_REAL){
        this->pf << format("movss xmm0, [qword M+%ld]",exp.getAddr())<<"\n";
        this->pf << "mov rax, -1"<<"\n";
        this->pf << "cvtsi2ss xmm1, rax"<<"\n";
        this->pf << "mulss xmm0, xmm1"<<"\n";
        this->pf << format("movss [qword M+%ld], xmm0",tmpAddr)<<"\n";
    }
}
//negação da expressão
void CodeGen::negExpression(Token exp){
    long tmpAddr=this->NewTmp(exp);
    this->pf << format("mov al, [qword M+%ld]", exp.getAddr())<<"\n";
    this->pf << "neg al"<<"\n";
    this->pf << "add al,1"<<"\n";
    this->pf << format("mov [qword M+%ld], al",tmpAddr)<<"\n";
}
//converte valor inteiro para real
void CodeGen::cvtToReal(Token t){
    this->pf << format("mov rax, [qword M+%ld]", t.getAddr())<<"\n";
    long tmpAddr = this->NewTmp(t);
    this->pf << "cvtsi2ss xmm0, rax"<<"\n";
    this->pf << format("movss [qword M+%d], xmm0", tmpAddr);
}
//converte valor real para inteiro
void CodeGen::cvtToInt(){
    this->pf << format("mov xmm0, [qword M+%ld]", t.getAddr())<<"\n";
    long tmpAddr = this->NewTmp(t);
    this->pf << "cvtsi2ss rax, xmm0"<<"\n";
    this->pf << format("mov [qword M+%d], rax", tmpAddr);
}