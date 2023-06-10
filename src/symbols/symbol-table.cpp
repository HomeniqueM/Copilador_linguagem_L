/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
*/
#include "symbol-table.hpp"
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
