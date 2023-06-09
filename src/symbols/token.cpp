/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
*/
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
    TokenClass _tokenClass;
    size_t _tokenSize;
    int maxTam;


public:
    Token(TokenID id = TOKEN_ID_NULL, TokenType tokenType = TOKEN_TYPE_UNDEFINED, std::string lexeme = "", size_t tokenSize = 0)
    {
        this->_id = id;
        this->_tokenType = tokenType;
        this->_lexeme = lexeme;
        this->_tokenSize = tokenSize;
        this->_tokenClass = TOKEN_CLASS_UNDEFINED;
        this->maxTam = 0;

        
    };

    void setTokenID(TokenID id);
    void setLexeme(std::string lexeme);
    void setTokenType(TokenType tokenType);
    void setTokenSize(size_t tokenSize);
    void setTokenClass(TokenClass _tokenClass);
    void setMaxTam(int maxTam);
    int getMaxTam();
    Token clone();
    TokenClass  getTokenClass();
    TokenID getTokenid();
    TokenType getTokenType();
    std::string getLexeme();
    size_t getTokeSize();
};

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

#endif
