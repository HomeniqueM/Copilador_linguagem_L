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
    TokenType tokenType = TOKEN_TYPE_UNDEFINED;
    bool erro = false;
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
// Estado final do analisador léxico
class FinalState : public State
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

    StatePackage package;

    if (isASpaceOrLineBreak(c))
    {
        nextState = std::make_shared<StartState>();
    }
    if (isAValidUnitarySymbol(c))
    {
        std::cout << "isAValidUnitarySymbol: ";
        package.identifier = +c;
        package.tokenType = TOKEN_TYPE_CHAR;
        this->completed = true;
        nextState = std::make_shared<StartState>();
    }
    else if (c == '{')
    {
        std::cout << "Comentario: ";
        nextState = std::make_shared<CommentState>();
        std::cout << std::endl;
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

    else{
        package.erro = true;
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

        std::cout << "\nToken encontrado: " << lexeme << std::endl;
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

        LexerAnalysis la("${aabb1111cc}{aassadad}");

        la.getNextToken();
    }
    catch (const LException &e)
    {

        std::cerr << e.what() << std::endl;
    }

    return 0;
}
