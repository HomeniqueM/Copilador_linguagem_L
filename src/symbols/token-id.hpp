/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
*/

#ifndef SYMBOL_TOKEN_ID
#define SYMBOL_TOKEN_ID

#include <string>
#include <map>
enum TokenID
{
    TOKEN_ID_FINAL,   // Final
    TOKEN_ID_INTEGER, // Integer
    TOKEN_ID_CHAR,    // Char
    TOKEN_ID_REAL,    // Real
    TOKEN_ID_STRING,  // String
    TOKEN_ID_BOOLEAN, // boolean

    TOKEN_ID_FOR,  // For
    TOKEN_ID_IF,   // If
    TOKEN_ID_ELSE, // Else

    TOKEN_ID_AND, // And
    TOKEN_ID_OR,  // Or
    TOKEN_ID_NOT, // Not

    TOKEN_ID_EQUALS,         // ==
    TOKEN_ID_ASSIGNMENT,       // =
    TOKEN_ID_GREATER_THEN,     // >
    TOKEN_ID_GREATER_EQUAL_TO, // >=
    TOKEN_ID_LESS_THAN,        // <
    TOKEN_ID_LESS_EQUAL_TO,    // <=
    TOKEN_ID_DIFFERENT,        // <>

    TOKEN_ID_OPEN_PARANTHESES,  // (
    TOKEN_ID_CLOSE_PARANTHESES, // )
    TOKEN_ID_OPEN_BRACKET,      // [
    TOKEN_ID_CLOSE_BRACKET,     // ]
    TOKEN_ID_COMMA,             // ,
    TOKEN_ID_ADDITION,          // +
    TOKEN_ID_SUBTRACTION,       // -
    TOKEN_ID_DIVISION,          // / | DIV
    TOKEN_ID_MULTIPLICATION,    // *
    TOKEN_ID_MODULO,            // mod

    TOKEN_ID_CONSTANT,   // constant
    TOKEN_ID_IDENTIFIER, // identifier

    TOKEN_ID_SEMICOLON, // ;
    TOKEN_ID_COLON,     // :
    TOKEN_ID_FALSE,     // false
    TOKEN_ID_TRUE,      // true
    TOKEN_ID_BEGIN,     // begin
    TOKEN_ID_END,       // end
    TOKEN_ID_WRITE,     // write
    TOKEN_ID_WRITELN,   // writeln
    TOKEN_ID_READLN,    // readln
    TOKEN_ID_EOF,        // \0

    TOKEN_ID_NULL       // null
};

std::string tokenToString(TokenID token);

TokenID stringToTokenId(std::string s);
#endif
