/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
*/
#pragma once

#include <string>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <cstdio>

#include "../symbols/token.hpp"
#include "../symbols/token-type.hpp"

/**
 * @brief Formata uma string em C++ estilo printf
 * 
 * Esta função aceita uma string de formato e um número variável de argumentos, 
 * formata a string de acordo com os argumentos e retorna a string formatada.
 * 
 * A função é segura e lança uma exceção std::runtime_error se um erro ocorrer durante 
 * a formatação (por exemplo, se a string de formato for inválida).
 * 
 * @tparam Args Tipo dos argumentos que serão usados para formatar a string.
 * @param format A string de formato.
 * @param args Os argumentos que serão usados para formatar a string.
 * @return A string formatada.
 * 
 * @throws std::runtime_error Se um erro ocorrer durante a formatação.
 */
template<typename ... Args>
std::string format(const std::string& format, Args ... args);

class CodeGen{
    private:
        std::string asmFileName;
        std::ofstream programFile;
        long mem_count;
        long tmp_count;
        int label_counter;

        const int char_size=0x1;
        const int number_size=0x4;
        const int string_size=0x100;

    public:
        CodeGen(std::string asmFileName);
        ~CodeGen();
        long NewTmp(Token* t);
        void end();
        void startData();
        void startText();
        void startProgram();
        void DeclareVariable(Token* t);
        void DeclareConst(Token* t, Token* constant);
        void DeclareVet(Token *t, Token *size);
        void storeConstOnTmp(Token *t,Token *constant);
        void atributionCommand(Token *id, Token *exp);
        void vetAtribution(Token *id, Token *pos, Token *exp);
        void invertExpression(Token *exp);

        void sumOperation(Token *op1, Token *op2);
        void subOperation(Token *op1, Token *op2);
        void orOperation(Token *op1,Token *op2);

        void multiplyOperation(Token *op1, Token *op2);
        void divideOperation(Token *op1, Token *op2);
        void modOperation(Token *op1,Token *op2);
        void andOperation(Token *op1,Token *op2);

        void defineOperation(Token *op1,Token *op2, Token *operation);

        void startIF(Token *t, int begin);

        void writeInProgramFile(std::string s);
        void negExpression(Token *exp);
        void cvtToReal(Token *t);
        void cvtToInt(Token *t);

        void write(Token *t);
        void writeln();
        void readln(Token *t);
        long newTmpByTokenType(TokenType tt);
        int newLabel() {
            return label_counter++;
        }
        void initCondition(Token *t, int startLabel);
        void finalizeBlock(int startLabel, int endLabel);
        void finalizeConditionalChain(bool onElse, int startLabel, int endLabel);
        
        void writeRot(int rot);
        void writeJump(int rot);
        void compareForExpression(Token *exp, int rot);
        void vectorAccess(Token *id, Token *exp, Token *t);

        void RelacionalOperator(Token *op1, Token *op2,TokenID op);
};
