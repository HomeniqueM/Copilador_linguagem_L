/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
*/
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <algorithm>
#include "../symbols/token.hpp"
#include "../error/l_exception.cpp"
#include "../symbols/token-type.hpp"
#include "../analyzers/lexical-analysis.hpp"

class SyntaticAnalysis
{
private:
    LexerAnalysis *la;
    Token *token;
    void setToken(Token *token);
    void matchToken(TokenID expectedToken);
    void productionS();
    void productionD();
    void productionD1();
    void productionC();
    void productionCMD();
    void productionCMD1();
    void productionA();
    void productionR();
    void productionR1();
    void productionT();
    void productionT1();
    void productionL();
    void productionE();
    void productionE1();
    void productionExp();
    void productionExp1();
    void productionExp2();
    void productionExp3();
    void productionExp4();
    void productionExp5();

public:
    SyntaticAnalysis(LexerAnalysis *la);
    void Start(Token *token);
};