#ifndef SYMBOLS_LEXEME
#define SYMBOLS_LEXEME

#include <vector>
#include <string>
#include <algorithm>

/**
 * @brief todos os simbolos unicos validos dentro do analisador lexico
 */
const char LEXEME_UNDERSCORE = '_';
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
};
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

#endif