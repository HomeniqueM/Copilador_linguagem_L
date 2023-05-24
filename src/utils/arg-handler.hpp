/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
*/
#ifndef UTILS_ARG_HANDLER
#define UTILS_ARG_HANDLER
#include <string>
#include "../error/l_exception.cpp"
/**
 * @brief Classe do utilitario de argumetos do compilador
*/
class ArgHandler{
    public:
        std::string handleArgs(int argc, char const *argv[]);
};

#endif