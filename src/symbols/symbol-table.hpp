/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
*/
#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include "token.hpp"
#include "token-id.hpp"
#include "lexeme.cpp"

/**
 * @brief classe tabela de símbolos
 */
class SymbolTable
{
private:
	std::unordered_map<std::string, Token> table;

public:
	SymbolTable();
	bool isItAValidChar(char c);
	Token *Insert(Token token);
	Token *Find(std::string lexem);
};
