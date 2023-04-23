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

bool isItaAlphabetHexa(char c)
{
    return std::find(LEXEME_VALID_ALPHABET_HEXA_SYMBOLS.begin(), LEXEME_VALID_ALPHABET_HEXA_SYMBOLS.end(), c) != LEXEME_VALID_ALPHABET_HEXA_SYMBOLS.end();
}

bool isAValidUnitarySymbol(char c)
{
    return std::find(LEXEME_VALID_UNARY_SYMBOLS.begin(), LEXEME_VALID_UNARY_SYMBOLS.end(), c) != LEXEME_VALID_UNARY_SYMBOLS.end();
}

#endif