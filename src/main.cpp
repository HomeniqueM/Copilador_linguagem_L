
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <algorithm>

#include "../src/symbols/symbol-table.cpp"
#include "../src/symbols/token.cpp"
#include "../src/error/l_exception.cpp"
#include "../src/symbols/token-type.cpp"
#include "../src/analyzers/lexical-analysis.cpp"

int main(int argc, char const *argv[])
{
    try
    {
        //LexerAnalysis la("int a = 4;");

      //  la.getNextToken();
       // la.getNextToken();
    }
    catch (const LException &e)
    {

        std::cerr << e.what() << std::endl;
    }

    return 0;
}