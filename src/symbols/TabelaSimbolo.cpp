#include <unordered_map>
#include <string>
#include "token.cpp"

using std::unordered_map;
using std::string;


// tabela de símbolos
class SymbolTable
{
private: 
   	unordered_map<string,Token> table;    
   	SymbolTable * prev;   

public:
	SymbolTable();
	SymbolTable(SymbolTable * t);
	
	bool Insert(string s, Token symb);
	Token * Find(string s); 
};


// construtor para a primeira tabela
SymbolTable::SymbolTable() : prev(nullptr)
{
}

// construtor para novas tabelas
SymbolTable::SymbolTable(SymbolTable * t) : prev(t)
{		
}

// insere um símbolo na tabela
bool SymbolTable::Insert(string s, Token symb) 
{ 
	const auto& [c, success] = table.insert({s,symb});
	return success;
}

// busca um símbolo na tabela atual 
// se não encontrado, busca nas tabelas dos escopos envolventes
Token * SymbolTable::Find(string s) 
{
	for (SymbolTable * st = this; st != nullptr; st = st->prev) 
	{
        auto found = st->table.find(s);
        if (found != st->table.end()) 
			return &found->second;
    }

    return nullptr;
} 