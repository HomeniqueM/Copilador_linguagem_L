/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
*/
#include "arg-handler.hpp"
/**
 *@brief Metodo que trata os argumentos do compilador gerando um erro caso o caminho do arquivo não tenha sido fornecido
 *@return retorna uma string com o caminho do arquivo do programa fonte
*/
std::string ArgHandler::handleArgs(int argc, char const *argv[]){
    std::string path;
    if(argc < 2){
        throw LException(ErrorCode::NO_FILE_PATH_FOUND,__LINE__);
    }else{
        path = argv[1];
    }
    return path;
} 
