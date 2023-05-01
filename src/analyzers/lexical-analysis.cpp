#ifndef ANALYZERS_LEXICAL_ANALYSIS
#define ANALYZERS_LEXICAL_ANALYSIS

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
#include "../utils/constants.cpp"
#include "../utils/file.cpp"

int currentLine;
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
 *
 * @brief Cabeçarios das classes
 *
 * ========================================================== */
class StartState : public State
{
public:
    StatePackage handle(char c) override;
};

/**
 * ===========================================================================
 * @brief Enquanto o
 * @category State para tratar Identificador
 * ===========================================================================
 */

/** =========================================================
 * @brief Represanta o estado (S3)
 * @details State para trata idetificadores
 * @category State para tratar Identificador
 * @param c caracteres a ser analisado
 * ========================================================== */
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

/** =========================================================
 * @brief Represanta o estado (S6)
 * @details Estado trasitorio para o final de char hexa ou
 *          transição para um indetificado
 * @category State para tratar Final de um char hexa ou
 *           Identificador
 * @param c caracteres a ser analisado
 * ========================================================== */

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
            package.tokenType = TOKEN_TYPE_UNDEFINED;
            package.tokenId = TOKEN_ID_IDENTIFIER;
            package.identifier = +c;
            nextState = std::make_shared<State03>();
        }
        else
        {
            package.tokenType = TOKEN_TYPE_UNDEFINED;
            package.tokenId = TOKEN_ID_IDENTIFIER;
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
            throw LException(ErrorCode::UNEXPECTED_CHARACTER, currentLine, msg);
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
            throw LException(ErrorCode::UNEXPECTED_CHARACTER, currentLine, msg);
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
            package.tokenType = TOKEN_TYPE_REAL;
            package.tokenId = TOKEN_ID_CONSTANT;
            package.identifier = +c;
            nextState = std::make_shared<State13>();
        }
        else
        {
            std::string msg = "Erra esperando um valor numerico porém foi lido " + c;
            throw LException(ErrorCode::UNEXPECTED_CHARACTER, currentLine, msg);
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
            package.tokenId = TOKEN_ID_CONSTANT;
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
            throw LException(ErrorCode::UNEXPECTED_CHARACTER, currentLine, msg);
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

            package.tokenType = TOKEN_TYPE_UNDEFINED;
            package.tokenId = TOKEN_ID_IDENTIFIER;
            package.identifier = +c;
            nextState = std::make_shared<State03>();
        }
        else
        {
            package.tokenType = TOKEN_TYPE_UNDEFINED;
            package.tokenId = TOKEN_ID_IDENTIFIER;

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
            package.tokenId = TOKEN_ID_ENQUALS;
            package.identifier = +c;
        }
        else
        {
            package.returnChar = true;
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
        if (c == '=')
        {
            package.tokenId = TOKEN_ID_LESS_EQUAL_TO;
            package.identifier = +c;
        }
        else if (c == '>')
        {
            package.tokenId = TOKEN_ID_DIFFERENT;
            package.identifier = +c;
        }
        else
        {
            package.returnChar = true;
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
        else
        {
            package.returnChar = true;
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
            throw LException(ErrorCode::UNEXPECTED_CHARACTER, currentLine, msg);
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
            // package.identifier = +c;
            this->completed = true;
        }
        else if (c == LEXEME_BREAK_LINE)
        {
            throw LException(ErrorCode::STRING_BREAK_LINE,currentLine,"");
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
        this->completed = true;
        package.tokenId = stringToTokenId(std::string(1, c));
    }

    // Estado de comentario
    else if (c == '{')
    {
        nextState = std::make_shared<CommentState>();
    }
    else if (c == '<')
    {
        package.identifier = +c;
        package.tokenId = TOKEN_ID_LESS_THAN;

        nextState = std::make_shared<State17>();
    }
    else if (c == '=')
    {
        package.identifier = +c;
        package.tokenId = TOKEN_ID_ASSIGNMENT;
        nextState = std::make_shared<State18>();
    }
    else if (c == '\'')
    {
        package.identifier = +c;

        package.tokenType = TOKEN_TYPE_CHAR;
        package.tokenId = TOKEN_ID_CONSTANT;
        nextState = std::make_shared<State19>();
    }
    else if (c == '"')
    {
        package.identifier = +c;
        package.tokenType = TOKEN_TYPE_STRING;
        package.tokenId = TOKEN_ID_CONSTANT;
        nextState = std::make_shared<State21>();
    }
    else if (c == '>')
    {
        package.identifier = +c;
        package.tokenId = TOKEN_ID_GREATER_THEN;
        nextState = std::make_shared<State22>();
    }
    else if (std::isalpha(c) || c == '_')
    {
        if (isItaAlphabetHexa(c))
        {
            package.identifier = +c;
            package.tokenType = TOKEN_TYPE_CHAR;
            package.tokenId = TOKEN_ID_CONSTANT;
            nextState = std::make_shared<State04>();
        }
        else
        {
            package.identifier = +c;
            package.tokenType = TOKEN_TYPE_UNDEFINED;
            package.tokenId = TOKEN_ID_IDENTIFIER;
            nextState = std::make_shared<State03>();
        }
    }
    else if (isdigit(c))
    {
        package.tokenType = TOKEN_TYPE_CHAR;
        package.tokenId = TOKEN_ID_CONSTANT;
        package.identifier = +c;
        nextState = std::make_shared<State07>();
    }
    else if (c == '.')
    {
        package.tokenType = TOKEN_TYPE_REAL;
        package.tokenId = TOKEN_ID_CONSTANT;
        package.identifier = +c;
        nextState = std::make_shared<State12>();
    }
    // Token Especial para informar o fim do arquivo
    else if (c == LEXEME_EOF)
    {
        package.identifier = +c;
        this->completed = true;
        package.tokenType = TOKEN_TYPE_CHAR;
        package.tokenId = TOKEN_ID_EOF;
    }
    else
    {
        // Tratativa a ser feita

        std::string str(1, c);
        throw LException(ErrorCode::UNEXPECTED_CHARACTER, currentLine, str);
    }
    return package;
}

/** =========================================================
 * @brief Represanta o estado (S1) ou estado final
 * @details Estado para tratar a criação de token
 * ========================================================== */
class State01
{
public:
    Token *makeAToken(SymbolTable *st, std::string lexeme, TokenType tokentype, TokenID tokenId, int currentLine)
    {

        Token token = Token();
        Token *result = nullptr;

        // verificar se e eh constante
        if (tokenId == TOKEN_ID_IDENTIFIER)
        {
            // Verifica se o Tamanho é maior
            if (lexeme.size() > CONSTANTS_IDENTIFIER_MAX_SIZE)
            {
                throw LException(ErrorCode::ENCEEDED_LIMIT_IDENTIFIER_MAX_SIZE, currentLine, "");
            }
            token.setLexeme(lexeme);
            token.setTokenType(tokentype);
            token.setTokenID(tokenId);
            // verificar se e palavra reservada
            // se nao for nenhum dos dois entao ele eh identificador
            // Aqui deve ser inserio o Token de identifier
            result = st->Insert(token);
        }
        else if (tokenId == TOKEN_ID_CONSTANT)
        {
            if (tokentype == TOKEN_TYPE_REAL)
            {
                std::string sublexeme = substringAfterDelimiter(lexeme, '.');
                double doulexeme = std::stod(lexeme);

                if (sublexeme.size() > CONSTANTS_MAXIMUM_ACCURACY_LENGTH)
                {
                    throw LException(ErrorCode::OVERFLOW_ACCURACY_LENGTH, currentLine, "");
                }
                if (doulexeme > CONSTANTS_REAL_MAX_VALUE)
                {
                    throw LException(ErrorCode::OVERFLOW_SIZE_REAL, currentLine, "");
                }
                if (doulexeme < CONSTANTS_REAL_MIN_VALUE)
                {
                    throw LException(ErrorCode::UNDERFLOW_SIZE_REAL, currentLine, "");
                }
            }
            else if (tokentype == TOKEN_TYPE_INTEGER)
            {
                int lexemeInt = std::stoi(lexeme);
                if (lexemeInt > CONSTANTS_INTEGER_MAX_VALUE)
                {
                    throw LException(ErrorCode::OVERFLOW_SIZE_INTEGER, currentLine, "");
                }
                else if (lexemeInt < CONSTANTS_INTEGER_MIN_VALUE)
                {
                    throw LException(ErrorCode::UNDERFLOW_SIZE_INTEGER, currentLine, "");
                }
            }
            result = new Token();
            result->setTokenSize(getTokenSize(tokentype, lexeme));
            result->setLexeme(lexeme);
            result->setTokenType(tokentype);
            result->setTokenID(tokenId);
        }
        else
        {
            result = new Token();
            result->setLexeme(lexeme);
            result->setTokenType(tokentype);
            result->setTokenID(tokenId);
        }

        return result;
    }

private:
    /**
     * @brief Retorna a substring depois do delimitador especificado.
     *
     * @param str A string original.
     * @param delimiter O caractere delimitador.
     * @return A substring depois do delimitador, ou uma string vazia se o delimitador não for encontrado.
     */
    std::string substringAfterDelimiter(std::string str, char delimiter)
    {
        int delimiterIndex = str.find(delimiter);
        if (delimiterIndex == std::string::npos)
        {
            return "";
        }
        return str.substr(delimiterIndex + 1);
    }

    size_t getTokenSize(TokenType tokenType, std::string lexeme)
    {
        size_t result = 0;
        switch (tokenType)
        {
        case TOKEN_TYPE_BOOLEAN:
            result = CONSTANTS_BOOLEAN_SIZE;
            break;
        case TOKEN_TYPE_CHAR:
            result = CONSTANTS_CHAR_SIZE;
            break;
        case TOKEN_TYPE_INTEGER:
            result = CONSTANTS_INTEGER_SIZE;
            break;
        case TOKEN_TYPE_REAL:
            result = CONSTANTS_REAL_SIZE;
            break;
        case TOKEN_TYPE_STRING:
            result = lexeme.size() * CONSTANTS_CHAR_SIZE;
            break;
        default:
            result = 0;
            break;
        }
        return result;
    }
};

/**
 * @brief Classe principal do Análise léxica
 */
class LexerAnalysis
{
private:
    FileHandler *fh;                     // String a ser tokenizada
    std::shared_ptr<State> currentState; // Validar necessidade
    SymbolTable *symboltable;

public:
    LexerAnalysis(FileHandler *fh, SymbolTable *symboltable) : currentState(std::make_shared<StartState>())
    {
        this->fh = fh;
        this->symboltable = symboltable;
    }
    /**
     * @brief função que retorna o proximo possivel token
     */
    Token *getNextToken()
    {
        currentState = std::make_shared<StartState>();
        std::string lexeme = "";
        TokenType tokentype = TOKEN_TYPE_UNDEFINED;

        TokenID tokenId = TOKEN_ID_NULL;

        char cc;

        while (!currentState->isComplete())
        {

            cc = getNextChar();

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
                if (result.tokenId != TOKEN_ID_NULL)
                {
                    tokenId = result.tokenId;
                }
                if (currentState->isComplete() == false)
                {
                    currentState = currentState->nextState;
                }
            }

            else
            {
                std::string str(1, cc);

                throw LException(ErrorCode::INVALIDCHARACTER, getCurrentLine(), str);
            }
        }

        return State01().makeAToken(symboltable, lexeme, tokentype, tokenId, getCurrentLine());
    };

    void pushBackCurrentChar()
    {
        // call FileHandler setPrevChar()
        fh->setPrevChar();
    }
    /**
     * @brief retorna o proximo char caso o arquivo não tenha terminado caso contrario retorna \0
     */
    char getNextChar()
    {
        currentLine = getCurrentLine();
        return fh->getNextFileChar();
    }
    int getCurrentLine()
    {
        return fh->getFileLine();
    }
};

#endif
