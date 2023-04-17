#include "TabeladeSimbolo.h"
#include "Simbolo.h"

// construtor para a primeira tabela
TabeladeSimbolo::TabeladeSimbolo() : prev(nullptr)
{
}

// construtor para novas tabelas
TabeladeSimbolo::TabeladeSimbolo(TabeladeSimbolo * t) : prev(t)
{		
}

// insere um símbolo na tabela
bool TabeladeSimbolo::Insert(string s, Simbolo symb) 
{ 
	const auto& [pos, success] = table.insert({s,symb});
	return success;
}

// busca um símbolo na tabela atual 
// se não encontrado, busca nas tabelas dos escopos envolventes
Simbolo * TabeladeSimbolo::Find(string s) 
{
	for (TabeladeSimbolo * st = this; st != nullptr; st = st->prev) 
	{
        auto found = st->table.find(s);
        if (found != st->table.end()) 
			return &found->second;
    }

    return nullptr;
} 