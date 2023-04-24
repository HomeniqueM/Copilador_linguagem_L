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
#include "../symbols/token-class.cpp"

typedef struct StatePackage
{
    bool returnChar = false; // deve voltar um char
    std::string identifier;  // onde o lexima é montado
    TokenType tokenType = TOKEN_TYPE_UNDEFINED;
    TokenClass tokenclass = TOKEN_CLASS_UNDEFINED;
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
 * ===========================================================================
 * @brief Enquanto o
 * @category State para tratar Identificador
 * ===========================================================================
 */
class State03 : public State
{
    StatePackage handle(char c) override
    {
        StatePackage package;
        if (isalpha(c) || isdigit(c) || c == '_')
        {
            package.identifier = +c;
            nextState = std::make_shared<State03>();
        }
        else
        {
            this->completed = true;
            package.returnChar = true;
        }

        return package;
    }
};

/**
 * ===========================================================================
 * @param c verifica se ele não possui nenhuma letra, digito ou underline caso
 *          não Finaliza esse estado encontra uma HEXA cas
 * @brief Estado 6 caso entre mais um caracterer é entendido que ele vai virar
 *        um indetificador
 * ===========================================================================
 */

class State06 : public State
{
    StatePackage handle(char c) override
    {
        StatePackage package;
        if (isalpha(c) || isdigit(c) || c == '_')
        {
            package.identifier = +c;
            nextState = std::make_shared<State03>();
        }
        else
        {
            package.returnChar = true;
            this->completed = true;
        }

        return package;
    }
};
/**
 * ===========================================================================
 * @param c verifica se ele não possui nenhuma letra, digito ou underline caso
 *          não Finaliza esse estado encontra uma HEXA cas
 * @brief Estado 5 caso entre mais um caracterer é entendido que ele vai virar
 *        um indetificador
 * ===========================================================================
 */
class State05 : public State
{
    StatePackage handle(char c) override
    {
        StatePackage package;
        if (c == 'h')
        {
            package.identifier = +c;
            nextState = std::make_shared<State06>();
        }
        else if (isalpha(c) || isdigit(c) || c == '_')
        {
            package.tokenclass = TOKEN_CLASS_VARIEBLE;
            package.tokenType = TOKEN_TYPE_STRING;
            package.identifier = +c;
            nextState = std::make_shared<State03>();
        }
        else
        {
            package.returnChar = true;
            this->completed = true;
        }

        return package;
    }
};
/**
 * ===========================================================================
 * @param Numerica
 * ===========================================================================
 */
class State16 : public State
{
    StatePackage handle(char c) override
    {
        StatePackage package;
        if (isdigit(c))
        {
            package.identifier = +c;
            nextState = std::make_shared<State16>();
        }
        else
        {
            package.returnChar = true;
            this->completed = true;
        }

        return package;
    }
};

class State15 : public State
{
    StatePackage handle(char c) override
    {
        StatePackage package;
        if (isdigit(c))
        {
            package.identifier = +c;
            nextState = std::make_shared<State16>();
        }
        else
        {
            std::string msg = "Erra esperando um valor numerico porém foi lido " + c;
            throw LException(ErrorCode::UNEXPECTED_CHARACTER, __LINE__, msg);
        }

        return package;
    }
};

class State14 : public State
{
    StatePackage handle(char c) override
    {
        StatePackage package;
        if (isdigit(c))
        {
            package.identifier = +c;
            nextState = std::make_shared<State16>();
        }
        else if (c == '+' || c == '-')
        {
            package.identifier = +c;
            nextState = std::make_shared<State15>();
        }
        else
        {
            std::string msg = "Erra esperando um valor numerico ou simbolo[+ ou -] porém foi lido " + c;
            throw LException(ErrorCode::UNEXPECTED_CHARACTER, __LINE__, msg);
        }

        return package;
    }
};

class State13 : public State
{
    StatePackage handle(char c) override
    {
        StatePackage package;
        if (isdigit(c))
        {
            package.identifier = +c;
            nextState = std::make_shared<State13>();
        }
        else if (c == 'e')
        {
            package.identifier = +c;
            nextState = std::make_shared<State14>();
        }
        else
        {
            package.returnChar = true;
            this->completed = true;
        }

        return package;
    }
};
class State12 : public State
{
    StatePackage handle(char c) override
    {
        StatePackage package;
        if (isdigit(c))
        {
            package.identifier = +c;
            nextState = std::make_shared<State13>();
        }
        else
        {
            std::string msg = "Erra esperando um valor numerico porém foi lido " + c;
            throw LException(ErrorCode::UNEXPECTED_CHARACTER, 0, msg);
        }

        return package;
    }
};

class State11 : public State
{
    StatePackage handle(char c) override
    {
        StatePackage package;
        if (isdigit(c))
        {
            package.identifier = +c;
            nextState = std::make_shared<State11>();
        }
        else if (c == '.')
        {
            package.identifier = +c;
            nextState = std::make_shared<State12>();
        }
        else
        {
            package.returnChar = true;
            this->completed = true;
        }

        return package;
    }
};

class State09 : public State
{
    StatePackage handle(char c) override
    {
        StatePackage package;
        if (c == 'h')
        {
            package.identifier = +c;
            this->completed = true;
        }
        else
        {
            std::string msg = "Erra esperando a letra a h porém o que foi lido foi " + c;
            throw LException(ErrorCode::UNEXPECTED_CHARACTER, 0, msg);
        }

        return package;
    }
};

class State08 : public State
{
    StatePackage handle(char c) override
    {
        StatePackage package;
        if (c == 'h')
        {
            package.identifier = +c;
            this->completed = true;
        }
        else if (isdigit(c))
        {
            package.tokenType = TOKEN_TYPE_REAL;
            package.identifier = +c;
            nextState = std::make_shared<State11>();
        }
        else if (c == '.')
        {
            package.identifier = +c;
            nextState = std::make_shared<State12>();
        }

        else
        {
            package.returnChar = true;
            this->completed = true;
        }

        return package;
    }
};

class State07 : public State
{
    StatePackage handle(char c) override
    {
        StatePackage package;
        if (isdigit(c))
        {
            package.identifier = +c;
            nextState = std::make_shared<State08>();
        }
        else if (isItaAlphabetHexa(c))
        {
            package.identifier = +c;
            nextState = std::make_shared<State09>();
        }
        else if (c == '.')
        {

            package.tokenType = TOKEN_TYPE_REAL;
            package.identifier = +c;
            nextState = std::make_shared<State12>();
        }
        else
        {
            package.returnChar = true;
            this->completed = true;
        }

        return package;
    }
};

/**
 *
 */
class State04 : public State
{
    StatePackage handle(char c) override
    {
        StatePackage package;
        if (isItaAlphabetHexa(c) || isdigit(c))
        {
            nextState = std::make_shared<State05>();
            package.identifier = +c;
        }
        else if (isalpha(c) || isdigit(c) || c == '_')
        {
            package.tokenclass = TOKEN_CLASS_VARIEBLE;
            package.tokenType = TOKEN_TYPE_STRING;
            package.identifier = +c;
            nextState = std::make_shared<State03>();
        }
        else
        {
            package.returnChar = true;
            this->completed = true;
        }

        return package;
    }
};

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
 * @brief classe representa o primeiro estado do automato [>s0]
 *
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
        package.tokenclass = TOKEN_CLASS_CONSTANT;
        package.tokenType = TOKEN_TYPE_BOOLEAN;
        nextState = std::make_shared<State17>();
    }
    else if (c == '=')
    {
        package.identifier = +c;
        package.tokenclass = TOKEN_CLASS_CONSTANT;
        package.tokenType = TOKEN_TYPE_BOOLEAN;
        nextState = std::make_shared<State18>();
    }
    else if (c == '\'')
    {
        package.identifier = +c;
        package.tokenclass = TOKEN_CLASS_CONSTANT;
        package.tokenType = TOKEN_TYPE_CHAR;
        nextState = std::make_shared<State19>();
    }
    else if (c == '"')
    {
        package.identifier = +c;
        package.tokenType = TOKEN_TYPE_STRING;
        package.tokenclass = TOKEN_CLASS_CONSTANT;
        nextState = std::make_shared<State21>();
    }
    else if (c == '>')
    {
        package.identifier = +c;
        package.tokenclass = TOKEN_CLASS_CONSTANT;
        package.tokenType = TOKEN_TYPE_BOOLEAN;
        nextState = std::make_shared<State22>();
    }
    else if (std::isalpha(c) || c == '_')
    {
        if (isItaAlphabetHexa(c))
        {
            package.identifier = +c;
            package.tokenclass = TOKEN_CLASS_CONSTANT;
            package.tokenType = TOKEN_TYPE_CHAR;
            nextState = std::make_shared<State04>();
        }
        else
        {
            package.identifier = +c;
            package.tokenclass = TOKEN_CLASS_VARIEBLE;
            package.tokenType = TOKEN_TYPE_STRING;
            nextState = std::make_shared<State03>();
        }
    }
    else if (isdigit(c))
    {
        package.identifier = +c;
        package.tokenclass = TOKEN_CLASS_CONSTANT;
        package.tokenType = TOKEN_TYPE_INTEGER;
        nextState = std::make_shared<State07>();
    }
    else if (c == '.')
    {
        package.tokenclass = TOKEN_CLASS_CONSTANT;
        package.tokenType = TOKEN_TYPE_REAL;
        package.identifier = +c;
        nextState = std::make_shared<State12>();
    }
    // Token Especial para informar o fim do arquivo
    else if (c == LEXEME_EOF)
    {
        package.identifier = +c;
        this->completed = true;
        package.tokenclass = TOKEN_CLASS_CONSTANT;
        package.tokenType = TOKEN_TYPE_CHAR;
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
        TokenType tokentype = TOKEN_TYPE_UNDEFINED;
        TokenClass tokenclas = TOKEN_CLASS_UNDEFINED;
        char cc;

        while (!currentState->isComplete())
        {

            cc = getNextChar();

            std::cout << (cc == LEXEME_EOF) << std::endl;
            // Pensar em alguma logica onde o

            if (symboltable->isItAValidChar(cc))
            {
                StatePackage result = currentState->handle(cc);
                lexeme += result.identifier;
                if (result.returnChar)
                {
                    pushBackCurrentChar();
                }
                if (result.tokenType != TOKEN_TYPE_UNDEFINED)
                {
                    tokentype = result.tokenType;
                }
                if (result.tokenclass != TOKEN_CLASS_UNDEFINED)
                {
                    tokenclas = result.tokenclass;
                }
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
        token.setLexeme(lexeme);

        std::cout << "\nToken encontrado: " << lexeme << std::endl;
        // Token t = Token(lexema);

        // adicionar o token

        // Tabela de simbolo relaciona o Token
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
        return this->file_point >= this->file.size();
    }
};
int main(int argc, char const *argv[])
{
    try
    {
        LexerAnalysis la("FF");

        la.getNextToken();
        la.getNextToken();
    }
    catch (const LException &e)
    {

        std::cerr << e.what() << std::endl;
    }

    return 0;
}
