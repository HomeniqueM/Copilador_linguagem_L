
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
        // Problema ver Com o home
        std::cout << "dhdid";
        LexerAnalysis la("int a = 4;");
        std::cout << "sssokso";
        //std:: cout <<  la.getNextToken();
        //Token a = la.getNextToken();
    }
    catch (const LException &e)
    {

        std::cerr << e.what() << std::endl;
    }

    return 0;
}