/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
*/
#ifndef SYMBOL_LEXEME
#define SYMBOL_LEXEME

#include <vector>
#include <string>
#include <algorithm>

/**
 * @brief todos os simbolos unicos validos dentro do analisador lexico
 */
const char LEXEME_UNDERSCORE = '_';
const char LEXEME_BREAK_LINE = '\n';
const char LEXEME_EOF = '\0';


const std::vector<char> LEXEME_VALID_UNARY_SYMBOLS{
    '(',
    ')',
    '[',
    ']',
    ',',
    ';',
    '+',
    '-',
    '*',
    '/'};

const std::vector<char> LEXEME_VALID_SYMBOLS{
    ' ',
    '_',
    '.',
    ',',
    ';',
    ':',
    '(',
    ')',
    '[',
    ']',
    '{',
    '}',
    '+',
    '-',
    '\"',
    '\'',
    '\\',
    '/',
    '@',
    '&',
    '%',
    '!',
    '?',
    '>',
    '<',
    '*',
    '=',
    '\n',
    '\t',
    '\r',
    '\0'};

const std::vector<char> LEXEME_VALID_ALPHABET_HEXA_SYMBOLS{
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'a',
    'b',
    'c',
    'd',
    'e',
    'f'};

const std::vector<char> LEXEME_DELIMITER{
    ' ',
    '\n',
    '\r',
    '\t'
};

bool isItaAlphabetHexa(char c);
bool isAValidUnitarySymbol(char c);
bool isALexemeDelimiter(char c);

#endif
