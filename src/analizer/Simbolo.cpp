#include <iostream>
#include "Simbolo.h"


Simbolo::Simbolo(string variavel, string tipo)
{
    this->tipo = tipo;
    this->variavel = variavel;
}

Simbolo::Simbolo()
{

}

Simbolo::~Simbolo()
{
}

void Simbolo::printSimbolo(){
   // cout << "Variavel: " + this->variavel + '\n';
   // cout <<  "Type: " + this->tipo;
}
