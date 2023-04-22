#ifndef SYMBOLS_LEXEME
#define SYMBOLS_LEXEME

#include <vector>
#include <string>

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
#endif