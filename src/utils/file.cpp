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
        std::map<int,std::string> arq;
    public:
        FileHandler();
        std::map<int,std::string> mapFile(std::string path);
};

FileHandler::FileHandler(){

}
/**
 * @brief metodo que faz a leitura do arquivo do programa fonte e mapeia em um map com as linhas do arquivo
 * @return map com as linhas do arquivo
*/
std::map<int,std::string> FileHandler::mapFile(std::string path){
    std::ifstream arqf;
    std::string s;

    arqf.open(path);
    if(arqf.fail()){
        std::cout <<"Falha ao abrir arquivo \n";
    }else{
        for(int i=0;!arqf.eof();i++){
            getline(arqf,s);
            this->arq[i]=s;
            //std::cout << s;
            //std::cout << "\n";
        }
        arqf.close();
    }

    return this->arq;
}
#endif
/*int main(){
    FileHandler fh;
    fh.mapFile("C:/Users/iagom/Downloads/exemplo.l");
    return 0;
}*/