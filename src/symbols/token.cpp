/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
*/
#include "token.hpp"

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
long Token::setTokenAddr(long addr)
{
    this->_addr = addr;
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

long Token::getTokenAddr()
{
    return _addr;
}


