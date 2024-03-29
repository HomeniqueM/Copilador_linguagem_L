/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
 */

#include "syntatic-analysis.hpp"
#include <iostream>

SyntaticAnalysis::SyntaticAnalysis(LexerAnalysis *la, SemanticAnalysis *se, SymbolTable *st, CodeGen *cg)
{
    this->la = la;
    this->se = se;
    this->st = st;
    this->cg = cg;
}
/**
 * @brief: Inicia o Analizador Sintatico.
 * @param Token inicial a ser analisado
 */

void SyntaticAnalysis::Start(Token *token)
{
    setToken(token);
    productionS();
    cg->startText();
    cg->end();
}

void SyntaticAnalysis::setToken(Token *token)
{
    this->token = token;
}

/**
 * @brief: Realiza o casamento do Token Experado pelo Token Encontrado.
 */
void SyntaticAnalysis::matchToken(TokenID expectedToken)
{
    // std::cout << "Token Esperado :" << tokenToString(expectedToken) << expectedToken << std::endl;
    // std::cout << "Token Encontrado :" << tokenToString(token->getTokenid()) << token->getTokenid() << std::endl;
    // Fazer o match
    // std::cout << expectedToken << "  " << token->getTokenid();

    if (expectedToken == token->getTokenid())
    {
        // Pedir o prx token
        setToken(la->getNextToken());
        // std::cout << "Proximo Token:" << tokenToString(token->getTokenid()) << token->getTokenid() << std::endl;
    }
    else
    {
        if (token->getTokenid() == TOKEN_ID_EOF)
        {
            throw LException(ErrorCode::UNEXPECTED_EOF, la->getCurrentLine() - 1, "");
        }
        else
        {
            throw LException(ErrorCode::UNEXPECTED_TOKEN, la->getCurrentLine(), token->getLexeme());
        }
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 *         S -> { ( D | Cmd) } fim_arquivo
 */
void SyntaticAnalysis::productionS()
{
    while (token->getTokenid() != TOKEN_ID_EOF)
    {
        bool declaration =
            token->getTokenid() == TOKEN_ID_FINAL ||
            token->getTokenid() == TOKEN_ID_INTEGER ||
            token->getTokenid() == TOKEN_ID_CHAR ||
            token->getTokenid() == TOKEN_ID_REAL ||
            token->getTokenid() == TOKEN_ID_STRING ||
            token->getTokenid() == TOKEN_ID_BOOLEAN;

        if (declaration)
        {
            cg->startData();
            productionD();
        }
        else
        {  
            cg->startText();
            productionCMD();
        }
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 *         D -> D1 C { ,C } ; | final id = [-] const ;
 */
void SyntaticAnalysis::productionD()
{
    if (token->getTokenid() == TOKEN_ID_FINAL)
    {
        bool negate = false;
        matchToken(TOKEN_ID_FINAL);
        //Ação Semantica [1]
        this->se->isTokenHasDeclarad(token, TOKEN_CLASS_CONSTANT);
        Token *identifier = token;
        matchToken(TOKEN_ID_IDENTIFIER);
        matchToken(TOKEN_ID_ASSIGNMENT);
        if (token->getTokenid() == TOKEN_ID_SUBTRACTION)
        {
            //Ação Semantica [2]
            negate = true;
            matchToken(TOKEN_ID_SUBTRACTION);
        }
        Token *constante = token;
        matchToken(TOKEN_ID_CONSTANT);
        //Ação Semantica [20]
        this->se->FinalVerify(constante, identifier, negate);

        // Geração de Codico 
        // Final indentificador = constante;
        cg->DeclareConst(identifier,constante);
    }
    else
    {
        productionD1();
        productionC();
        while (token->getTokenid() == TOKEN_ID_COMMA)
        {
            matchToken(TOKEN_ID_COMMA);
            productionC();
        }
    }
    matchToken(TOKEN_ID_SEMICOLON);
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * D1 -> ( char | integer | real | boolean | string )
 */
void SyntaticAnalysis::productionD1()
{

    switch (token->getTokenid())
    {
    case TOKEN_ID_CHAR:
        matchToken(TOKEN_ID_CHAR);
        // Ação Semantica [4]
        this->se->defineNewType(token, TOKEN_TYPE_CHAR);
        break;
    case TOKEN_ID_INTEGER:
        matchToken(TOKEN_ID_INTEGER);
         // Ação Semantica [4]
        this->se->defineNewType(token, TOKEN_TYPE_INTEGER);
        break;
    case TOKEN_ID_REAL:
        matchToken(TOKEN_ID_REAL);
         // Ação Semantica [4]
        this->se->defineNewType(token, TOKEN_TYPE_REAL);
        break;
    case TOKEN_ID_BOOLEAN:
        matchToken(TOKEN_ID_BOOLEAN);
         // Ação Semantica [4]
        this->se->defineNewType(token, TOKEN_TYPE_BOOLEAN);
        break;
    case TOKEN_ID_STRING:
        matchToken(TOKEN_ID_STRING);
         // Ação Semantica [4]
        this->se->defineNewType(token, TOKEN_TYPE_STRING);
        break;
    default:
        std::cout << "ERRO NÃO SEI COMO SE CONSEGUIU ESSA PROESA.";
        break;
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * C -> id [ ( = [ - ] const | [ ( id | const) ] ) ]
 */
void SyntaticAnalysis::productionC()
{
    bool neg = false;
    Token *tokenVar = token;
    matchToken(TOKEN_ID_IDENTIFIER);
    // Ação Semantica [5]
    this->se->isTokenHasDeclarad(tokenVar, TOKEN_CLASS_VARIABLE);

    // Geração de Codigo 
    // Declaração de Variavel
    cg->DeclareVariable(tokenVar);

    if (token->getTokenid() == TOKEN_ID_ASSIGNMENT)
    {
        matchToken(TOKEN_ID_ASSIGNMENT);
        Token *constant = token;
        if (token->getTokenid() == TOKEN_ID_FALSE)
        {
            // Ação Semantica [3]
            this->se->ifTokenTypeHasEqualsorIntandReal(tokenVar, TOKEN_TYPE_BOOLEAN);
            this->se->defineNewType(constant, TOKEN_TYPE_BOOLEAN);
            matchToken(TOKEN_ID_FALSE);
        }
        else if (token->getTokenid() == TOKEN_ID_TRUE)
        {
            // Ação Semantica [3]
            this->se->ifTokenTypeHasEqualsorIntandReal(tokenVar, TOKEN_TYPE_BOOLEAN);
            this->se->defineNewType(constant, TOKEN_TYPE_BOOLEAN);
            matchToken(TOKEN_ID_TRUE);
        }
        else
        {
            if (token->getTokenid() == TOKEN_ID_SUBTRACTION)
            {
                // Ação Semantica [2]
                neg = true;
                matchToken(TOKEN_ID_SUBTRACTION);
            }
            // Ação Semantica [3]
            this->se->isTokenTypeEquals(token, tokenVar, neg);
            constant = token;
            matchToken(TOKEN_ID_CONSTANT);
        }

        // Geração de Codigo 
        // Operação de atribuição
        // real a = 5;
        cg->storeConstOnTmp(constant,constant);
        if(neg){
            cg->invertExpression(constant);
        }
        if(tokenVar->getTokenType() == TOKEN_TYPE_REAL && constant->getTokenType() == TOKEN_TYPE_INTEGER){
            cg->cvtToReal(constant);
        }
        cg->atributionCommand(tokenVar,constant);

    }
    else if (token->getTokenid() == TOKEN_ID_OPEN_BRACKET)
    {
        // Gambiarra
        tokenVar->setTokenClass(TOKEN_CLASS_UNDEFINED);
        
        matchToken(TOKEN_ID_OPEN_BRACKET);
        // Ação Semantica [6]
        this->se->isTokenHasDeclarad(tokenVar, TOKEN_CLASS_VETOR);
        this->se->setMaxTamVet(tokenVar, token);
        this->se->ifTokenTypehasDiff(token, TOKEN_TYPE_INTEGER);
        // Geração de Codigo 
        // Operação de vetor
        // char a  [20];
        cg->DeclareVet(tokenVar,token);

        matchToken(TOKEN_ID_CONSTANT);
        matchToken(TOKEN_ID_CLOSE_BRACKET);
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * Cmd -> ( ( [ ( A | L | E ) ] ; ) | ( R | T ) )
 */
void SyntaticAnalysis::productionCMD()
{

    if (token->getTokenid() == TOKEN_ID_IF || token->getTokenid() == TOKEN_ID_FOR)
    {
        if (token->getTokenid() == TOKEN_ID_FOR)
        {
            productionR();
        }
        else
        {
            productionT();
        }
    }
    else
    {
        if (token->getTokenid() == TOKEN_ID_IDENTIFIER)
        {
            productionA();
        }
        else if (token->getTokenid() == TOKEN_ID_READLN)
        {
            productionL();
        }
        else if (token->getTokenid() == TOKEN_ID_WRITE || token->getTokenid() == TOKEN_ID_WRITELN)
        {
            productionE();
        }
        matchToken(TOKEN_ID_SEMICOLON);
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * Cmd1 -> [ ( A | R | T | L | E ) ]
 */
void SyntaticAnalysis::productionCMD1()
{
    if (token->getTokenid() == TOKEN_ID_IDENTIFIER)
    {
        productionA();
    }
    else if (token->getTokenid() == TOKEN_ID_FOR)
    {
        productionR();
    }
    else if (token->getTokenid() == TOKEN_ID_IF)
    {
        productionT();
    }
    else if (token->getTokenid() == TOKEN_ID_READLN)
    {
        productionL();
    }
    else
    {
        productionE();
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * A ->  id = ( Exp | L )
 */
void SyntaticAnalysis::productionA()
{
    Token *tokenId = token;
    Token tokenExp;
    Token *constant = new Token();
    bool isVector = false;
    matchToken(TOKEN_ID_IDENTIFIER);
    // Ação Semantica [7]
    this->se->isTokenNotHasDeclarationAndNotHasConst(tokenId);
    if (token->getTokenid() == TOKEN_ID_OPEN_BRACKET)
    {
        matchToken(TOKEN_ID_OPEN_BRACKET);
        isVector = true;
        // Ação Semantica [8]
        this->se->ifTokenTypehasDiff(token, TOKEN_TYPE_INTEGER);
        this->se->ifTokenVectorInRange(tokenId, token);
        constant = token;
        matchToken(TOKEN_ID_CONSTANT);
        matchToken(TOKEN_ID_CLOSE_BRACKET);
    }
    matchToken(TOKEN_ID_ASSIGNMENT);
    if (token->getTokenid() == TOKEN_ID_READLN)
    {
        productionL();
    }
    tokenExp = productionExp();
    // Ação Semantica [9]
    // se  id.tipo != Exp1.tipo e !(id.tipo == real e Exp1.tipo == inteiro)
    this->se->verificacaoDeAtribuicao(tokenId,&tokenExp);
    if(tokenId->getTokenType()==TOKEN_TYPE_REAL && tokenExp.getTokenType()==TOKEN_TYPE_INTEGER){
        cg->cvtToReal(&tokenExp);
    }
    if(isVector){
        cg->storeConstOnTmp(constant,constant);
        cg->vetAtribution(tokenId,constant,&tokenExp);
    }else{
        cg->atributionCommand(tokenId,&tokenExp);
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * R -> for (  R1 ;  Exp ;  R1  ) T1

*/
void SyntaticAnalysis::productionR()
{
    int rot1 = cg->newLabel();
    int rot2 = cg->newLabel();
    int rot3 = cg->newLabel();
    int rot4 = cg->newLabel();
    matchToken(TOKEN_ID_FOR);
    matchToken(TOKEN_ID_OPEN_PARANTHESES);
    productionR1();
    cg->writeRot(rot1);
    matchToken(TOKEN_ID_SEMICOLON);
    Token exp = productionExp();
    // Ação Semantica [10]
    this->se->ifTokenTypehasDiff(&exp, TOKEN_TYPE_BOOLEAN);
    cg->compareForExpression(&exp,rot4);
    cg->writeJump(rot3);
    cg->writeRot(rot2);
    matchToken(TOKEN_ID_SEMICOLON);
    productionR1();
    cg->writeJump(rot1);
    cg->writeRot(rot3);
    matchToken(TOKEN_ID_CLOSE_PARANTHESES);
    productionT1();
    cg->writeJump(rot2);
    cg->writeRot(rot4);
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * R1-> Cmd1 { ,Cmd1 }

*/
void SyntaticAnalysis::productionR1()
{
    productionCMD1();
    while (token->getTokenid() == TOKEN_ID_COMMA)
    {
        matchToken(TOKEN_ID_COMMA);
        productionCMD1();
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * T -> if ( Exp ) T1 [ else T1 ]
 */
void SyntaticAnalysis::productionT()
{   
    Token tokenExp;
    bool hasElse = false;
    int rot1 = cg->newLabel();
    int rot2 = cg->newLabel();
    matchToken(TOKEN_ID_IF);
    matchToken(TOKEN_ID_OPEN_PARANTHESES);
    tokenExp = productionExp();
    Token *result = new Token();
    result->setTokenID(tokenExp.getTokenid());
    result->setLexeme(tokenExp.getLexeme());
    result->setTokenType(tokenExp.getTokenType());
    result->setTokenSize(tokenExp.getTokeSize());
    result->setTokenClass(tokenExp.getTokenClass());
    result->setMaxTam(tokenExp.getMaxTam());
    result->setTokenAddr(tokenExp.getTokenAddr());
    // Ação Semantica [10]
    this->se->ifTokenTypehasDiff(&tokenExp, TOKEN_TYPE_BOOLEAN);
    matchToken(TOKEN_ID_CLOSE_PARANTHESES);
    cg->initCondition(result,rot1);
    productionT1();
    if (token->getTokenid() == TOKEN_ID_ELSE)
    {
        hasElse = true;
        matchToken(TOKEN_ID_ELSE);
        cg->finalizeBlock(rot2,rot1);
        productionT1();
    }
    cg->finalizeConditionalChain(hasElse,rot1,rot2);
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * T1 -> ( Cmd | begin { Cmd } end )
 */
void SyntaticAnalysis::productionT1()
{
    if (token->getTokenid() == TOKEN_ID_BEGIN)
    {
        matchToken(TOKEN_ID_BEGIN);
        while (token->getTokenid() != TOKEN_ID_END)
        {
            productionCMD();
        }
        matchToken(TOKEN_ID_END);
    }
    else
    {
        productionCMD();
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * L -> readln ( id )
 */
void SyntaticAnalysis::productionL()
{
    matchToken(TOKEN_ID_READLN);
    matchToken(TOKEN_ID_OPEN_PARANTHESES);
    Token *tokenID = token;
    matchToken(TOKEN_ID_IDENTIFIER);
    // Ação Semantiva [7]
    this->se->isTokenNotHasDeclarationAndNotHasConst(tokenID);
    // Açao Semantica [11]
    this->se->tokenIsTypeEqualsErro(tokenID, TOKEN_TYPE_BOOLEAN);
    matchToken(TOKEN_ID_CLOSE_PARANTHESES);
    cg->readln(tokenID);
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * E -> ( write | writeln ) ( E1 )
 */
void SyntaticAnalysis::productionE()
{
    bool lineBreak = false;
    Token E_tmp;
    if (token->getTokenid() == TOKEN_ID_WRITE)
    {
        matchToken(TOKEN_ID_WRITE);
    }
    else
    {
        matchToken(TOKEN_ID_WRITELN);
        lineBreak = true;
    }
    matchToken(TOKEN_ID_OPEN_PARANTHESES);
    E_tmp = productionE1();
    matchToken(TOKEN_ID_CLOSE_PARANTHESES);

    if(lineBreak){
        cg->writeln();
    }
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * E1 -> Exp { , Exp }
 */
Token SyntaticAnalysis::productionE1()
{
    Token tokenE1;
    tokenE1 = productionExp();
    // Açao Semantica [11]
    this->se->tokenIsTypeEqualsErro(&tokenE1, TOKEN_TYPE_BOOLEAN);
    cg->write(&tokenE1);

    while (token->getTokenid() == TOKEN_ID_COMMA)
    {
        matchToken(TOKEN_ID_COMMA);
        tokenE1 = productionExp();
        // Açao Semantica [11]
        this->se->tokenIsTypeEqualsErro(&tokenE1, TOKEN_TYPE_BOOLEAN);
        cg->write(&tokenE1);
    }
    return tokenE1;
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * Exp -> Exp1 { ( == | < | <= | > | >= ) Exp1 }
 */
Token SyntaticAnalysis::productionExp()
{
    Token tokenExp;
    Token operador;
    Token tokenExp1;
    // Regra 14;
    tokenExp = productionExp1();
    while (token->getTokenid() == TOKEN_ID_EQUALS || token->getTokenid() == TOKEN_ID_GREATER_THEN || token->getTokenid() == TOKEN_ID_GREATER_EQUAL_TO || token->getTokenid() == TOKEN_ID_LESS_THAN || token->getTokenid() == TOKEN_ID_LESS_EQUAL_TO || token->getTokenid() == TOKEN_ID_DIFFERENT) 
    {
        // Regra [21]
        operador = token->clone();
        if (token->getTokenid() == TOKEN_ID_EQUALS)
        {
            matchToken(TOKEN_ID_EQUALS);
        }
        else if (token->getTokenid() == TOKEN_ID_GREATER_THEN)
        {
            matchToken(TOKEN_ID_GREATER_THEN);
        }
        else if (token->getTokenid() == TOKEN_ID_GREATER_EQUAL_TO)
        {
            matchToken(TOKEN_ID_GREATER_EQUAL_TO);
        }
        else if (token->getTokenid() == TOKEN_ID_LESS_THAN)
        {
            matchToken(TOKEN_ID_LESS_THAN);
        }
        else if (token->getTokenid() == TOKEN_ID_DIFFERENT)
            matchToken(TOKEN_ID_DIFFERENT);
        else
        {
            matchToken(TOKEN_ID_LESS_EQUAL_TO);
        }
        tokenExp1 = productionExp1();
        // Regra [20]
        this->se->rulle20(&tokenExp, &tokenExp1, &operador);
        cg->RelacionalOperator(&tokenExp,&tokenExp1,operador.getTokenid());
        tokenExp.setTokenType(TOKEN_TYPE_BOOLEAN);
    }
    return tokenExp;
}

/**
 * @brief: Analisa o caso da produção da Gramatica.
 * Exp1 -> [ - ] Exp2 { ( + | - | or ) Exp2 }
 */
Token SyntaticAnalysis::productionExp1()
{
    Token tokenExp1;
    Token operador;
    Token tokenExp2;
    bool isNeg = false;
    if (token->getTokenid() == TOKEN_ID_SUBTRACTION)
    {
        // Regra 19 Da Gramatica
        isNeg = true;
        matchToken(TOKEN_ID_SUBTRACTION);
    }
    tokenExp1 = productionExp2();
    if(isNeg){
        cg->invertExpression(&tokenExp1);
    }
    // Regra 15 Da Gramatica
    this->se->tokenIsIntergerOrReal(&tokenExp1, isNeg);

    while (token->getTokenid() == TOKEN_ID_ADDITION || token->getTokenid() == TOKEN_ID_SUBTRACTION || token->getTokenid() == TOKEN_ID_OR)
    {
        // Regra [22]
        operador = token->clone();
        if (token->getTokenid() == TOKEN_ID_ADDITION)
        {
            matchToken(TOKEN_ID_ADDITION);
            // this->se->tokenIsIntergerOrReal(&tokenExp1, true);
        }
        else if (token->getTokenid() == TOKEN_ID_SUBTRACTION)
        {
            matchToken(TOKEN_ID_SUBTRACTION);
            // this->se->tokenIsIntergerOrReal(&tokenExp1, true);
        }
        else
        {
            matchToken(TOKEN_ID_OR);
            // this->se->ifTokenTypehasDiff(&tokenExp1, TOKEN_TYPE_BOOLEAN);
        }
        tokenExp2 = productionExp2();
        // Regra [23]
        this->se->rulle23(&tokenExp1, &tokenExp2, &operador);
        cg->defineOperation(&tokenExp1,&tokenExp2,&operador);
    }
    return tokenExp1;
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * Exp2 -> Exp3 { ( *  | mod | (div |/)| and ) Exp3}
 */
Token SyntaticAnalysis::productionExp2()
{
    //(2*4)
    Token tokenEXP2 = productionExp3();
    Token operatorToken = token->clone(); // 2
    while (token->getTokenid() == TOKEN_ID_MULTIPLICATION || token->getTokenid() == TOKEN_ID_DIVISION || token->getTokenid() == TOKEN_ID_MODULO || token->getTokenid() == TOKEN_ID_AND)
    {
        if (token->getTokenid() == TOKEN_ID_MULTIPLICATION)
        {
            matchToken(TOKEN_ID_MULTIPLICATION);
        }
        else if (token->getTokenid() == TOKEN_ID_DIVISION)
            matchToken(TOKEN_ID_DIVISION);
        else if (token->getTokenid() == TOKEN_ID_MODULO)
            matchToken(TOKEN_ID_MODULO);
        else
            matchToken(TOKEN_ID_AND);
        Token tokenAux = productionExp3();
        this->se->rulle25(&tokenEXP2, &tokenAux, &operatorToken);
        cg->defineOperation(&tokenEXP2,&tokenAux,&operatorToken);
        // tokenEXP2 tokenAux
    }
    return tokenEXP2;
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * Exp3 -> Exp4 | not Exp4
 */
Token SyntaticAnalysis::productionExp3()
{
    Token tokenExp3;
    bool isNot = false;
    if (token->getTokenid() == TOKEN_ID_NOT)
    {
        isNot = true;
        matchToken(TOKEN_ID_NOT);
    }
    //[17]
    tokenExp3 = productionExp4();
    // [26]
    this->se->tokenIsBoolean(&tokenExp3, isNot);
    if(isNot){
        cg->negExpression(&tokenExp3);
    }
    return tokenExp3;
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
 * Exp4 -> Exp5 | real( Exp5 ) | integer( Exp5 )
 */
Token SyntaticAnalysis::productionExp4()
{
    Token tokenExp4;
    Token *tokenExp5 = nullptr;
    if (token->getTokenid() != TOKEN_ID_INTEGER && token->getTokenid() != TOKEN_ID_REAL)
    {
        // [18]
        tokenExp5 = productionExp5();
        tokenExp4 = tokenExp5->clone();
    }
    else
    {
        if (token->getTokenid() == TOKEN_ID_INTEGER)
        {
            matchToken(TOKEN_ID_INTEGER);
        }
        else
        {
            matchToken(TOKEN_ID_REAL);
        }
        matchToken(TOKEN_ID_OPEN_PARANTHESES);
        tokenExp5 = productionExp5();
        tokenExp4 = tokenExp5->clone();
        // [27 && 28]
        this->se->TokenReplaceType(&tokenExp4);
        if(tokenExp4.getTokenType()==TOKEN_TYPE_INTEGER){
            cg->cvtToInt(&tokenExp4);
        }else{
            cg->cvtToReal(&tokenExp4);
        }
        matchToken(TOKEN_ID_CLOSE_PARANTHESES);
    }
    return tokenExp4;
}

/**
 * @brief: Analísa o caso da produção da Gramatica.
Exp5 -> const | id | true | false | [ [ ( id | const) ]  ] | ( Exp )

 */
Token *SyntaticAnalysis::productionExp5()
{
    Token *tokenExp5 = token;
    if (token->getTokenid() == TOKEN_ID_CONSTANT)
    {
        // [28] nao existe
        matchToken(TOKEN_ID_CONSTANT);
        cg->storeConstOnTmp(tokenExp5,tokenExp5);
    }
    else if (token->getTokenid() == TOKEN_ID_TRUE)
    {
        this->se->defineNewType(tokenExp5, TOKEN_TYPE_BOOLEAN);
        matchToken(TOKEN_ID_TRUE);
        cg->storeConstOnTmp(tokenExp5,tokenExp5);
    }
    else if (token->getTokenid() == TOKEN_ID_FALSE)
    {
        this->se->defineNewType(tokenExp5, TOKEN_TYPE_BOOLEAN);
        matchToken(TOKEN_ID_FALSE);
        cg->storeConstOnTmp(tokenExp5,tokenExp5);
    }
    else if (token->getTokenid() == TOKEN_ID_IDENTIFIER)
    {
        Token tokenID = *token;
        matchToken(TOKEN_ID_IDENTIFIER);
        // 29
        this->se->isTokenNotHasDeclaration(tokenExp5);
        if (token->getTokenid() == TOKEN_ID_OPEN_BRACKET)
        {
            matchToken(TOKEN_ID_OPEN_BRACKET);
            this->se->ifTokenTypehasDiff(token, TOKEN_TYPE_INTEGER);
            this->se->ifTokenVectorInRange(tokenExp5, token);
            cg->storeConstOnTmp(tokenExp5,token);
            cg->vectorAccess(&tokenID,tokenExp5,tokenExp5);
            matchToken(TOKEN_ID_CONSTANT);
            matchToken(TOKEN_ID_CLOSE_BRACKET);
        }
    }
    else
    {
        // relizar ajuste
        matchToken(TOKEN_ID_OPEN_PARANTHESES);
        Token a = productionExp();
        Token *result = new Token();
        result->setTokenID(a.getTokenid());
        result->setLexeme(a.getLexeme());
        result->setTokenType(a.getTokenType());
        result->setTokenSize(a.getTokeSize());
        result->setTokenClass(a.getTokenClass());
        result->setMaxTam(a.getMaxTam());
        result->setTokenAddr(a.getTokenAddr());

        matchToken(TOKEN_ID_CLOSE_PARANTHESES);
        tokenExp5 = result;
    }
    return tokenExp5;
}
