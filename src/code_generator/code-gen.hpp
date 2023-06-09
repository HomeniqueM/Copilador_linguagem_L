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
        long mem_count = 0;
        long tmp_count = 0;
        int label_counter = 0;

        const int char_size=0x1;
        const int number_size=0x4;
        const int string_size=0x100;

    public:
        CodeGen(std::string asmFileName = "program.asm");
        ~CodeGen();
        void end();
        void startData();
        void startText();
        void startProgram();
        void DeclareVariable(Token* t);
        void DeclareConst(Token* t, Token* constant);
        long NewTmp(Token* t);
        void storeConstOnTmp(Token* constant);
        int new_label() {
            return label_counter++;
        }
        // adicione aqui outras funções de manipulação de tipos
};
