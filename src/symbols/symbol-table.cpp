#include <unordered_map>
#include <string>
#include <algorithm>
#include "token.cpp"
#include "lexeme.cpp"

using std::string;
using std::unordered_map;

// tabela de símbolos
class SymbolTable
{
private:
	unordered_map<string, Token> table;
	SymbolTable *prev;

public:
	SymbolTable();
	SymbolTable(SymbolTable *t);
	bool isItAValidChar(char c);
	bool isItaAlphabetHexa(char c); 
	bool Insert(string s, Token symb);
	Token *Find(string s);
};

// construtor para a primeira tabela
SymbolTable::SymbolTable() : prev(nullptr)
{
}

// construtor para novas tabelas
SymbolTable::SymbolTable(SymbolTable *t) : prev(t)
{
}

// insere um símbolo na tabela
bool SymbolTable::Insert(string s, Token symb)
{
	// const auto& [c, success] = table.insert({s,symb});
	// return success;
	return true;
}

// busca um símbolo na tabela atual
// se não encontrado, busca nas tabelas dos escopos envolventes
Token *SymbolTable::Find(string s)
{
	for (SymbolTable *st = this; st != nullptr; st = st->prev)
	{
		auto found = st->table.find(s);
		if (found != st->table.end())
			return &found->second;
	}

	return nullptr;
}
/**
 * @brief Dado um char c, é verificado se o mesmo é numérico ou letra, ou está contido na lista de símbolos válidos.
 * 		Caso contrário, retorna false.
 */

bool SymbolTable::isItAValidChar(char c)
{
	return std::isalnum(c) || std::isalpha(c) ;
//	return std::isalnum(c) || std::isalpha(c) || (std::find(VALID_SYMBOLS.begin(), VALID_SYMBOLS.end(), c) != VALID_UNARY_SYMBOLS.end());


}

