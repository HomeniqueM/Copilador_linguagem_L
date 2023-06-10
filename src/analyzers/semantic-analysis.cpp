/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
 */
#ifndef ANALYZERS_SEMANTIC_ANALYSIS
#define ANALYZERS_SEMANTIC_ANALYSIS
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <algorithm>
#include "../symbols/token.cpp"
#include "../error/l_exception.cpp"
#include "../symbols/token-type.cpp"
#include "../analyzers/lexical-analysis.cpp"

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
    // void setMaxTamVet();
    void rulle23(Token *tokenA, Token *tokenB, Token *operador);
    void rulle20(Token *tokenA, Token *tokenB, Token *operador);
    void setMaxTamVet(Token *tokenId, Token *tokenConst);
    void ifTokenVectorInRange(Token *tokenId, Token *tokenConst);
};

SemanticAnalysis::SemanticAnalysis(LexerAnalysis *la)
{
    this->la = la;
}

void SemanticAnalysis::defineNewType(Token *t, TokenType typeToken)
{
    t->setTokenType(typeToken);
}

void SemanticAnalysis::isTokenHasDeclarad(Token *t, TokenClass classToken)
{
    if (t->getTokenClass() == TOKEN_CLASS_UNDEFINED)
    {
        t->setTokenClass(classToken);
    }
    else
    {
        throw LException(ErrorCode::IDENTIFIER_ALREADY_DECLARED, la->getCurrentLine(), "1");
    }
}

void SemanticAnalysis::isTokenNotHasDeclarationAndNotHasConst(Token *t)
{

    if (t->getTokenClass() == TOKEN_CLASS_UNDEFINED)
    {
        throw LException(ErrorCode::IDENTIFIER_NO_DECLARED, la->getCurrentLine(), "2");
    }
    if (t->getTokenClass() == TOKEN_CLASS_CONSTANT)
    {
        throw LException(ErrorCode::MISMATCHED_IDENTIFIER_CLASS, la->getCurrentLine(), "3");
    }
}

void SemanticAnalysis::isTokenNotHasDeclaration(Token *t)
{

    if (t->getTokenClass() == TOKEN_CLASS_UNDEFINED)
    {
        throw LException(ErrorCode::IDENTIFIER_NO_DECLARED, la->getCurrentLine(), "4");
    }
}

void SemanticAnalysis::isTokenTypeEquals(Token *tokenConst, Token *tokenId, bool isneg)
{
    if (isneg && (tokenConst->getTokenType() != TOKEN_TYPE_INTEGER && tokenConst->getTokenType() != TOKEN_TYPE_REAL))
    {
        throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine(), "5");
    }
    if (tokenId->getTokenType() == TOKEN_TYPE_INTEGER && tokenConst->getTokenType() != TOKEN_TYPE_INTEGER)
    {
        throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine(), "6");
    }
    else
    {
        // tokenIdSetClass(tokenConst, tokenId);
        tokenIdSetType(tokenConst, tokenId);
    }
}

void SemanticAnalysis::tokenIdSetType(Token *tokenConst, Token *tokenId)
{
    tokenId->setTokenType(tokenConst->getTokenType());
}

void SemanticAnalysis::tokenIdSetClass(Token *tokenConst, Token *tokenId)
{
    tokenId->setTokenClass(tokenConst->getTokenClass());
}

/*
[6] ->
{
        se const.tipo != typeToken
]
*/
void SemanticAnalysis::ifTokenTypehasDiff(Token *token, TokenType typeToken)
{
    if (token->getTokenType() != typeToken)
    {
        throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine(), "7");
    }
}

void SemanticAnalysis::ifTokenTypeHasEqualsorIntandReal(Token *token, TokenType typeToken)
{
    if (token->getTokenType() != typeToken && (token->getTokenType() != TOKEN_TYPE_REAL && typeToken != TOKEN_TYPE_INTEGER))
    {
        throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine(), "8");
    }
    else
    {
        // Comando redunfdante
    }
}

void SemanticAnalysis::TokenReplaceType(Token *token)
{
    if (token->getTokenType() == TOKEN_TYPE_REAL)
    {
        token->setTokenType(TOKEN_TYPE_INTEGER);
    }
    else if (token->getTokenType() == TOKEN_TYPE_INTEGER)
    {
        token->setTokenType(TOKEN_TYPE_REAL);
    }
    else
    {
        throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine(), "9");
    }
}

void SemanticAnalysis::tokenIsBoolean(Token *token, bool isNeg)
{
    if (token->getTokenType() != TOKEN_TYPE_BOOLEAN && isNeg)
    {
        throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine(), "10");
    }
}

void SemanticAnalysis::tokenIsMultDivAndMod(Token *token)
{

    // Fazer algo
}

void SemanticAnalysis::tokenIsIntergerOrReal(Token *token, bool isNeg)
{
    if (isNeg && token->getTokenType() != TOKEN_TYPE_INTEGER && token->getTokenType() != TOKEN_TYPE_REAL)
    {
        throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine(), "10");
    }
}


/*[23] ->
{
    se Exp1.operator150 == or
        se  !( Exp1.tipo == logico e Exp2.tipo == logico)
            erro
    senao
        se (Exp1.tipo != Exp2.tipo && Exp1.tipo == real ou Exp1.tipo == inteiro && Exp2.tipo = real ou Exp2.tipo == inteiro)
            Exp1.tipo = real
    senao
        erro
}

*/
void SemanticAnalysis::rulle23(Token *tokenA, Token *tokenB, Token *operador)
{
    if (operador->getTokenid() == TOKEN_ID_OR)
    {
        if (!(tokenA->getTokenType() == TOKEN_TYPE_BOOLEAN && tokenB->getTokenType() == TOKEN_TYPE_BOOLEAN))
        {
            throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine(), "11");
        }
    }
    else
    {
        if (tokenA->getTokenType() != tokenB->getTokenType()){
            if  ((tokenA->getTokenType() == TOKEN_TYPE_REAL || tokenA->getTokenType() == TOKEN_TYPE_INTEGER) &&
                (tokenB->getTokenType() == TOKEN_TYPE_REAL || tokenB->getTokenType() == TOKEN_TYPE_INTEGER))
            {
                tokenA->setTokenType(TOKEN_TYPE_REAL);
            }
            else
            {
            throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine(), "121");
            }
        }
    }
}


/*[20] ->
{
    se¹ Exp.tipo != Exp1.Tipo 
        se² !( 	(Exp.tipo = real ou Exp.tipo = inteiro) && 
        (Exp1.tipo = real ou Exp1.tipo = inteiro))
        erro
    senao¹
        se³ Exp.tipo == “String” & Exp.operator 150!= “==”
            erro
}
*/
void SemanticAnalysis::rulle20(Token *tokenA, Token *tokenB, Token *operador)
{
    if (tokenA->getTokenType() != tokenB->getTokenType()){
            if  ( !((tokenA->getTokenType() == TOKEN_TYPE_REAL || tokenA->getTokenType() == TOKEN_TYPE_INTEGER) &&
                  (tokenB->getTokenType() == TOKEN_TYPE_REAL || tokenB->getTokenType() == TOKEN_TYPE_INTEGER)) )
            {
               throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine(), "13");
            }
    }
    else{
        if(operador->getTokenid() == TOKEN_ID_ENQUALS && tokenA->getTokenType() == TOKEN_TYPE_STRING ){
            throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine(), "14");
        }
    }
}

void SemanticAnalysis::setMaxTamVet(Token *tokenId, Token *tokenConst){
    tokenId->setMaxTam(stoi(tokenConst->getLexeme()));
}

void SemanticAnalysis::ifTokenVectorInRange(Token *tokenId, Token *tokenConst){
    if(stoi(tokenConst->getLexeme()) >= tokenId->getMaxTam()){
        throw LException(ErrorCode::IDENTIFIER_NO_DECLARED, la->getCurrentLine(), "15");
    }
}


#endif