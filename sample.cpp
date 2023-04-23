#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>

typedef struct StatePackage
{
    bool returnChar = false;
    std::string newLexeme = "";
    //TokenClass
    //TokenType
} StatePackage;


class State
{
public:
    virtual StatePackage handle(char c) = 0;
    virtual bool isComplete() { return completed; };
    std::shared_ptr<State> nextState;

protected:

    bool completed = false;
};


class IdentifierState : public State
{
public:

    StatePackage handle(char c) override
    {
        StatePackage package;

        std::cout << "IdentifierState" << std::endl;
        if (isalpha(c) || isdigit(c) || c == '_')
        {
           // nextState = std::make_shared<StartState>();

        }
        else
        {
            completed = true;
        }

        return package;
    }

};

class StartState : public State
{
public:
    StatePackage handle(char c) override
    {

        StatePackage package;

        if (c == '_' || (c >= 'G' && c <= 'Z'))
        {

            completed = false;
            nextState = std::make_shared<IdentifierState>();
        }
        else if (isdigit(c))
        {

            completed = false;
            //token_class = TokenClassConstante;
            //nextState = std::make_shared<IdentifierState>();
        }

        else
        {
            completed = true;
        }

        return package;
    }
};

class Lexer
{
public:
    Lexer(std::string file);
    void returnChar();
    char nextChar();
    bool isEndFile();
    std::string handle();

private:
    std::string file;
    std::vector<std::string> tokens;
    int file_point;
    std::shared_ptr<State> currentState;
};

Lexer::Lexer(std::string file) : currentState(std::make_shared<StartState>())
{
    this->file = file;
    file_point = 0;
}

std::string Lexer::handle() // get next token 
{
    std::string lexeme = "";

    // Validar se é um char valido

    while (!currentState->isComplete())
    {

        StatePackage package = currentState->handle(nextChar());
        
        if (package.returnChar) {
            returnChar();
        }

        lexeme += package.newLexeme;

        currentState = currentState->nextState;
    }

    //Token t = Token(lexema);

    // adicionar o token 
    // tabela_simbolos.tratar_token(t);

    // Token class
        // verificar se e eh constante 
        // verificar se e palavra reservada
        // se nao for nenhum dos dois entao ele eh identificador 

    //return t;
    return lexeme;
}

void Lexer::returnChar()
{
    this->file_point--;
}

char Lexer::nextChar()
{
    if (isEndFile())
    {
        return '\0';
    }

    return this->file[this->file_point++];
}

bool Lexer::isEndFile()
{
    return this->file_point < this->file.size();
}

int main()
{
    std::string input = "G1234 _hello Zworld 1234";
    Lexer lexer(input);

    // for (const auto &token : lexer.getTokens())
    //{
       std::cout << "Token: " << lexer.handle() << std::endl;
    // }
    return 0;
}