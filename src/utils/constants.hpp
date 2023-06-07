/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
*/
#ifndef UTILS_CONSTANTES
#define UTILS_CONSTANTES
#include <iostream>
/**
 * @brief classe responsável por armazenar constantes
 *
 */

/// @brief Número maximo de caracteres de um identificador
const static int CONSTANTS_IDENTIFIER_MAX_SIZE = 32;
/// @brief Limite da casas de precissão de FLOT
const static int CONSTANTS_MAXIMUM_ACCURACY_LENGTH = 6;
/// @brief Valores minimo(-2^(31)) e maximo((2^(31))-1) de que um valor inteiro pode possuir
const static int CONSTANTS_INTEGER_MIN_VALUE = -2147483648;
const static int CONSTANTS_INTEGER_MAX_VALUE = 2147483647;

/// @brief Valores minimo e maximo de que um valor inteiro pode possuir
const static float CONSTANTS_REAL_MIN_VALUE = -99999.9;
const static float CONSTANTS_REAL_MAX_VALUE = 99999.9;

/// @brief espaço ocupado por cada variavel
const static size_t CONSTANTS_CHAR_SIZE = sizeof(char);
const static size_t CONSTANTS_BOOLEAN_SIZE = sizeof(bool) ;
const static size_t CONSTANTS_INTEGER_SIZE = sizeof(int)  ;
const static size_t CONSTANTS_REAL_SIZE = sizeof(double);
#endif