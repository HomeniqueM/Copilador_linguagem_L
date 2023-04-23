#ifndef SYMBOL_TOKEN_ID
#define SYMBOL_TOKEN_ID

#include <string>

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
    TOKEN_ID_NULL,       // null

    TOKEN_ID_SEMICOLON, // ;
    TOKEN_ID_COLON,     // :
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
    default:
        return "NULL";
    }
}

TokenID stringToTokenId(std::string s){

    if (s.compare("true")==0)
        return TOKEN_ID_TRUE;
    if (s.compare("false")==0)
        return TOKEN_ID_FALSE;
    if (s.compare("boolean")==0)
        return TOKEN_ID_BOOLEAN;
    if (s.compare("mod")==0)
        return TOKEN_ID_MODULO;
    if(s.compare("string")==0)
        return TOKEN_ID_STRING;
    if(s.compare("write")==0)
        return TOKEN_ID_WRITE;
    if(s.compare("writeln")==0)
        return TOKEN_ID_WRITELN;
    if(s.compare("readln")==0)
        return TOKEN_ID_READLN;
    if(s.compare("div")==0)
        return TOKEN_ID_DIVISION;
    if(s.compare("end")==0)
        return TOKEN_ID_END;
    if(s.compare("begin")==0)
        return TOKEN_ID_BEGIN;
    if(s.compare("and")==0)
        return TOKEN_ID_AND;
    if(s.compare("or")==0)
        return TOKEN_ID_OR;
    if(s.compare("not")==0)
        return TOKEN_ID_NOT;
    if(s.compare("else")==0)
        return TOKEN_ID_ELSE;
    if(s.compare("real")==0)
        return TOKEN_ID_REAL;
    if(s.compare("if")==0)
        return TOKEN_ID_IF;
    if(s.compare("for")==0)
        return TOKEN_ID_FOR;
    if(s.compare("char")==0)
        return TOKEN_ID_CHAR;
    if(s.compare("integer")==0)
        return TOKEN_ID_INTEGER;
    if(s.compare("final")==0)
        return TOKEN_ID_FINAL;
    if(s.compare("(")==0)
        return TOKEN_ID_OPEN_PARANTHESES;
    if(s.compare(")")==0)
        return TOKEN_ID_CLOSE_PARANTHESES;
    if(s.compare("[")==0)
        return TOKEN_ID_OPEN_BRACKET;
    if(s.compare("]")==0)
        return TOKEN_ID_CLOSE_BRACKET;
    if(s.compare("+")==0)
        return TOKEN_ID_ADDITION;
    if(s.compare("-")==0)
        return TOKEN_ID_SUBTRACTION;
    if(s.compare("*")==0)
        return TOKEN_ID_MULTIPLICATION;
    if(s.compare("/")==0)
        return TOKEN_ID_DIVISION;
    if(s.compare(";")==0)
        return TOKEN_ID_SEMICOLON;
    if(s.compare(":")==0)
        return TOKEN_ID_COLON;
    if(s.compare(",")==0)
        return TOKEN_ID_COMMA;
    if(s.compare("=")==0)
        return TOKEN_ID_ASSIGNMENT;
    if(s.compare("==")==0)
        return TOKEN_ID_EQUALITY;
    if(s.compare("<")==0)
        return TOKEN_ID_LESS_THAN;
    if(s.compare("<=")==0)
        return TOKEN_ID_LESS_EQUAL_TO;
    if(s.compare(">")==0)
        return TOKEN_ID_GREATER_THEN;
    if(s.compare(">=")==0)
        return TOKEN_ID_GREATER_EQUAL_TO;
    if(s.compare("<>")==0)
        return TOKEN_ID_DIFFERENT; 
}

#endif



