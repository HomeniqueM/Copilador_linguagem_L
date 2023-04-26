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
    Token token;
    void setToken(Token token);
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
    void productionExp6();

public:
    SyntaticAnalysis(LexerAnalysis *la);
    void Start(Token token);
};

SyntaticAnalysis::SyntaticAnalysis(LexerAnalysis *la)
{
    this->la = la;
}

void SyntaticAnalysis::Start(Token token)
{
    // Pedir um token pro amigo
    // Set token
    setToken(token);
    productionS();
}

void SyntaticAnalysis::setToken(Token token)
{
    this->token = token;
}

void SyntaticAnalysis::matchToken(TokenID expectedToken)
{
    // Fazer o match
    if (expectedToken == token.getTokenid())
    {
        // Pedir o prx token
        Token nextToken;
        nextToken.setTokenID(TOKEN_ID_EOF);
        nextToken.setLexeme("ssss");
        setToken(nextToken);
    }
    else
    {
        if (token.getTokenid() == TOKEN_ID_EOF)
        {
            throw LException(ErrorCode::UNEXPECTED_TOKEN_EOF, 0, "");
        }
        else
        {
            throw LException(ErrorCode::UNEXPECTED_TOKEN, 0, token.getLexeme());
        }
    }
}

// S -> { ( D | Cmd) } fim_arquivo
void SyntaticAnalysis::productionS()
{
    while (token.getTokenid() != TOKEN_ID_EOF)
    {
        bool declaration =
            token.getTokenid() == TOKEN_ID_FINAL ||
            token.getTokenid() == TOKEN_ID_INTEGER ||
            token.getTokenid() == TOKEN_ID_CHAR ||
            token.getTokenid() == TOKEN_ID_REAL ||
            token.getTokenid() == TOKEN_ID_STRING ||
            token.getTokenid() == TOKEN_ID_BOOLEAN;

        if (declaration)
        {
            productionD();
        }
        else
        {
            productionCMD();
        }

        this->token.setTokenID(TOKEN_ID_EOF);
    }
}

// D -> D1 C { ,C } ; | final id = [-] const ;
void SyntaticAnalysis::productionD()
{
    if (token.getTokenid() == TOKEN_ID_FINAL)
    {
        matchToken(TOKEN_ID_FINAL);
        matchToken(TOKEN_ID_IDENTIFIER);
        matchToken(TOKEN_ID_ASSIGNMENT);
        if (token.getTokenid() == TOKEN_ID_SUBTRACTION)
        {
            matchToken(TOKEN_ID_SUBTRACTION);
        }
        matchToken(TOKEN_ID_CONSTANT);
    }
    else
    {
        productionD1();
        productionC();
        while (token.getTokenid() == TOKEN_ID_COMMA)
        {
            matchToken(TOKEN_ID_COMMA);
            productionC();
        }
        matchToken(TOKEN_ID_SEMICOLON);
    }
}

// D1 -> ( char | integer | real | boolean | string )
void SyntaticAnalysis::productionD1()
{

    switch (token.getTokenid())
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

// C -> id [ ( = [ - ] const | [ ( id | num ) ] ) ]
void SyntaticAnalysis::productionC()
{
    matchToken(TOKEN_ID_IDENTIFIER);
    if (token.getTokenid() == TOKEN_ID_ASSIGNMENT)
    {
        matchToken(TOKEN_ID_ASSIGNMENT);
        if (token.getTokenid() == TOKEN_ID_SUBTRACTION)
        {
            matchToken(TOKEN_ID_SUBTRACTION);
        }
        matchToken(TOKEN_ID_CONSTANT);
    }
    else
    {
        matchToken(TOKEN_ID_OPEN_BRACKET);
        if (token.getTokenid() == TOKEN_ID_IDENTIFIER)
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

// Cmd ->  [ ( A | R | T | L | E ) ] ;
void SyntaticAnalysis::productionCMD()
{
    if (token.getTokenid() == TOKEN_ID_IDENTIFIER)
    {
        productionA();
        matchToken(TOKEN_ID_SEMICOLON);
    }
    else if (token.getTokenid() == TOKEN_ID_FOR)
    {
        productionR();
    }
    else if (token.getTokenid() == TOKEN_ID_IF)
    {
        productionT();
    }
    else if (token.getTokenid() == TOKEN_ID_READLN)
    {
        productionL();
        matchToken(TOKEN_ID_SEMICOLON);
    }
    else if (token.getTokenid() == TOKEN_ID_WRITE || token.getTokenid() == TOKEN_ID_WRITELN)
    {
        productionE();
        matchToken(TOKEN_ID_SEMICOLON);
    }

   
}

// Cmd1 ->  [ ( A | R | T | L | E ) ] ;
void SyntaticAnalysis::productionCMD1()
{
    if (token.getTokenid() == TOKEN_ID_IDENTIFIER)
    {
        productionA();
    }
    else if (token.getTokenid() == TOKEN_ID_FOR)
    {
        productionR();
    }
    else if (token.getTokenid() == TOKEN_ID_IF)
    {
        productionT();
    }
    else if (token.getTokenid() == TOKEN_ID_READLN)
    {
        productionL();
    }
    else if (token.getTokenid() == TOKEN_ID_WRITE || token.getTokenid() == TOKEN_ID_WRITELN)
    {
        productionE();
    }
}

// A ->  id = Exp
void SyntaticAnalysis::productionA()
{
    matchToken(TOKEN_ID_IDENTIFIER);
    matchToken(TOKEN_ID_ASSIGNMENT);
    productionExp();
}

// R -> for ( { R1 } ; Exp ; { R1 } ) ( [Cmd] | begin {Cmd} end )
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

// R1-> Cmd { ,Cmd }
void SyntaticAnalysis::productionR1()
{
    productionCMD1();
    while (token.getTokenid() == TOKEN_ID_COMMA)
    {
        matchToken(TOKEN_ID_COMMA);
        productionCMD1();
    }
}

// T -> if ( Exp ) T1 [ else T1 ]
void SyntaticAnalysis::productionT()
{
    matchToken(TOKEN_ID_IF);
    matchToken(TOKEN_ID_OPEN_PARANTHESES);
    productionExp();
    matchToken(TOKEN_ID_CLOSE_PARANTHESES);
    productionT1();
    if (token.getTokenid() == TOKEN_ID_ELSE)
    {
        matchToken(TOKEN_ID_ELSE);
        productionT1();
    }
}

// T1 -> ( Cmd | begin {Cmd} end )
void SyntaticAnalysis::productionT1()
{
    if (token.getTokenid() == TOKEN_ID_BEGIN)
    {
        matchToken(TOKEN_ID_BEGIN);
        while (token.getTokenid() == TOKEN_ID_END)
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

// L -> readln ( id )
void SyntaticAnalysis::productionL()
{
    matchToken(TOKEN_ID_READLN);
    matchToken(TOKEN_ID_OPEN_PARANTHESES);
    matchToken(TOKEN_ID_IDENTIFIER);
    matchToken(TOKEN_ID_CLOSE_PARANTHESES);
}

// E -> ( write( E1 ) | writeln( E1 ) )
void SyntaticAnalysis::productionE()
{
    if (token.getTokenid() == TOKEN_ID_WRITE)
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

// E1 -> Exp { , Exp }
void SyntaticAnalysis::productionE1()
{
    productionExp();
    while (token.getTokenid() == TOKEN_ID_COMMA)
    {
        matchToken(TOKEN_ID_COMMA);
        productionExp();
    }
}

// Exp -> Exp1 { ( == | < | <= | > | >= ) Exp1 }
void SyntaticAnalysis::productionExp()
{
    productionExp1();
    while (token.getTokenid() == TOKEN_ID_EQUALITY || token.getTokenid() == TOKEN_ID_GREATER_THEN || token.getTokenid() == TOKEN_ID_GREATER_EQUAL_TO || token.getTokenid() == TOKEN_ID_LESS_THAN || token.getTokenid() == TOKEN_ID_LESS_EQUAL_TO)
    {
        if (token.getTokenid() == TOKEN_ID_EQUALITY)
        {
            matchToken(TOKEN_ID_EQUALITY);
        }
        else if (token.getTokenid() == TOKEN_ID_GREATER_THEN)
        {
            matchToken(TOKEN_ID_GREATER_THEN);
        }
        else if (token.getTokenid() == TOKEN_ID_GREATER_EQUAL_TO)
        {
            matchToken(TOKEN_ID_GREATER_EQUAL_TO);
        }
        else if (token.getTokenid() == TOKEN_ID_LESS_THAN)
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

// Exp1 -> [ - ] Exp2 { ( + | - | or ) Exp2 }
void SyntaticAnalysis::productionExp1()
{
    if (token.getTokenid() == TOKEN_ID_SUBTRACTION)
    {
        matchToken(TOKEN_ID_SUBTRACTION);
    }
    productionExp2();

    while (token.getTokenid() == TOKEN_ID_ADDITION || token.getTokenid() == TOKEN_ID_SUBTRACTION || token.getTokenid() == TOKEN_ID_OR)
    {
        if (token.getTokenid() == TOKEN_ID_ADDITION)
            matchToken(TOKEN_ID_ADDITION);
        else if (token.getTokenid() == TOKEN_ID_SUBTRACTION)
            matchToken(TOKEN_ID_SUBTRACTION);
        else
            matchToken(TOKEN_ID_OR);
        productionExp2();
    }
}

// Exp2 -> Exp3 { ( *  | mod | div| and ) Exp3}
void SyntaticAnalysis::productionExp2()
{
    productionExp3();
    while (token.getTokenid() == TOKEN_ID_MULTIPLICATION || token.getTokenid() == TOKEN_ID_DIVISION || token.getTokenid() == TOKEN_ID_MODULO || token.getTokenid() == TOKEN_ID_AND)
    {
        if (token.getTokenid() == TOKEN_ID_MULTIPLICATION)
            matchToken(TOKEN_ID_MULTIPLICATION);
        else if (token.getTokenid() == TOKEN_ID_DIVISION)
            matchToken(TOKEN_ID_DIVISION);
        else if (token.getTokenid() == TOKEN_ID_MODULO)
            matchToken(TOKEN_ID_MODULO);
        else
            matchToken(TOKEN_ID_AND);
        productionExp3();
    }
}

// Exp3 -> Exp4 | not Exp4
void SyntaticAnalysis::productionExp3()
{
    if (token.getTokenid() == TOKEN_ID_NOT)
    {
        matchToken(TOKEN_ID_NOT);
    }
    productionExp4();
}

// Exp4 -> Exp5 | real(Exp5) | integer(Exp5)
void SyntaticAnalysis::productionExp4()
{
    if (token.getTokenid() != TOKEN_ID_INTEGER && token.getTokenid() != TOKEN_ID_REAL)
    {
        productionExp5();
    }
    else
    {
        if (token.getTokenid() == TOKEN_ID_INTEGER)
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

// Exp5 -> ( Exp6 )
void SyntaticAnalysis::productionExp5()
{
    matchToken(TOKEN_ID_OPEN_PARANTHESES);
    productionExp6();
    matchToken(TOKEN_ID_CLOSE_PARANTHESES);
}

// Exp6 -> const | id | Exp
void SyntaticAnalysis::productionExp6()
{
    if (token.getTokenid() == TOKEN_ID_CONSTANT)
        matchToken(TOKEN_ID_CONSTANT);
    else if (token.getTokenid() == TOKEN_ID_INTEGER)
        matchToken(TOKEN_ID_IDENTIFIER);
    else
        productionExp();
}

int main()
{

    std::cout << "Inicio do teste" << std::endl;
    SyntaticAnalysis *sintatico = new SyntaticAnalysis();
    Token *token = new Token();
    token->setTokenID(TOKEN_ID_CHAR);
    token->setLexeme("Cosso");
    sintatico->Start(*token);
}
