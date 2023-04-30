#ifndef UTILS_FILE
#define UTILS_FILE
#include <iostream>
#include <fstream>
#include <string>
#include <map>
/**
 * @brief Classe do utilitario de leitura do arquivo do programa fonte a ser compilado
*/
class FileHandler{
    private:
        std::string fileContent = "";
    public:
        FileHandler(std::string path);
        std::string getFileContent();
};
/**
 * @brief Construtor da classe do utilitario de leitura do arquivo do programa fonte
 * @param string com o path para o arquivo do programa fonte a ser compilado
*/
FileHandler::FileHandler(std::string path){
    std::ifstream arqf;
    std::string s;
    //abertura do arquivo do programa fonte.
    arqf.open(path);
    //Em caso de sucesso ao abrir o arquivo escreve as linhas do arquivo em um string
    //caso contrário gera uma mensagem de erro.
    if(arqf.fail()){
        std::cout <<"Falha ao abrir arquivo \n";
    }else{
        for(int i=0;!arqf.eof();i++){
            getline(arqf,s);
            this->fileContent.append(s);
            this->fileContent.append("\n");
        }
        arqf.close();
    }
}
/**
 * @brief Metodo que retorna a string com o conteudo do programa fonte
 * @return conteudo do programa fonte em formato de string
*/
std::string FileHandler::getFileContent(){
    return this->fileContent;
}

#endif
/*int main(){
    FileHandler fh("C:/Users/iagom/Downloads/exemplo.l");
    std::cout << fh.getFileContent();
    return 0;
}*/