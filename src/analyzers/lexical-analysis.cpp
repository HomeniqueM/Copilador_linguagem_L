#ifndef ANALYZERS_LEXICAL_ANALYSIS
#define ANALYZERS_LEXICAL_ANALYSIS

#include <iostream>
#include <string>
#include "src/symbols/token.cpp"
/** 
 * @brief classe base para fazer a represetação de cada estado 
 * 
 */
class State
{
public:
    virtual void handle(char c) = 0;
    virtual bool isComplete() = 0;
    virtual State *getNextState() = 0;
    virtual std::string getIdentifier() const = 0;
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
