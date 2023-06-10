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
#include "token-id.hpp"
#include "token-class.hpp"
#include "token-type.hpp"

class Token
{
private:
    std::string _lexeme;
    TokenID _id;
    TokenType _tokenType;
    size_t _tokenSize;
    long _addr;

public:
    Token(TokenID id = TOKEN_ID_NULL, TokenType tokenType = TOKEN_TYPE_UNDEFINED, std::string lexeme = "", size_t tokenSize = 0)
    {
        this->_id = id;
        this->_tokenType = tokenType;
        this->_lexeme = lexeme;
        this->_tokenSize = tokenSize;
    };

    void setTokenID(TokenID id);
    void setLexeme(std::string lexeme);
    void setTokenType(TokenType tokenType);
    void setTokenSize(size_t tokenSize);
    void setTokenAddr(long addr);

    TokenID getTokenid();
    TokenType getTokenType();
    std::string getLexeme();
    size_t getTokeSize();
    long getTokenAddr();
};

#endif