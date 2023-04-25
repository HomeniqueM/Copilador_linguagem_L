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

/**
 * @brief classe tabela de símbolos
*/
class SymbolTable
{
private:
   	std::unordered_map<std::string,Token> table;       

public:
	SymbolTable();
	bool isItAValidChar(char c);
	Token *Insert(std::string lexem, Token tok);
	Token *Find(std::string lexem);
};

/**
 * @brief construtor para a primeira tabela de escopo mais abrangente
 *
 */
SymbolTable::SymbolTable()
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


/**
 * @brief insere um símbolo na tabela
 */
Token * SymbolTable::Insert(std::string lexem, Token tok)
{
	const auto& [c,success] = table.insert({lexem,tok});
	
	if(success){
		Token * res=this->Find(lexem);
		return res;
	}else{
		return nullptr;
	}
}
/**
 * @brief busca um símbolo na tabela atual e retorna o endereço do registro
 * @if se não encontrado, busca nas tabelas dos escopos envolventes
 * @else caso não encontre em nenhuma tabela de nenhum escopo envolvente retorna um endereço nulo
*/
Token * SymbolTable::Find(std::string lexem) 
{
    auto found = this->table.find(lexem);
    if (found != this->table.end()) 
		return &found->second;
    return nullptr;
}

bool SymbolTable::isItAValidChar(char c)
{

	//	return std::isalnum(c) || std::isalpha(c) ;
	return std::isdigit(c) || std::isalpha(c) || (std::find(LEXEME_VALID_SYMBOLS.begin(), LEXEME_VALID_SYMBOLS.end(), c) != LEXEME_VALID_SYMBOLS.end());
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

