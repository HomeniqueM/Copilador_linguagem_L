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
    int _tokenSize;

public:
    Token(TokenID id = TOKEN_ID_NULL, TokenType tokenType = TOKEN_TYPE_UNDEFINED, std::string lexeme = "", int tokenSize = 0)
    {
        this->_id = id;
        this->_tokenType = tokenType;
        this->_lexeme = lexeme;
        this->_tokenSize = tokenSize;
    };

    void setTokenID(TokenID id);
    void setLexeme(std::string lexeme);
    void setTokenType(TokenType tokenType);
    void setTokenSize(int tokenSize);

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