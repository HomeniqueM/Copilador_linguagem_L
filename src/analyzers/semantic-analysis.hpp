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

class SemanticAnalysis 
{ 
private:
    LexerAnalysis *la;
    void tokenIdSetType(Token *tokenConst, Token *tokenId);
    void tokenIdSetClass(Token *tokenConst, Token *tokenId);




public:
    SemanticAnalysis(LexerAnalysis *la);
    void defineNewType(Token *t, TokenType typeToken);
    void isTokenHasDeclarad(Token *t, TokenClass classToken);
    void isTokenTypeEquals(Token *tokenConst, Token *tokenId, bool isneg);
    void ifTokenTypehasDiff(Token *token, TokenType typeToken);
    void ifTokenTypeHasEqualsorIntandReal(Token *token, TokenType typeToken);
    void isTokenNotHasDeclarationAndNotHasConst(Token *t);
    void isTokenNotHasDeclaration(Token *t);
    void TokenReplaceType(Token *token);
    void tokenIsBoolean(Token *token, bool isNeg);
    void tokenIsMultDivAndMod(Token *token);
    void tokenIsIntergerOrReal(Token *token, bool isNeg);
    //void setMaxTamVet();
    void rulle20(Token *tokenA, Token *tokenB, Token *operador);
    void rulle23(Token *tokenA, Token *tokenB, Token *operador);
    void rulle25(Token *tokenA, Token *tokenB, Token *operador);
    void setMaxTamVet(Token *tokenId, Token *tokenConst);
    void ifTokenVectorInRange(Token *tokenId, Token *tokenConst);
};