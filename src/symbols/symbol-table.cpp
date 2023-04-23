#ifndef SYMBOLS_SYMBOL_TABLE
#define SYMBOLS_SYMBOL_TABLE
#include <iostream>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <vector>
#include "token.cpp"
#include "token-id.cpp"
#include "lexeme.cpp"

// classe tabela de símbolos
class SymbolTable
{
private:
	std::unordered_map<std::string, Token> table;
	SymbolTable *prev;

public:
	SymbolTable();
	SymbolTable(SymbolTable *t);
	bool isItAValidChar(char c);
	bool Insert(std::string lexem, Token tok);
	Token *Find(std::string lexem);
};

/**
 * @brief construtor para a primeira tabela de escopo mais abrangente
 *
 */
SymbolTable::SymbolTable() : prev(nullptr)
{
	// passar para a classe lexeme
	std::vector<std::string> init = {
		"true", "false", "boolean", "mod", "string", "write", "writeln", "readln", "div", "end",
		"begin", "and", "or", "not", "else", "real", "if", "for", "char", "integer", "final",
		"(", ")", "[", "]", "+", "-", "*", "/", ";", ":", ",", "=", "==", "<", "<=", ">", ">=", "<>"};

	Token symbl;
	TokenID id;
	for (std::string s = init.back(); !init.empty(); init.pop_back(), s = init.back())
	{
		id = stringToTokenId(s);
		symbl.setTokenID(id);
		this->Insert(s, symbl);

		/*std::cout <<"inserted " +s+" at ";
		std::cout << this->Find(s);
		std::cout << "\n";*/
	}
}

// construtor para novas tabelas criadas quando temos novos escopos
SymbolTable::SymbolTable(SymbolTable *t) : prev(t)
{
}

//
// retorna se o símbolo foi inserido na tabela

/**
 * @brief insere um símbolo na tabela
 */
bool SymbolTable::Insert(std::string lexem, Token tok)
{
	const auto& [c,success] = table.insert({lexem,tok});
	return success;
}

// busca um símbolo na tabela atual e retorna o endereço do registro
// se não encontrado, busca nas tabelas dos escopos envolventes
// caso não encontre em nenhuma tabela de nenhum escopo envolvente retorna um endereço nulo
Token *SymbolTable::Find(std::string lexem)
{
	for (SymbolTable *st = this; st != nullptr; st = st->prev)
	{
		auto found = st->table.find(lexem);
		if (found != st->table.end())
			return &found->second;
	}

	return nullptr;
}

bool SymbolTable::isItAValidChar(char c)
{

	//	return std::isalnum(c) || std::isalpha(c) ;
	return std::isalnum(c) || std::isalpha(c) || (std::find(LEXEME_VALID_SYMBOLS.begin(), LEXEME_VALID_SYMBOLS.end(), c) != LEXEME_VALID_SYMBOLS.end());
}

#endif

int main(){
	SymbolTable symtable;
	Token symbl;
	TokenID id = TOKEN_ID_INTEGER;
	symbl.setTokenID(id);
	symtable.Insert("integer",symbl);
	std::cout << symtable.Find("integer");
	return 0;
}
