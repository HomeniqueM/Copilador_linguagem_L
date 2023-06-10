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

#include "symbols/symbol-table.hpp"
#include "error/l_exception.cpp"
#include "analyzers/lexical-analysis.hpp"
#include "analyzers/syntatic-analysis.hpp"
#include "utils/arg-handler.hpp"
#include "utils/file.hpp"
#include "code_generator/code-gen.hpp"
#include <string>

/*int main(int argc, char const *argv[])
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
}*/

int main(){
    CodeGen cg("program.asm");
    Token *id = new Token();
    Token *constant = new Token();
    id->setTokenType(TOKEN_TYPE_STRING);
    constant->setTokenType(TOKEN_TYPE_STRING);
    constant->setLexeme("Banana");
    constant->setTokenSize(6);
    cg.startData();
    cg.startText();
    cg.write(id);
    cg.end();
    return 0;
}