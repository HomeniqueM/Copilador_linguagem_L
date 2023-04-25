#ifndef UTILS_ARG_HANDLER
#define UTILS_ARG_HANDLER
#include <string>
#include "../error/l_exception.cpp"

class ArgHandler{
    public:
        std::string handleArgs(int argc, char const *argv[]);
};

std::string ArgHandler::handleArgs(int argc, char const *argv[]){
    std::string path;
    if(argc < 2){
        throw LException(ErrorCode::NO_FILE_PATH_FOUND,__LINE__);
    }else{
        path = argv[1];
    }
    return path;
} 
#endif