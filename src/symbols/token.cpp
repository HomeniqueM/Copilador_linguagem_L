#ifndef SYMBOLS_TOKEN
#define SYMBOLS_TOKEN
#include <string>
#include "token-id.cpp"

class Token
{
private:
private:
    std::string _lexeme;
    TokenID _id;
    // lexema
    // tipo

public:
    Token(TokenID id = TOKEN_ID_NULL, std::string lexeme = "") : _id(id), _lexeme(lexeme){};
   

    void setTokenID(TokenID id);
    void setLexeme(std::string lexeme);

    TokenID getTokenid();

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

TokenID Token::getTokenid()
{
    return _id;
}

std::string Token::getLexeme()
{
    return _lexeme;
}

#endif