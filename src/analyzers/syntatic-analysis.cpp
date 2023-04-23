#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <algorithm>
#include "../symbols/token.cpp"
#include "../error/l_exception.cpp"
#include "../symbols/token-type.cpp"

class SyntaticAnalysis
{
private:
    Token token;
    void setToken(Token token);
    void matchToken(TokenID expectedToken);
    void productionS();
    void productionD();
    void productionD1();
    void productionC();
    void productionCMD();
    void productionA();
    void productionR();
    void productionT();
    void productionL();
    void productionE();
    void productionExp();
public:
    SyntaticAnalysis(/* args */);
    ~SyntaticAnalysis();
    void Start(Token token);
};

SyntaticAnalysis::SyntaticAnalysis(/* args */)
{
}

SyntaticAnalysis::~SyntaticAnalysis()
{
}

void SyntaticAnalysis::Start(Token token)
{
 // Pedir um token pro amigo
 // Set token
    setToken(token);
    productionS();

}

void SyntaticAnalysis::setToken(Token token)
{
   this->token = token;
}

void  SyntaticAnalysis::matchToken(TokenID expectedToken){
    //Fazer o match
    if(expectedToken == token.getTokenid()){
        // Pedir o prx token
        Token nextToken;
        setToken(nextToken);


    }else{
        if(token.getTokenid() == TOKEN_ID_EOF){
            //Gerar erro de Fim de arquivo não esperado.
        }else{
            //Gerar erro de Token não esperado.
        }

    }
}

// S -> { ( D | Cmd) } fim_arquivo
void SyntaticAnalysis::productionS()
{
    while (token.getTokenid() !=  TOKEN_ID_EOF)
    {
        bool declaration = 
        token.getTokenid() == TOKEN_ID_FINAL || 
        token.getTokenid() == TOKEN_ID_INTEGER ||
        token.getTokenid() == TOKEN_ID_CHAR ||
        token.getTokenid() == TOKEN_ID_REAL ||
        token.getTokenid() == TOKEN_ID_STRING ||
        token.getTokenid() == TOKEN_ID_BOOLEAN;

        if(declaration){
            std::cout << "declaration" << std::endl;
            productionD();
        }
        else{
            productionCMD();
        }
        
        this->token.setTokenID(TOKEN_ID_EOF);
    }
    
}

// D -> D1 C { ,C } ; | final id = [-] const ;
void SyntaticAnalysis::productionD(){
    if(token.getTokenid() == TOKEN_ID_FINAL){
        matchToken(TOKEN_ID_FINAL);
        matchToken(TOKEN_ID_IDENTIFIER);
        matchToken(TOKEN_ID_ASSIGNMENT);
        if(token.getTokenid() == TOKEN_ID_SUBTRACTION)
        {
             matchToken(TOKEN_ID_SUBTRACTION);
        }
        matchToken(TOKEN_ID_CONSTANT);
    }
    else{
        productionD1();
        productionC();
        while (token.getTokenid() == TOKEN_ID_COMMA)
        {
            matchToken(TOKEN_ID_COMMA);
            productionC();
        }
        matchToken(TOKEN_ID_SEMICOLON);
    }

}

// D1 -> ( char | integer | real | boolean | string )
void SyntaticAnalysis::productionD1()
{

    switch (token.getTokenid())
    {
    case TOKEN_ID_CHAR:
        matchToken(TOKEN_ID_CHAR);
        break;
    case TOKEN_ID_INTEGER:
        matchToken(TOKEN_ID_INTEGER);
        break;    
    case TOKEN_ID_REAL:
        matchToken(TOKEN_ID_REAL);
        break;
    case TOKEN_ID_BOOLEAN:
        matchToken(TOKEN_ID_BOOLEAN);
    case TOKEN_ID_STRING:
        matchToken(TOKEN_ID_STRING);
        break;
    default:
        std::cout << "ERRO NÃO SEI COMO SE CONSEGUIU ESSA PROESA.";
        break;
    }
}

//C -> id [ ( = [ - ] const | [ ( id | num ) ] ) ]
void SyntaticAnalysis::productionC()
{
    matchToken(TOKEN_ID_IDENTIFIER);
    if(token.getTokenid() == TOKEN_ID_ASSIGNMENT) 
    {
        matchToken(TOKEN_ID_ASSIGNMENT);
        if(token.getTokenid() == TOKEN_ID_SUBTRACTION)
        {
             matchToken(TOKEN_ID_SUBTRACTION);
        }
        matchToken(TOKEN_ID_CONSTANT);
    }
    else{
        matchToken(TOKEN_ID_OPEN_BRACKET);
        if(token.getTokenid() == TOKEN_ID_IDENTIFIER)
        {
            matchToken(TOKEN_ID_IDENTIFIER);
        }
        else
        {
            matchToken(TOKEN_ID_CONSTANT);
        }

        matchToken(TOKEN_ID_CLOSE_BRACKET);
    }
}

// Cmd ->  [ ( A | R | T | L | E ) ] ;
void SyntaticAnalysis::productionCMD(){
    if(token.getTokenid() == TOKEN_ID_IDENTIFIER){
        productionA();
    }
    productionR();
    productionT();
    productionL();
    productionE();

}


void SyntaticAnalysis::productionA(){
    matchToken(TOKEN_ID_IDENTIFIER);
    productionExp();

}

void SyntaticAnalysis::productionR(){
  

}

void SyntaticAnalysis::productionT(){
  

}

void SyntaticAnalysis::productionL(){
  

}

void SyntaticAnalysis::productionE(){
  

}

void SyntaticAnalysis::productionExp(){
  

}


int main(){

  std:: cout<< "Inicio do teste" << std::endl;
    SyntaticAnalysis *sintatico = new SyntaticAnalysis();
    Token *token = new Token();
    token->setTokenID(TOKEN_ID_CHAR);
    std::string a = tokenToString(token->getTokenid());
    std::cout << a ;

    sintatico->Start(*token);


    

}
