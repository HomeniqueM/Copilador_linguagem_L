/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
*/
#include "file.hpp"
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
        //criar exception
        throw LException(ErrorCode::FILE_OPENNING_FAIL,0);
    }else{
        for(int i=0;!arqf.eof();i++){
            getline(arqf,s);
            s.append("\n");
            this->fileContent[i] = s;
        }
        arqf.close();
    }
    this->line = this->line_pos = 0;
}
/**
 * @brief Metodo que retorna o proximo caractere do arquivo do programa fonte
*/
char FileHandler::getNextFileChar(){
    char res;
    //Se solicitado um caractere em uma linha que não existe no arquivo res recebe \0 
    if(this->isEof()){
        res='\0';
    }else{
        //Verifica se foi solicitado um caractere em uma posição não existente da linha
        if(this->line_pos >= this->fileContent[line].size()){
            //caso exista a proxima linha no arquivo passa para a proxima linha no primeiro caractere da linha
            if((this->line+1) < this->fileContent.size()){
                this->line++;
                this->line_pos = 0;
                res = this->fileContent[this->line][this->line_pos];
                this->line_pos++;
            //caso não exista a proxima linha do arquivo res recebe \0
            }else{
                this->line++;
                res='\0';
            }
        //res recebe o caractere da linha na posição correta
        }else{ 
            res = this->fileContent[this->line][this->line_pos];
            this->line_pos++;
        }
    }
    return res;
}
/**
 * @brief retorna a linha atual do arquivo
*/
int FileHandler::getFileLine(){
    return static_cast<int>(this->line+1);
}
/**
 * @brief retorna a posição atual da linha do arquivo
*/
int FileHandler::getFileLinePos(){
    return static_cast<int>(this->line_pos);
}
/**
 * @brief retorna se o fim do arquivo foi aingido
*/
bool FileHandler::isEof(){
    return this->line >= this->fileContent.size() ? true : false;
}
/**
 * @brief retorna um caractere na linha 
*/
void FileHandler:: setPrevChar(){
    this->line_pos--;
}
