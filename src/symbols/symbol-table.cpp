#ifndef SYMBOLS_SYMBOL_TABLE
#define SYMBOLS_SYMBOL_TABLE
#include <iostream>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include "token.cpp"
#include "token-id.cpp"
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

/**
 * @brief construtor para a primeira tabela de escopo mais abrangente
 *
 *
 */
SymbolTable::SymbolTable()
{
	// passar para a classe lexeme
	std::vector<std::string> init = {
		"true", "false", "boolean", "mod", "string", "write", "writeln", "readln", "div", "end",
		"begin", "and", "or", "not", "else", "real", "if", "for", "char", "integer", "final",
		"(", ")", "[", "]", "+", "-", "*", "/", ";", ":", ",", "=", "==", "<", "<=", ">", ">=", "<>"};

	Token symbl = Token();
	TokenID id;

	for (auto &s : init){
		id = stringToTokenId(s);
		symbl.setTokenID(id);
		symbl.setLexeme(s);
		Insert(symbl);
	}
}

/**
 * @brief insere um símbolo na tabela
 * @return retorna a posição de inserção do símbolo
 */
Token *SymbolTable::Insert(Token token)
{
	Token *res = this->Find(token.getLexeme());
	if (res == nullptr)
	{

		std::pair<std::unordered_map<std::string, Token>::iterator, bool> rtn;
		rtn = this->table.insert({token.getLexeme(), token});
		return &(rtn.first->second);
	}

	return res;
}
/**
 * @brief busca um símbolo na tabela atual e retorna o endereço do registro
 * @if se não encontrar o símbolo na tabela retorna nulo
 */
Token *SymbolTable::Find(std::string lexem)
{
	auto found = this->table.find(lexem);

	if (found != this->table.end())
	{
		return &found->second;
	}
	return nullptr;
}

bool SymbolTable::isItAValidChar(char c)
{

	//	return std::isalnum(c) || std::isalpha(c) ;
	return std::isdigit(c) || std::isalpha(c) || (std::find(LEXEME_VALID_SYMBOLS.begin(), LEXEME_VALID_SYMBOLS.end(), c) != LEXEME_VALID_SYMBOLS.end());
}

#endif

/*int main(){
	SymbolTable symtable;
	std::cout << symtable.Find("integer")<<"\n";
	Token symbl;
	TokenID id;
	std::string s = "integer";
	id = stringToTokenId(s);
	symbl.setTokenID(id);
	symbl.setLexeme(s);
	std::cout << symtable.Insert(symbl);
	return 0;
}*/
