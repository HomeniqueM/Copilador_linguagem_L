#ifndef SYMBOLS_TOKEN
#define SYMBOLS_TOKEN
#include <string>
#include "token-id.cpp"
#include "token-class.cpp"
#include "token-type.cpp"


class Token
{
private:
    std::string _lexeme;
    TokenID _id;
    TokenType _tokenType;

public:
    Token(TokenID id = TOKEN_ID_NULL, TokenClass tokenClass = TOKEN_CLASS_UNDEFINED, TokenType tokenType = TOKEN_TYPE_UNDEFINED, std::string lexeme = "") : _id(id), _tokenClass(tokenClass), _tokenType(tokenType), _lexeme(lexeme){};

    void setTokenID(TokenID id);
    void setLexeme(std::string lexeme);
    void setTokenType(TokenType tokenType);

    TokenID getTokenid();
    TokenType getTokenType();
    std::string getLexeme();
};

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


#endif