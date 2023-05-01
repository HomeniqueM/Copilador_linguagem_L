/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
*/
#ifndef ANALYZERS_SYNTATIC_ANALYSIS
#define ANALYZERS_SYNTATIC_ANALYSIS
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <algorithm>
#include "../symbols/token.cpp"
#include "../error/l_exception.cpp"
#include "../symbols/token-type.cpp"
#include "../analyzers/lexical-analysis.cpp"

class SyntaticAnalysis
{
private:
    LexerAnalysis *la;
    Token *token;
    void setToken(Token *token);
    void matchToken(TokenID expectedToken);
    void productionS();
    void productionD();
    void productionD1();
    void productionC();
    void productionCMD();
    void productionCMD1();
    void productionA();
    void productionR();
    void productionR1();
    void productionT();
    void productionT1();
    void productionL();
    void productionE();
    void productionE1();
    void productionExp();
    void productionExp1();
    void productionExp2();
    void productionExp3();
    void productionExp4();
    void productionExp5();

public:
    SyntaticAnalysis(LexerAnalysis *la);
    void Start(Token *token);
};

SyntaticAnalysis::SyntaticAnalysis(LexerAnalysis *la)
{
    this->la = la;
}
/**
 * @brief: Inicia o Analizador Sintatico.
 * @param Token inicial a ser analisado
 */

void SyntaticAnalysis::Start(Token *token)
{
    setToken(token);
    productionS();
}

void SyntaticAnalysis::setToken(Token *token)
{
    this->token = token;
}

/**
 * @brief: Realiza o casamento do Token Experado pelo Token Encontrado.
 */
void SyntaticAnalysis::matchToken(TokenID expectedToken)
{
    // std::cout << "Token Esperado :" << tokenToString(expectedToken) << expectedToken << std::endl;
    // std::cout << "Token Encontrado :" << tokenToString(token->getTokenid()) << token->getTokenid() << std::endl;
    // Fazer o match
    if (expectedToken == token->getTokenid())
    {
        // Pedir o prx token
        setToken(la->getNextToken());
        // std::cout << "Proximo Token:" << tokenToString(token->getTokenid()) << token->getTokenid() << std::endl;
    }
    else
    {
        if (token->getTokenid() == TOKEN_ID_EOF)
        {
            throw LException(ErrorCode::UNEXPECTED_TOKEN_EOF, la->getCurrentLine() + 1, "");
        }
        else
        {
            throw LException(ErrorCode::UNEXPECTED_TOKEN, la->getCurrentLine() + 1, token->getLexeme());
        }
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 *         S -> { ( D | Cmd) } fim_arquivo
 */
void SyntaticAnalysis::productionS()
{
    while (token->getTokenid() != TOKEN_ID_EOF)
    {
        bool declaration =
            token->getTokenid() == TOKEN_ID_FINAL ||
            token->getTokenid() == TOKEN_ID_INTEGER ||
            token->getTokenid() == TOKEN_ID_CHAR ||
            token->getTokenid() == TOKEN_ID_REAL ||
            token->getTokenid() == TOKEN_ID_STRING ||
            token->getTokenid() == TOKEN_ID_BOOLEAN;

        if (declaration)
        {
            productionD();
        }
        else
        {
            productionCMD();
        }
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 *         D -> D1 C { ,C } ; | final id = [-] const ;
 */
void SyntaticAnalysis::productionD()
{
    if (token->getTokenid() == TOKEN_ID_FINAL)
    {
        matchToken(TOKEN_ID_FINAL);
        matchToken(TOKEN_ID_IDENTIFIER);
        matchToken(TOKEN_ID_ASSIGNMENT);
        if (token->getTokenid() == TOKEN_ID_SUBTRACTION)
        {
            matchToken(TOKEN_ID_SUBTRACTION);
        }
        matchToken(TOKEN_ID_CONSTANT);
    }
    else
    {
        productionD1();
        productionC();
        while (token->getTokenid() == TOKEN_ID_COMMA)
        {
            matchToken(TOKEN_ID_COMMA);
            productionC();
        }
    }
    matchToken(TOKEN_ID_SEMICOLON);
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * D1 -> ( char | integer | real | boolean | string )
 */
void SyntaticAnalysis::productionD1()
{

    switch (token->getTokenid())
    {
    case TOKEN_ID_CHAR:
        matchToken(TOKEN_ID_CHAR);
        break;
    case TOKEN_ID_INTEGER:
        matchToken(TOKEN_ID_INTEGER);
        break;
    case TOKEN_ID_REAL:
        matchToken(TOKEN_ID_REAL);
        break;
    case TOKEN_ID_BOOLEAN:
        matchToken(TOKEN_ID_BOOLEAN);
    case TOKEN_ID_STRING:
        matchToken(TOKEN_ID_STRING);
        break;
    default:
        std::cout << "ERRO NÃO SEI COMO SE CONSEGUIU ESSA PROESA.";
        break;
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * C -> id [ ( = [ - ] const | [ ( id | const) ] ) ]
 */
void SyntaticAnalysis::productionC()
{
    matchToken(TOKEN_ID_IDENTIFIER);
    if (token->getTokenid() == TOKEN_ID_ASSIGNMENT)
    {
        matchToken(TOKEN_ID_ASSIGNMENT);
        if (token->getTokenid() == TOKEN_ID_SUBTRACTION)
        {
            matchToken(TOKEN_ID_SUBTRACTION);
        }
        matchToken(TOKEN_ID_CONSTANT);
    }
    else if (token->getTokenid() == TOKEN_ID_OPEN_BRACKET)
    {
        matchToken(TOKEN_ID_OPEN_BRACKET);
        if (token->getTokenid() == TOKEN_ID_IDENTIFIER)
        {
            matchToken(TOKEN_ID_IDENTIFIER);
        }
        else
        {
            matchToken(TOKEN_ID_CONSTANT);
        }

        matchToken(TOKEN_ID_CLOSE_BRACKET);
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * Cmd -> ( ( [ ( A | L | E ) ] ; ) | ( R | T ) )
 */
void SyntaticAnalysis::productionCMD()
{
    if (token->getTokenid() == TOKEN_ID_IDENTIFIER)
    {
        productionA();
        matchToken(TOKEN_ID_SEMICOLON);
    }
    else if (token->getTokenid() == TOKEN_ID_FOR)
    {
        productionR();
    }
    else if (token->getTokenid() == TOKEN_ID_IF)
    {
        productionT();
    }
    else if (token->getTokenid() == TOKEN_ID_READLN)
    {
        productionL();
        matchToken(TOKEN_ID_SEMICOLON);
    }
    else
    {
        productionE();
        matchToken(TOKEN_ID_SEMICOLON);
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * Cmd1 -> [ ( A | R | T | L | E ) ]
 */
void SyntaticAnalysis::productionCMD1()
{
    if (token->getTokenid() == TOKEN_ID_IDENTIFIER)
    {
        productionA();
    }
    else if (token->getTokenid() == TOKEN_ID_FOR)
    {
        productionR();
    }
    else if (token->getTokenid() == TOKEN_ID_IF)
    {
        productionT();
    }
    else if (token->getTokenid() == TOKEN_ID_READLN)
    {
        productionL();
    }
    else
    {
        productionE();
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * A ->  id = ( Exp | L )
 */
void SyntaticAnalysis::productionA()
{
    matchToken(TOKEN_ID_IDENTIFIER);
    if (token->getTokenid() == TOKEN_ID_OPEN_BRACKET)
    {
        matchToken(TOKEN_ID_OPEN_BRACKET);
        if (token->getTokenid() == TOKEN_ID_IDENTIFIER)
        {
            matchToken(TOKEN_ID_IDENTIFIER);
        }
        else
        {
            matchToken(TOKEN_ID_CONSTANT);
        }

        matchToken(TOKEN_ID_CLOSE_BRACKET);
    }
    matchToken(TOKEN_ID_ASSIGNMENT);
    if (token->getTokenid() == TOKEN_ID_READLN)
    {
        productionL();
    }
    productionExp();
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * R -> for (  R1 ;  Exp ;  R1  ) T1

*/
void SyntaticAnalysis::productionR()
{
    matchToken(TOKEN_ID_FOR);
    matchToken(TOKEN_ID_OPEN_PARANTHESES);
    productionR1();
    matchToken(TOKEN_ID_SEMICOLON);
    productionExp();
    matchToken(TOKEN_ID_SEMICOLON);
    productionR1();
    matchToken(TOKEN_ID_CLOSE_PARANTHESES);
    productionT1();
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * R1-> Cmd1 { ,Cmd1 }

*/
void SyntaticAnalysis::productionR1()
{
    productionCMD1();
    while (token->getTokenid() == TOKEN_ID_COMMA)
    {
        matchToken(TOKEN_ID_COMMA);
        productionCMD1();
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * T -> if ( Exp ) T1 [ else T1 ]
 */
void SyntaticAnalysis::productionT()
{
    matchToken(TOKEN_ID_IF);
    matchToken(TOKEN_ID_OPEN_PARANTHESES);
    productionExp();
    matchToken(TOKEN_ID_CLOSE_PARANTHESES);
    productionT1();
    if (token->getTokenid() == TOKEN_ID_ELSE)
    {
        matchToken(TOKEN_ID_ELSE);
        productionT1();
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * T1 -> ( Cmd | begin { Cmd } end )
 */
void SyntaticAnalysis::productionT1()
{
    if (token->getTokenid() == TOKEN_ID_BEGIN)
    {
        matchToken(TOKEN_ID_BEGIN);
        while (token->getTokenid() != TOKEN_ID_END)
        {
            productionCMD();
        }
        matchToken(TOKEN_ID_END);
    }
    else
    {
        productionCMD();
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * L -> readln ( id )
 */
void SyntaticAnalysis::productionL()
{
    matchToken(TOKEN_ID_READLN);
    matchToken(TOKEN_ID_OPEN_PARANTHESES);
    matchToken(TOKEN_ID_IDENTIFIER);
    matchToken(TOKEN_ID_CLOSE_PARANTHESES);
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * E -> ( write | writeln ) ( E1 )
 */
void SyntaticAnalysis::productionE()
{
    if (token->getTokenid() == TOKEN_ID_WRITE)
    {
        matchToken(TOKEN_ID_WRITE);
    }
    else
    {
        matchToken(TOKEN_ID_WRITELN);
    }
    matchToken(TOKEN_ID_OPEN_PARANTHESES);
    productionE1();
    matchToken(TOKEN_ID_CLOSE_PARANTHESES);
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * E1 -> Exp { , Exp }
 */
void SyntaticAnalysis::productionE1()
{
    productionExp();
    while (token->getTokenid() == TOKEN_ID_COMMA)
    {
        matchToken(TOKEN_ID_COMMA);
        productionExp();
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * Exp -> Exp1 { ( == | < | <= | > | >= ) Exp1 }
 */
void SyntaticAnalysis::productionExp()
{
    productionExp1();
    while (token->getTokenid() == TOKEN_ID_ENQUALS || token->getTokenid() == TOKEN_ID_GREATER_THEN || token->getTokenid() == TOKEN_ID_GREATER_EQUAL_TO || token->getTokenid() == TOKEN_ID_LESS_THAN || token->getTokenid() == TOKEN_ID_LESS_EQUAL_TO)
    {
        if (token->getTokenid() == TOKEN_ID_ENQUALS)
        {
            matchToken(TOKEN_ID_ENQUALS);
        }
        else if (token->getTokenid() == TOKEN_ID_GREATER_THEN)
        {
            matchToken(TOKEN_ID_GREATER_THEN);
        }
        else if (token->getTokenid() == TOKEN_ID_GREATER_EQUAL_TO)
        {
            matchToken(TOKEN_ID_GREATER_EQUAL_TO);
        }
        else if (token->getTokenid() == TOKEN_ID_LESS_THAN)
        {
            matchToken(TOKEN_ID_LESS_THAN);
        }
        else
        {
            matchToken(TOKEN_ID_LESS_EQUAL_TO);
        }
        productionExp1();
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * Exp1 -> [ - ] Exp2 { ( + | - | or ) Exp2 }
 */
void SyntaticAnalysis::productionExp1()
{
    if (token->getTokenid() == TOKEN_ID_SUBTRACTION)
    {
        matchToken(TOKEN_ID_SUBTRACTION);
    }
    productionExp2();

    while (token->getTokenid() == TOKEN_ID_ADDITION || token->getTokenid() == TOKEN_ID_SUBTRACTION || token->getTokenid() == TOKEN_ID_OR)
    {
        if (token->getTokenid() == TOKEN_ID_ADDITION)
            matchToken(TOKEN_ID_ADDITION);
        else if (token->getTokenid() == TOKEN_ID_SUBTRACTION)
            matchToken(TOKEN_ID_SUBTRACTION);
        else
            matchToken(TOKEN_ID_OR);
        productionExp2();
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * Exp2 -> Exp3 { ( *  | mod | (div |/)| and ) Exp3}
 */
void SyntaticAnalysis::productionExp2()
{
    productionExp3();
    while (token->getTokenid() == TOKEN_ID_MULTIPLICATION || token->getTokenid() == TOKEN_ID_DIVISION || token->getTokenid() == TOKEN_ID_MODULO || token->getTokenid() == TOKEN_ID_AND)
    {
        if (token->getTokenid() == TOKEN_ID_MULTIPLICATION)
            matchToken(TOKEN_ID_MULTIPLICATION);
        else if (token->getTokenid() == TOKEN_ID_DIVISION)
            matchToken(TOKEN_ID_DIVISION);
        else if (token->getTokenid() == TOKEN_ID_MODULO)
            matchToken(TOKEN_ID_MODULO);
        else
            matchToken(TOKEN_ID_AND);
        productionExp3();
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * Exp3 -> Exp4 | not Exp4
 */
void SyntaticAnalysis::productionExp3()
{
    if (token->getTokenid() == TOKEN_ID_NOT)
    {
        matchToken(TOKEN_ID_NOT);
    }
    productionExp4();
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * Exp4 -> Exp5 | real( Exp5 ) | integer( Exp5 )
 */
void SyntaticAnalysis::productionExp4()
{
    if (token->getTokenid() != TOKEN_ID_INTEGER && token->getTokenid() != TOKEN_ID_REAL)
    {
        productionExp5();
    }
    else
    {
        if (token->getTokenid() == TOKEN_ID_INTEGER)
        {
            matchToken(TOKEN_ID_INTEGER);
        }
        else
        {
            matchToken(TOKEN_ID_REAL);
        }
        matchToken(TOKEN_ID_OPEN_PARANTHESES);
        productionExp5();
        matchToken(TOKEN_ID_CLOSE_PARANTHESES);
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * Exp5 -> const | id | ( Exp )
 */
void SyntaticAnalysis::productionExp5()
{
    if (token->getTokenid() == TOKEN_ID_CONSTANT)
        matchToken(TOKEN_ID_CONSTANT);
    else if (token->getTokenid() == TOKEN_ID_IDENTIFIER)
    {
        matchToken(TOKEN_ID_IDENTIFIER);
        if (token->getTokenid() == TOKEN_ID_OPEN_BRACKET)
        {
            matchToken(TOKEN_ID_OPEN_BRACKET);
            if (token->getTokenid() == TOKEN_ID_IDENTIFIER)
            {
                matchToken(TOKEN_ID_IDENTIFIER);
            }
            else
            {
                matchToken(TOKEN_ID_CONSTANT);
            }
            matchToken(TOKEN_ID_CLOSE_BRACKET);
        }
    }
    else
    {
        matchToken(TOKEN_ID_OPEN_PARANTHESES);
        productionExp();
        matchToken(TOKEN_ID_CLOSE_PARANTHESES);
    }
}

#endif