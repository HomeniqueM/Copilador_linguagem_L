/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
*/
#ifndef SYMBOLS_TOKEN_TYPE 
#define SYMBOLS_TOKEN_TYPE

enum TokenType{
    TOKEN_TYPE_INTEGER,     // Integer
    TOKEN_TYPE_CHAR,        // Char
    TOKEN_TYPE_REAL,        // Real
    TOKEN_TYPE_STRING,      // String
    TOKEN_TYPE_BOOLEAN,     // boolean
    TOKEN_TYPE_UNDEFINED    // Não definido 
};

#endif