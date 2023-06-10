/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
*/
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "../error/l_exception.cpp"
/**
 * @brief Classe do utilitario de leitura do arquivo do programa fonte a ser compilado
*/
class FileHandler{
    private:
        std::map<int, std::string> fileContent;
        std::size_t line;
        std::size_t line_pos;
    public:
        FileHandler(std::string path);
        char getNextFileChar();
        int getFileLine();
        int getFileLinePos();
        bool isEof();
        void setPrevChar();
};
