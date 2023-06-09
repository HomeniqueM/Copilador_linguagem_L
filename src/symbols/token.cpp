/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
*/
#include "token.hpp"

Token Token::clone(){
    Token a;
    a.setTokenID(this->_id);
    a.setLexeme(this->_lexeme);
    a.setTokenType(this->_tokenType);
    a.setTokenSize(this->_tokenSize); 
    a.setTokenClass(this->_tokenClass); 
    a.setMaxTam(this->maxTam);
    return a;
}

void Token::setMaxTam(int maxTam)
{
    this->maxTam = maxTam;
}
void Token::setTokenID(TokenID id)
{
    this->_id = id;
}

void Token::setLexeme(std::string lexeme)
{
    this->_lexeme = lexeme;
}

void Token::setTokenType(TokenType tokenType)
{
    this->_tokenType = tokenType;
}
void Token::setTokenSize(size_t tokenSize)
{
    this->_tokenSize = tokenSize;
}

void Token::setTokenClass(TokenClass _tokenClass)
{
    this->_tokenClass = _tokenClass;
}

int Token::getMaxTam(){
    return maxTam;
}

TokenClass Token::getTokenClass()
{
    return _tokenClass;
}

TokenID Token::getTokenid()
{
    return _id;
}

TokenType Token::getTokenType()
{
    return _tokenType;
}

std::string Token::getLexeme()
{
    return _lexeme;
}
size_t Token::getTokeSize()
{
    return _tokenSize;
}

