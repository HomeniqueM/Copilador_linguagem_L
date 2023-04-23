#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <algorithm>

#include "../symbols/symbol-table.cpp"
#include "../symbols/token.cpp"
#include "../error/l_exception.cpp"
#include "../symbols/token-type.cpp"

typedef struct StatePackage
{
    bool returnChar = false; // deve voltar um char
    std::string identifier;  // onde o lexima é montado
} StatePackage;

/**
 * @
 * @brief classe abstrata para geração de estados
 */
class State
{
public:
    virtual StatePackage handle(char c) = 0;
    virtual bool isComplete() { return completed; };
    std::shared_ptr<State> nextState;

protected:
    bool completed = false;
};
// ============================= [HEADER States] ====================================================================================
// Estado inicial do analisador léxico
class StartState : public State
{
public:
    StatePackage handle(char c) override;
};

// ==================================================================================================================================
class CommentState : public State
{
public:
    StatePackage handle(char c) override
    {
        StatePackage package;
        if (c == '}')
        {
            std::cout << "\n voltando para o StartState" << std::endl;
            nextState = std::make_shared<StartState>();
        }
        else
        {
            std::cout << c;
            nextState = std::make_shared<CommentState>();
        }

        return package;
    }
};
/**
 *
 * @brief classe representa o primeiro estado do automato [>s0]
 */

StatePackage StartState::handle(char c)
{
    std::cout << "StartState " << std::endl;

    StatePackage package;
    if (isAValidUnitarySymbol(c))
    {
    }
    else if (c == '{')
    {
        std::cout << "CommentState " << std::endl;
        nextState = std::make_shared<CommentState>();
    }

    else if (std::isalpha(c) || c == '_')
    {
        if (isItaAlphabetHexa(c))
        {
            // chamada para para estado do de possivel valor HEXA
        }
        else
        {
        }
    }
    return package;
}

/**
 * @brief Classe principal do Análise léxica
 */
class LexerAnalysis
{
private:
    std::string file;                    // String a ser tokenizada
    int file_point;                      // possição atual do char a ser tratado
    std::shared_ptr<State> currentState; // Validar necessidade
    SymbolTable *symboltable;

public:
    LexerAnalysis(std::string file) : currentState(std::make_shared<StartState>())
    {
        this->file = file;
        file_point = 0;
        symboltable = new SymbolTable();
    }
    /**
     * @brief função que retorna o proximo possivel token
     */
    Token getNextToken()
    {
        std::string lexeme = "";
        char cc;
        while (currentState->isComplete() || !isEndFile())
        {
            cc = getNextChar();

            if (symboltable->isItAValidChar(cc))
            {

                StatePackage result = currentState->handle(cc);

                if (result.returnChar)
                {
                    pushBackCurrentChar();
                }
                lexeme += result.identifier;
                currentState = currentState->nextState;
            }

            else
            {
                std::string str(1, cc);

                throw LException(ErrorCode::INVALIDCHARACTER, file_point, str);
            }
        }
        Token token = Token();
        // Token t = Token(lexema);

        // adicionar o token
        // tabela_simbolos.tratar_token(t);

        // Token class
        // verificar se e eh constante
        // verificar se e palavra reservada
        // se nao for nenhum dos dois entao ele eh identificador
        return Token(TOKEN_ID_ADDITION);
    };

    void pushBackCurrentChar()
    {
        file_point--;
    }

    char getNextChar()
    {

        if (isEndFile())
        {
            return LEXEME_EOF;
        }

        return this->file[this->file_point++];
    }

    bool isEndFile()
    {
        return this->file_point >= this->file.size();
    }
};
int main(int argc, char const *argv[])
{
    try
    {

        LexerAnalysis la("{aabb1111cc}");

        la.getNextToken();
    }
    catch (const LException &e)
    {

        std::cerr << e.what() << std::endl;
    }

    return 0;
}
