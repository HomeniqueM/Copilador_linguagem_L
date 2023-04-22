#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>

typedef struct StatePackage
{
    bool returnChar = false; // deve voltar um char 
    std::string identifier; // onde o lexima é montado
};
