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

// ==================================================================================================================================
/**
 * @brief classe para tratar do simbolo  de igual(=) [s18]
 */
class State18 : public State
{
public:
    StatePackage handle(char c) override
    {
        StatePackage package;
        if (c == '=')
        {
            package.identifier = +c;
        }

        this->completed = true;

        return package;
    }
};

/**
 * @brief classe para tratar do simbolo  de menor que(<) [s17]
 */
class State17 : public State
{
public:
    StatePackage handle(char c) override
    {
        StatePackage package;
        if (c == '=' | c == '>')
        {
            package.identifier = +c;
        }
        this->completed = true;

        return package;
    }
};

/**
 * @brief classe para tratar do simbolo  de menor que(>) [s22]
 */
class State22 : public State
{
public:
    StatePackage handle(char c) override
    {
        StatePackage package;
        if (c == '=')
        {
            package.identifier = +c;
        }
        this->completed = true;

        return package;
    }
};
/**
 * @brief classe ao receber um Apostrophe espera um caracter mesmo vazio e avança para o estado s20[s19]
 */
class State20 : public State
{
public:
    StatePackage handle(char c) override
    {
        StatePackage package;
        if (c == '\'')
        {
            package.identifier = +c;
            this->completed = true;
        }
        else
        {
            std::string msg = ": Erra esperado \' porém veio " + c;
            throw LException(ErrorCode::UNEXPECTED_CHARACTER, 0, msg);
        }
        return package;
    }
};
/**
 * @brief classe ao receber um aspas vai ler caracteres  até encontrar aspas [s21]
 */
class State21 : public State
{
public:
    StatePackage handle(char c) override
    {
        StatePackage package;
        if (c == '"')
        {
            package.identifier = +c;
            this->completed = true;
        }
        else
        {
            package.identifier = +c;
            package.tokenType = TOKEN_TYPE_STRING;
            nextState = std::make_shared<State21>();
        }

        return package;
    }
};

/**
 * @brief classe ao receber um Apostrophe espera um caracter mesmo vazio e avança para o estado s20[s19]
 */
class State19 : public State
{
public:
    StatePackage handle(char c) override
    {
        StatePackage package;

        package.identifier = +c;
        nextState = std::make_shared<State20>();

        return package;
    }
};

/**
 * @brief classe representa o primeiro estado do automato [s21]
 */
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

    if (isALexemeDelimiter(c))
    {

        nextState = std::make_shared<StartState>();
    }
    else if (isAValidUnitarySymbol(c))
    {

        package.identifier = +c;
        package.tokenType = TOKEN_TYPE_CHAR;
        this->completed = true;
        //
    }

    else if (c == '{')
    {

        nextState = std::make_shared<CommentState>();
    }
    else if (c == '<')
    {
        package.identifier = +c;
        nextState = std::make_shared<State17>();
    }
    else if (c == '=')
    {
        package.identifier = +c;
        nextState = std::make_shared<State18>();
    }
    else if (c == '\'')
    {
        package.identifier = +c;
        nextState = std::make_shared<State19>();
    }
    else if (c == '>')
    {
        package.identifier = +c;
        nextState = std::make_shared<State22>();
    }

    else if (c == '"')
    {
        package.identifier = +c;
        nextState = std::make_shared<State21>();
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

    else
    {
        // Tratativa a ser feita

        std::string str(1, c);
        throw LException(ErrorCode::UNEXPECTED_CHARACTER, __LINE__, str);
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
        currentState = std::make_shared<StartState>();
        std::string lexeme = "";
        char cc;
        while (!currentState->isComplete() || isEndFile() == true)
        {

            cc = getNextChar();

            // Pensar em alguma logica onde o

            if (symboltable->isItAValidChar(cc))
            {
                StatePackage result = currentState->handle(cc);
                if (result.returnChar)
                {
                    pushBackCurrentChar();
                }
                lexeme += result.identifier;

                if (currentState->isComplete() == false)
                {
                    currentState = currentState->nextState;
                }
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
        return token;
    };

    void pushBackCurrentChar()
    {
        file_point--;
    }
    /**
     * @brief retorna o proximo char caso o arquivo não tenha terminado caso contrario retorna \0
     */
    char getNextChar()
    {

        if (isEndFile())
        {
            return LEXEME_EOF;
        }

        return this->file[this->file_point++];
    }
    /**
     * @brief verifica se foi encontrado o fim do arquivo
     */
    bool isEndFile()
    {
        return this->file_point > this->file.size();
    }
};
int main(int argc, char const *argv[])
{
    try
    {

        LexerAnalysis la("\"> >=   \"{sdsada} <> (    )");

        la.getNextToken();
        la.getNextToken();
    }
    catch (const LException &e)
    {

        std::cerr << e.what() << std::endl;
    }

    return 0;
}
