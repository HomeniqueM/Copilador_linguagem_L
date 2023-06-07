/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
*/

#include <vector>
#include <string>
#include <algorithm>
#include "lexeme.hpp"


bool isItaAlphabetHexa(char c)
{
    return std::find(LEXEME_VALID_ALPHABET_HEXA_SYMBOLS.begin(), LEXEME_VALID_ALPHABET_HEXA_SYMBOLS.end(), c) != LEXEME_VALID_ALPHABET_HEXA_SYMBOLS.end();
}
/**
 * @brief Verifica se
 */
bool isAValidUnitarySymbol(char c)
{
    return std::find(LEXEME_VALID_UNARY_SYMBOLS.begin(), LEXEME_VALID_UNARY_SYMBOLS.end(), c) != LEXEME_VALID_UNARY_SYMBOLS.end();
}
/**
 * @brief verifica se é o char c é uma quebra de linha o espaço
 */
bool isALexemeDelimiter(char c)
{
    return std::find(LEXEME_DELIMITER.begin(), LEXEME_DELIMITER.end(), c) != LEXEME_DELIMITER.end();
}
