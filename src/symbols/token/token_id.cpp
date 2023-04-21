#include <string>

#ifndef SYMBOL_TOKEN_ID_CPP
#define SYMBOL_TOKEN_ID_CPP

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

    TOKEN_ID_EQUALITY,         // ==
    TOKEN_ID_ASSIGNMENT,       // =
    TOKEN_ID_BIGGER_THEN,      // >
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
    TOKEN_ID_FALSE,     // false
    TOKEN_ID_TRUE,      // true
    TOKEN_ID_BEGIN,     // begin
    TOKEN_ID_END,       // end
    TOKEN_ID_WRITE,     // write
    TOKEN_ID_WRITELN,   // writeln

    TOKEN_ID_READLN // readln
};

std::string tokenToString(TokenID token)
{
    switch (token)
    {
    case TOKEN_ID_FINAL:
        return "final";
    case TOKEN_ID_INTEGER:
        return "integer";
    case TOKEN_ID_CHAR:
        return "char";
    case TOKEN_ID_REAL:
        return "real";
    case TOKEN_ID_STRING:
        return "string";
    case TOKEN_ID_BOOLEAN:
        return "boolean";
    case TOKEN_ID_FOR:
        return "for";
    case TOKEN_ID_IF:
        return "if";
    case TOKEN_ID_ELSE:
        return "else";
    case TOKEN_ID_AND:
        return "and";
    case TOKEN_ID_OR:
        return "or";
    case TOKEN_ID_NOT:
        return "not";
    case TOKEN_ID_EQUALITY:
        return "==";
    case TOKEN_ID_ASSIGNMENT:
        return "=";
    case TOKEN_ID_BIGGER_THEN:
        return ">";
    case TOKEN_ID_GREATER_EQUAL_TO:
        return ">=";
    case TOKEN_ID_LESS_THAN:
        return "<";
    case TOKEN_ID_LESS_EQUAL_TO:
        return "<=";
    case TOKEN_ID_DIFFERENT:
        return "<>";
    case TOKEN_ID_OPEN_PARANTHESES:
        return "(";
    case TOKEN_ID_CLOSE_PARANTHESES:
        return ")";
    case TOKEN_ID_OPEN_BRACKET:
        return "[";
    case TOKEN_ID_CLOSE_BRACKET:
        return "]";
    case TOKEN_ID_COMMA:
        return ",";
    case TOKEN_ID_ADDITION:
        return "+";
    case TOKEN_ID_SUBTRACTION:
        return "-";
    case TOKEN_ID_DIVISION:
        return "/";
    case TOKEN_ID_MULTIPLICATION:
        return "*";
    case TOKEN_ID_MODULO:
        return "mod";
    case TOKEN_ID_CONSTANT:
        return "constant";
    case TOKEN_ID_IDENTIFIER:
        return "identifier";
    case TOKEN_ID_SEMICOLON:
        return ";";
    case TOKEN_ID_FALSE:
        return "false";
    case TOKEN_ID_TRUE:
        return "true";
    case TOKEN_ID_BEGIN:
        return "begin";
    case TOKEN_ID_END:
        return "end";
    case TOKEN_ID_WRITE:
        return "write";
    case TOKEN_ID_WRITELN:
        return "writeln";
    case TOKEN_ID_READLN:
        return "readln";
    default:
        return "NULL";
    }
}

#endif



