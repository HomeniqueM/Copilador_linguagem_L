#include <unordered_map>
#include <string>
#include "Simbolo.h"
using std::unordered_map;
using std::string;


// tabela de símbolos
class TabeladeSimbolo
{
private: 
   	unordered_map<string,Simbolo> table;    
   	TabeladeSimbolo * prev;   

public:
	TabeladeSimbolo();
	TabeladeSimbolo(TabeladeSimbolo * t);
	
	bool Insert(string s, Simbolo symb);
	Simbolo * Find(string s); 
};
