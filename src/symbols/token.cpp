#ifndef SYMBOL_TOKEN
#define SYMBOL_TOKEN
#include <string>
#include "token-id.cpp"

class Token
{
private:
private:
    TokenID _id;
    std::string lexema;
    // lexema
    // tipo

public:
    Token(TokenID id = TOKEN_ID_NULL);
    ~Token();

    void setTokenID(TokenID id);
    TokenID get_token_id();
};

Token::Token(TokenID id)
{
    this->_id = id;
}

Token::~Token()
{
}

void Token::setTokenID(TokenID id)
{
    this->_id = id;
}

TokenID Token::get_token_id()
{
    return _id;
}
#endif