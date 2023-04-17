#ifndef SIMBOLO
#define SIMBOLO
#include <unordered_map>
#include <string>
using std::unordered_map;
using std::string;

//Simbolo Presente na Tabela de Simbolos.
class Simbolo
{
private: 
    string  variavel;
    string  tipo;

public:
	Simbolo();
    Simbolo(string variavel, string tipo);
    ~Simbolo();
    void printSimbolo();
};

#endif