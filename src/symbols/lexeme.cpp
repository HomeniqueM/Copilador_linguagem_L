#ifndef SYMBOLS_LEXEME
#define SYMBOLS_LEXEME

#include <vector>
#include <string>
#include <algorithm>

/**
 * @brief todos os simbolos unicos validos dentro do analisador lexico
 */
const std::vector<char> VALID_UNARY_SYMBOLS{
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

const std::vector<char> VALID_SYMBOLS{
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
    '\0'
    };

const std::vector<char> VALID_ALPHABET_HEXA_SYMBOLS{
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
    'f'
};

const char LEXEME_EOF(){
    return '\0';
}

bool isItaAlphabetHexa(char c)
{
    return std::find(VALID_ALPHABET_HEXA_SYMBOLS.begin(), VALID_ALPHABET_HEXA_SYMBOLS.end(), c) != VALID_ALPHABET_HEXA_SYMBOLS.end();
}

#endif