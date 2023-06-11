/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
*/

#include "token-id.hpp"
#include <string>
#include <map>

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
    case TOKEN_ID_EQUALS:
        return "==";
    case TOKEN_ID_ASSIGNMENT:
        return "=";
    case TOKEN_ID_GREATER_THEN:
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
    case TOKEN_ID_COLON:
        return ":";
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
    case TOKEN_ID_EOF:
        return "/0";
    default:
        return "NULL";
    }
}

TokenID stringToTokenId(std::string s)
{
    static const std::map<std::string, TokenID> tokenMap = {
        {"true", TOKEN_ID_TRUE},
        {"false", TOKEN_ID_FALSE},
        {"boolean", TOKEN_ID_BOOLEAN},
        {"mod", TOKEN_ID_MODULO},
        {"string", TOKEN_ID_STRING},
        {"write", TOKEN_ID_WRITE},
        {"writeln", TOKEN_ID_WRITELN},
        {"readln", TOKEN_ID_READLN},
        {"div", TOKEN_ID_DIVISION},
        {"end", TOKEN_ID_END},
        {"begin", TOKEN_ID_BEGIN},
        {"and", TOKEN_ID_AND},
        {"or", TOKEN_ID_OR},
        {"not", TOKEN_ID_NOT},
        {"else", TOKEN_ID_ELSE},
        {"real", TOKEN_ID_REAL},
        {"if", TOKEN_ID_IF},
        {"for", TOKEN_ID_FOR},
        {"char", TOKEN_ID_CHAR},
        {"integer", TOKEN_ID_INTEGER},
        {"final", TOKEN_ID_FINAL},
        {"(", TOKEN_ID_OPEN_PARANTHESES},
        {")", TOKEN_ID_CLOSE_PARANTHESES},
        {"[", TOKEN_ID_OPEN_BRACKET},
        {"]", TOKEN_ID_CLOSE_BRACKET},
        {"+", TOKEN_ID_ADDITION},
        {"-", TOKEN_ID_SUBTRACTION},
        {"*", TOKEN_ID_MULTIPLICATION},
        {"/", TOKEN_ID_DIVISION},
        {";", TOKEN_ID_SEMICOLON},
        {":", TOKEN_ID_COLON},
        {",", TOKEN_ID_COMMA},
        {"=", TOKEN_ID_ASSIGNMENT},
        {"==", TOKEN_ID_EQUALS},
        {"<", TOKEN_ID_LESS_THAN},
        {"<=", TOKEN_ID_LESS_EQUAL_TO},
        {">", TOKEN_ID_GREATER_THEN},
        {">=", TOKEN_ID_GREATER_EQUAL_TO},
        {"<>", TOKEN_ID_DIFFERENT}};

    auto it = tokenMap.find(s);
    if (it != tokenMap.end())
    {
        return it->second;
    }
    return TOKEN_ID_NULL;
}

