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

SemanticAnalysis::SemanticAnalysis(LexerAnalysis *la){
    this->la = la;

}


void SemanticAnalysis::defineNewType(Token *t, TokenType typeToken) {
    t->setTokenType(typeToken);
}


void SemanticAnalysis::isTokenHasDeclarad(Token *t, TokenClass classToken ) {
    if(t->getTokenClass()  == TOKEN_CLASS_UNDEFINED){
        t->setTokenClass(classToken);
    }
    else{
        throw LException(ErrorCode::IDENTIFIERALREADYDECLARED, la->getCurrentLine() , "");
    }
}

void SemanticAnalysis::isTokenNotHasDeclarationAndNotHasConst(Token *t) {

    if(t->getTokenClass()  == TOKEN_CLASS_UNDEFINED){
        throw LException(ErrorCode::IDENTIFIERNODECLARED, la->getCurrentLine() , "");
    }
    if(t->getTokenClass()  == TOKEN_CLASS_CONSTANT){
        throw LException(ErrorCode::MISMATCHEDIDENTIFIERCLASS, la->getCurrentLine() , "");
    }   
}


void SemanticAnalysis::isTokenNotHasDeclaration(Token *t) {

    if(t->getTokenClass()  == TOKEN_CLASS_UNDEFINED){
        throw LException(ErrorCode::IDENTIFIERNODECLARED, la->getCurrentLine() , "");
    }
  
}



void SemanticAnalysis::isTokenTypeEquals(Token *tokenConst, Token *tokenId, bool isneg){
    if(isneg && ( tokenConst->getTokenType() != TOKEN_TYPE_INTEGER && tokenConst->getTokenType() != TOKEN_TYPE_REAL)){
        throw LException(ErrorCode::INCOMPATIBLETYPES, la->getCurrentLine(), "");
    }
    if(tokenId->getTokenType()  == TOKEN_TYPE_INTEGER && tokenConst->getTokenType() != TOKEN_TYPE_INTEGER){
        throw LException(ErrorCode::INCOMPATIBLETYPES, la->getCurrentLine(), "");
    }
    else{
       // tokenIdSetClass(tokenConst, tokenId);
        tokenIdSetType(tokenConst, tokenId);
    }
}

void SemanticAnalysis::tokenIdSetType(Token *tokenConst, Token *tokenId){
    tokenId->setTokenType(tokenConst->getTokenType());

}

void SemanticAnalysis::tokenIdSetClass(Token *tokenConst, Token *tokenId){
        tokenId->setTokenClass(tokenConst->getTokenClass());
}

/*
[6] ->
{
       	se const.tipo != inteiro 
		erro
]
*/
void SemanticAnalysis::ifTokenTypehasDiff(Token *token, TokenType typeToken){
    if(token->getTokenType() != typeToken){
       throw LException(ErrorCode::INCOMPATIBLETYPES, la->getCurrentLine(), "");
    }
}



void SemanticAnalysis::ifTokenTypeHasEqualsorIntandReal(Token *token, TokenType typeToken){
    if (token->getTokenType() != typeToken && (token->getTokenType() != TOKEN_TYPE_REAL && typeToken != TOKEN_TYPE_INTEGER))
    {
        throw LException(ErrorCode::INCOMPATIBLETYPES, la->getCurrentLine(), "");
    }
    else
    {
        // Comando redunfdante
    }
}

void SemanticAnalysis::TokenReplaceType(Token *token){
    if (token->getTokenType() == TOKEN_TYPE_REAL)
    {
        token->setTokenType(TOKEN_TYPE_INTEGER);
    } else if(token->getTokenType() == TOKEN_TYPE_INTEGER){
        token->setTokenType(TOKEN_TYPE_REAL);
    }
    else
    {
       throw LException(ErrorCode::INCOMPATIBLETYPES, la->getCurrentLine(), "");
    }
}

void SemanticAnalysis::tokenIsBoolean(Token *token, bool isNeg){
    if(token->getTokenType() != TOKEN_TYPE_BOOLEAN && isNeg){
        throw LException(ErrorCode::INCOMPATIBLETYPES, la->getCurrentLine(), "");
    }
}

void SemanticAnalysis::tokenIsMultDivAndMod(Token *token){

// Fazer algo

}

void SemanticAnalysis::tokenIsIntergerOrReal(Token *token, bool isNeg){
    if(isNeg && token->getTokenType() != TOKEN_TYPE_INTEGER  && token->getTokenType() != TOKEN_TYPE_REAL){
         throw LException(ErrorCode::INCOMPATIBLETYPES, la->getCurrentLine(), "");
    }
}







/*
* @brief: Caso o token Class nao seja variavel erro.
* Exemplo:
* Final a = 2;
* a = 5;
void SemanticAnalysis::isTokenClassEquals(Token& token ){
    if(token.getTokenClass() != TOKEN_CLASS_VARIABLE){
        throw LException(ErrorCode::MISMATCHEDIDENTIFIERCLASS, la->getCurrentLine(), "");
    }

}
*/

