#ifndef ANALYZERS_LEXICAL_ANALYSIS
#define ANALYZERS_LEXICAL_ANALYSIS

#include <iostream>
#include <string>
#include "src/symbols/token.cpp"
#include "src/symbols/lexeme.cpp"
/**
 * @brief classe base para fazer a represetação de cada estado
 *
 */
class State
{
public:
    /** Função principal do estado onde cada estado vai 
     * tratar como deve ser feito a transição para o proximo estado
     */
    virtual void handle(char c) = 0;
    virtual bool isComplete() = 0;
    virtual State *getNextState() = 0;
    virtual std::string getLexeme() const = 0;
};

/**
 * @brief Representa o primeiro estado do analisador lexico
 */
class StartState : public State
{
private:
    std::string lexeme;
    bool completed;
    State *nextState = nullptr;

public:
    // Função principal
    void handle(char c)
    {
      
    }
    bool isComplete() override
    {
        return completed;
    }
    State *getNextState()override{
        return nextState;
    }
    std::string getLexeme(){
        return lexeme;
    }
};

class LexicalAnalysis
{
private:
    std::string file;

public:
    LexicalAnalysis(/* args */);
    ~LexicalAnalysis();
    Token handleToke();
};

LexicalAnalysis::LexicalAnalysis(/* args */)
{
}

LexicalAnalysis::~LexicalAnalysis()
{
}

#endif
