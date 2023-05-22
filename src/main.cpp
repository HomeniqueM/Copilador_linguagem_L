/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
*/
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <algorithm>

#include "symbols/symbol-table.cpp"
#include "symbols/token.h"
#include "error/l_exception.cpp"
#include "symbols/token-type.cpp"
#include "analyzers/lexical-analysis.cpp"
#include "analyzers/syntatic-analysis.cpp"

#include "symbols/token-id.cpp"

#include "utils/arg-handler.h"
#include "utils/file.cpp"
#include <string>

int main(int argc, char const *argv[])
{
    try
    {
        ArgHandler handler;
        FileHandler fh(handler.handleArgs(argc, argv));
        SymbolTable st = SymbolTable();
        LexerAnalysis la(&fh, &st);
        SyntaticAnalysis sa = SyntaticAnalysis(&la);
        sa.Start(la.getNextToken());
        std::cout << "[" << la.getCurrentLine() -1 << "] Linhas Compiladas.";
    }
    catch (const LException &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}