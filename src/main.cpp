
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <algorithm>

#include "symbols/symbol-table.cpp"
#include "symbols/token.cpp"
#include "error/l_exception.cpp"
#include "symbols/token-type.cpp"
#include "analyzers/lexical-analysis.cpp"
#include "analyzers/syntatic-analysis.cpp"

#include "symbols/token-id.cpp"

#include "utils/arg-handler.cpp"
#include "utils/file.cpp"
#include <string>

int main(int argc, char const *argv[])
{
    try
    {
        ArgHandler handler;
        FileHandler fh(handler.handleArgs(argc,argv));
        SymbolTable st = SymbolTable();
        LexerAnalysis la(&fh,&st);
        SyntaticAnalysis sa = SyntaticAnalysis(&la);
        sa.Start(la.getNextToken());
    }
    catch (const LException &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}