#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <algorithm>

#include "src/symbols/symbol-table.cpp"
#include "src/symbols/token.cpp"
#include "src/error/l_exception.cpp"

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

class CommentState : public State
{
public:
    StatePackage handle(char c)
    {
        StatePackage package;
        if (c == '}')
        {
            completed = true;
        }
        else
        {
            nextState = std::make_shared<CommentState>();
        }

        return package;
    }
};
/**
 *
 * @brief classe representa o primeiro estado do automato >s0
 */
class StartState : public State
{
public:
    StatePackage handle(char c)
    {
        StatePackage package;
        if (c == '{')
        {
        }
        else if (std::isalpha(c))
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
};

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
        char cc = getNextChar();

        if (symboltable->isItAValidChar(cc))
        {

            // while (currentState->isComplete())
            // {
            // }
        }
        else
        {
            std::string str(1, cc); 

            throw LException(ErrorCode::INVALIDCHARACTER, file_point,str);
        }
        return Token(TOKEN_ID_ADDITION);
    };

    void pushBackCurrentState()
    {
        file_point--;
    }

    char getNextChar()
    {

        if (isEndFile())
        {
            return LEXEME_EOF();
        }

        return this->file[this->file_point++];
    }

    bool isEndFile()
    {
        return this->file_point > this->file.size();
    }
};
int main(int argc, char const *argv[])
{
    try
    {

        LexerAnalysis la("$aabb1111cc");

        la.getNextToken();
    }
    catch (const LException &e)
    {

        std::cerr << e.what() << std::endl;
    }

    return 0;
}
