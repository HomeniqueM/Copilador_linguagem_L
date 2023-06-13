/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
 */
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
#include "../analyzers/semantic-analysis.hpp"

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
        throw LException(ErrorCode::IDENTIFIER_ALREADY_DECLARED, la->getCurrentLine(), t->getLexeme());
    }
}

void SemanticAnalysis::isTokenNotHasDeclarationAndNotHasConst(Token *t)
{

    if (t->getTokenClass() == TOKEN_CLASS_UNDEFINED)
    {
        throw LException(ErrorCode::IDENTIFIER_NO_DECLARED, la->getCurrentLine(), t->getLexeme());
    }
    if (t->getTokenClass() == TOKEN_CLASS_CONSTANT)
    {
        throw LException(ErrorCode::MISMATCHED_IDENTIFIER_CLASS, la->getCurrentLine(), t->getLexeme());
    }
}

void SemanticAnalysis::isTokenNotHasDeclaration(Token *t)
{

    if (t->getTokenClass() == TOKEN_CLASS_UNDEFINED)
    {
        throw LException(ErrorCode::IDENTIFIER_NO_DECLARED, la->getCurrentLine(), t->getLexeme());
    }
}

void SemanticAnalysis::isTokenTypeEquals(Token *tokenConst, Token *tokenId, bool isneg)
{

    if (isneg)
    {
        if (tokenConst->getTokenType() != TOKEN_TYPE_INTEGER && tokenConst->getTokenType() != TOKEN_TYPE_REAL)
        {
            throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine());
        }
    }
    if (tokenConst->getTokenType() != tokenId->getTokenType()){ 
        if( tokenId->getTokenType() != TOKEN_TYPE_REAL &&  tokenConst->getTokenType() != TOKEN_TYPE_INTEGER ){
            throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine());
        }

    }
    else
    {
        // tokenIdSetClass(tokenConst, tokenId);
        tokenIdSetType(tokenConst, tokenId);
    }
}

void SemanticAnalysis::FinalVerify(Token *tokenConst, Token *tokenId, bool isneg){
    if (isneg)
    {
        if (tokenConst->getTokenType() != TOKEN_TYPE_INTEGER && tokenConst->getTokenType() != TOKEN_TYPE_REAL)
        {
            throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine());
        }
    }
    else{
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
        throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine());
    }
}

void SemanticAnalysis::ifTokenTypeHasEqualsorIntandReal(Token *token, TokenType typeToken)
{
    if (token->getTokenType() != typeToken && (token->getTokenType() != TOKEN_TYPE_REAL && typeToken != TOKEN_TYPE_INTEGER))
    {
        throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine());
    }
    else
    {
        // Comando redunfdante
    }
}

void SemanticAnalysis::verificacaoDeAtribuicao(Token *token, Token *tokenConst)
{
    if (token->getTokenType() != tokenConst->getTokenType())
    {
        if( !(token->getTokenType() == TOKEN_TYPE_REAL && tokenConst->getTokenType() == TOKEN_TYPE_INTEGER ) && !(token->getTokenType() == TOKEN_TYPE_CHAR && tokenConst->getTokenType() == TOKEN_TYPE_STRING) ){
            throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine());
        }
        else if( tokenConst->getTokeSize() >= token->getMaxTam() && tokenConst->getTokenType() == TOKEN_TYPE_STRING )
            throw LException(ErrorCode::IDENTIFIER_NO_DECLARED, la->getCurrentLine(), token->getLexeme());
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
        throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine());
    }
}

void SemanticAnalysis::tokenIsBoolean(Token *token, bool isNeg)
{
    if (token->getTokenType() != TOKEN_TYPE_BOOLEAN && isNeg)
    {
        throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine());
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
        throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine());
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
            throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine());
        }
    }
    else
    {
        if (tokenA->getTokenType() != tokenB->getTokenType())
        {
            if ((tokenA->getTokenType() == TOKEN_TYPE_REAL || tokenA->getTokenType() == TOKEN_TYPE_INTEGER) &&
                (tokenB->getTokenType() == TOKEN_TYPE_REAL || tokenB->getTokenType() == TOKEN_TYPE_INTEGER))
            {
                tokenA->setTokenType(TOKEN_TYPE_REAL);
            }
            else
            {
                throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine());
            }
        }
        else
        {
            if (!(tokenA->getTokenType() == TOKEN_TYPE_REAL || tokenA->getTokenType() == TOKEN_TYPE_INTEGER))
            {
                throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine());
            }
        }
    }
}

void SemanticAnalysis::rulle25(Token *tokenA, Token *tokenB, Token *operador)
{
    if (operador->getTokenid() == TOKEN_ID_AND)
    {
        if (!(tokenA->getTokenType() == TOKEN_TYPE_BOOLEAN && tokenB->getTokenType() == TOKEN_TYPE_BOOLEAN))
        {
            throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine());
        }
    }
    else if (operador->getTokenid() == TOKEN_ID_MODULO && tokenB->getTokenType() != TOKEN_TYPE_INTEGER)
    {
        // Printar token B
        throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine());
    }
    else
    {

        if (tokenA->getTokenType() != tokenB->getTokenType())
        {
            if ((tokenA->getTokenType() == TOKEN_TYPE_REAL || tokenA->getTokenType() == TOKEN_TYPE_INTEGER) &&
                (tokenB->getTokenType() == TOKEN_TYPE_REAL || tokenB->getTokenType() == TOKEN_TYPE_INTEGER))
            {
                tokenA->setTokenType(TOKEN_TYPE_REAL);
            }
            else
            {
                throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine());
            }
        }
        else
        {
            if (!(tokenA->getTokenType() == TOKEN_TYPE_REAL || tokenA->getTokenType() == TOKEN_TYPE_INTEGER))
            {
                throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine());
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
    if (tokenA->getTokenType() != tokenB->getTokenType())
    {
        if (!((tokenA->getTokenType() == TOKEN_TYPE_REAL || tokenA->getTokenType() == TOKEN_TYPE_INTEGER) &&
              (tokenB->getTokenType() == TOKEN_TYPE_REAL || tokenB->getTokenType() == TOKEN_TYPE_INTEGER)))
        {
            throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine());
        }
    }
    else
    {
        if ( tokenA->getTokenType() == TOKEN_TYPE_STRING)
        {   
            if(operador->getTokenid() != TOKEN_ID_EQUALS)
                throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine());
        }
    }
   // tokenA->setTokenType(TOKEN_TYPE_BOOLEAN);
}

void SemanticAnalysis::setMaxTamVet(Token *tokenId, Token *tokenConst)
{
    tokenId->setMaxTam(stoi(tokenConst->getLexeme()));
}

void SemanticAnalysis::ifTokenVectorInRange(Token *tokenId, Token *tokenConst)
{
    if (stoi(tokenConst->getLexeme()) >= tokenId->getMaxTam())
    {
        throw LException(ErrorCode::IDENTIFIER_NO_DECLARED, la->getCurrentLine()," " + tokenId->getLexeme() +"[" + tokenConst->getLexeme()   + "] ");
    }
}



void SemanticAnalysis::tokenIsTypeEqualsErro(Token *token, TokenType tokenType)
{
    if (token->getTokenType() == tokenType )
    {
        throw LException(ErrorCode::INCOMPATIBLE_TYPES, la->getCurrentLine());
    }
}
