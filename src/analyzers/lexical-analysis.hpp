/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
*/
#pragma once


#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <algorithm>

#include "../symbols/token.hpp"
#include "../symbols/symbol-table.hpp"
#include "../utils/file.hpp"

extern int currentLine;

/** =========================================================
 *
 * @brief Pacote de retorno para cada estado
 *
 * ========================================================== */
typedef struct StatePackage
{
    bool returnChar = false; // deve voltar um char
    std::string identifier;  // onde o lexima é montado
    TokenType tokenType = TOKEN_TYPE_UNDEFINED;
    TokenID tokenId = TOKEN_ID_NULL;
    
} StatePackage;

/** =========================================================
 *
 * @brief classe abstrata para geração de estados
 *
 * ========================================================== */

class State
{
public:
    virtual StatePackage handle(char c) = 0;
    virtual bool isComplete() { return completed; };
    std::shared_ptr<State> nextState;

protected:
    bool completed = false;
};


/** =========================================================
 * @brief Represanta o estado (S0)
 * @details Estado inicial
 * @category Primeiro está
 * @param c caracteres a ser analisado
 * ========================================================== */
class StartState : public State
{
public:
    StatePackage handle(char c) override;
};

/** =========================================================
 * @brief Represanta o estado (S1)
 * @details Estado final
 * @category Criação do token
 * @param c 
 * ========================================================== */
class State01
{
public:
    Token *makeAToken(SymbolTable *st, std::string lexeme, TokenType tokentype, TokenID tokenId, int currentLine);

private:
    std::string substringAfterDelimiter(std::string str, char delimiter);
    size_t getTokenSize(TokenType tokenType, std::string lexeme);
};


/** =========================================================
 * @brief Represanta o estado (S3)
 * @details State para trata idetificadores
 * @category State para tratar Identificador
 * @param c caracteres a ser analisado
 * ========================================================== */
class State03 : public State
{
public:
    StatePackage handle(char c) override;
};

/** =========================================================
 * @brief Represanta o estado (S4)
 * @details State para tratar idetificadores 
 * @category 
 * @param c caracteres a ser analisado
 * ========================================================== */
class State04 : public State
{
public:
    StatePackage handle(char c) override;
};

/** =========================================================
 * @brief Represanta o estado (S5)
 * @details State para tratar idetificadores
 * @category 
 * @param c caracteres a ser analisado
 * ========================================================== */
class State05 : public State
{
public:
    StatePackage handle(char c) override;
};

/** =========================================================
 * @brief Represanta o estado (S6)
 * @details Estado final do hexadecimal
 * @category 
 * @param c caracteres a ser analisado
 * ========================================================== */
class State06 : public State
{
public:
    StatePackage handle(char c) override;
};

/** =========================================================
 * @brief Represanta o estado (S7)
 * @details Estado para tratar hexadecimal no estado inicial
 * @category 
 * @param c caracteres a ser analisado
 * ========================================================== */
class State07 : public State
{
public:
    StatePackage handle(char c) override;
};

/** =========================================================
 * @brief Represanta o estado (S8)
 * @details Estado para tratar estado intermediario hexadecimal 
 * @category 
 * @param c caracteres a ser analisado
 * ========================================================== */
class State08 : public State
{
public:
    StatePackage handle(char c) override;
};
/** =========================================================
 * @brief Represanta o estado (S9)
 * @details Estado para tratar estado intermediario hexadecimal 
 * @category 
 * @param c caracteres a ser analisado
 * ========================================================== */
class State09 : public State
{
public:
    StatePackage handle(char c) override;
};
/** =========================================================
 * @brief Represanta o estado (S10)
 * @details Estado para tratar estado final hexadecimal 
 * @category 
 * @param c caracteres a ser analisado
 * ========================================================== */
class State11 : public State
{
public:
    StatePackage handle(char c) override;
};
/** =========================================================
 * @brief Represanta o estado (S12)
 * @details  Transição para tratar de valores inteiros 
 * @category 
 * @param c caracteres a ser analisado
 * ========================================================== */
class State12 : public State
{
public:
    StatePackage handle(char c) override;
};

/** =========================================================
 * @brief Represanta o estado (S13)
 * @details Transição para tratar de valores reais 
 * @category 
 * @param c caracteres a ser analisado
 * ========================================================== */
class State13 : public State
{
public:
    StatePackage handle(char c) override;
};

/** =========================================================
 * @brief  Represanta o estado (S14)
 * @details Transição par ler valores reias 
 * @category 
 * @param c caracteres a ser analisado
 * ========================================================== */
class State14 : public State
{
public:
    StatePackage handle(char c) override;
};

/** =========================================================
 * @brief Represanta o estado (S15)
 * @details Transição par ler numero exponential 
 * @category 
 * @param c caracteres a ser analisado
 * ========================================================== */
class State15 : public State
{
public:
    StatePackage handle(char c) override;
};
/** =========================================================
 * @brief Represanta o estado (S16)
 * @details Transição par ler valores numero até encontrar 
 * algo diferente de um valor numerico 
 * @category 
 * @param c caracteres a ser analisado
 * ========================================================== */
class State16 : public State
{
public:
    StatePackage handle(char c) override;
};
/** =========================================================
 * @brief Represanta o estado (S17)
 * @details Tratar o valores de < | <= | <>
 * @category 
 * @param c caracteres a ser analisado
 * ========================================================== */
class State17 : public State
{
public:
    StatePackage handle(char c) override;
};
/** =========================================================
 * @brief Represanta o estado (S18)
 * @details Tratar o valores de = | == 
 * @category 
 * @param c caracteres a ser analisado
 * ========================================================== */
class State18 : public State
{
public:
    StatePackage handle(char c) override;
};
/** =========================================================
 * @brief Represanta o estado (S19)
 * @details Tratar o valores char
 * @category 
 * @param c caracteres a ser analisado
 * ========================================================== */
class State19 : public State
{
public:
    StatePackage handle(char c) override;
};
/** =========================================================
 * @brief Represanta o estado (S20)
 * @details Tratar o valores char
 * @category 
 * @param c caracteres a ser analisado
 * ========================================================== */
class State20 : public State
{
public:
    StatePackage handle(char c) override;
};
/** =========================================================
 * @brief Represanta o estado (S21)
 * @details Tratar o Strings
 * @category 
 * @param c caracteres a ser analisado
 * ========================================================== */
class State21 : public State
{
public:
    StatePackage handle(char c) override;
};
/** =========================================================
 * @brief Represanta o estado (S22)
 * @details Trata o valores > | >=
 * @category 
 * @param c caracteres a ser analisado
 * ========================================================== */
class State22 : public State
{
public:
    StatePackage handle(char c) override;
};
/** =========================================================
 * @brief 
 * @details 
 * @category 
 * @param c caracteres a ser analisado
 * ========================================================== */
class CommentState : public State
{
public:
    StatePackage handle(char c) override;
};
/** =========================================================
 * @brief Classe principal do Análise léxica
 * @details 
 * @category 
 * ========================================================== */

class LexerAnalysis
{
private:
    FileHandler *fh;
    std::shared_ptr<State> currentState;
    SymbolTable *symboltable;

public:
    LexerAnalysis(FileHandler *fh, SymbolTable *symboltable);
    Token *getNextToken();
    void pushBackCurrentChar();
    char getNextChar();
    int getCurrentLine();
    Token *clone();
};

