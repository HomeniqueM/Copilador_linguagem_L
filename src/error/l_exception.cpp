#ifndef ERROR_ERROR_CODE
#define ERROR_ERROR_CODE
#include <string>
#include <exception>
#include <sstream>
#include "../utils/constants.cpp"
enum class ErrorCode
{
    INVALIDCHARACTER,
    UNEXPECTED_CHARACTER,
    UNEXPECTED_TOKEN,
    UNEXPECTED_TOKEN_EOF,
    UNEXPECTED_END_OFFILE,
    NO_FILE_PATH_FOUND,
    OVERFLOW_SIZE_INTEGER,
    OVERFLOW_SIZE_REAL,
    UNDERFLOW_SIZE_INTEGER,
    UNDERFLOW_SIZE_REAL,
    OVERFLOW_ACCURACY_LENGTH,
    ENCEEDED_LIMIT_IDENTIFIER_MAX_SIZE,
    FILE_OPENNING_FAIL,
    UNKNOWN
};

/**
 * @brief classe para criar erros customizado para o compilador da linguagem L
 */
class LException : public std::exception
{
public:
    LException(ErrorCode code, int line, const std::string &message = "") : code_(code), line_(line), mOptional_(message) {}

    const char *what() const noexcept override
    {
        return message_.c_str();
    }

    ErrorCode code() const { return code_; }
    int line() const { return line_; }

private:
    ErrorCode code_;
    int line_;
    std::string mOptional_;
    std::string message_ = buildMessage();
    std::string buildMessage() const
    {
        std::ostringstream oss;
        oss << "Error: ";
        oss << " [" << line_ << "] ";
        switch (code_)
        {
        case ErrorCode::INVALIDCHARACTER:
            oss << "character não é valido";
            break;
        case ErrorCode::UNEXPECTED_CHARACTER:
            oss << "character não era esperado ";
            break;
        case ErrorCode::UNEXPECTED_TOKEN:
            oss << "token nao esperado";
            break;
        case ErrorCode::UNEXPECTED_TOKEN_EOF:
            oss << "fim de arquivo nao esperado";
            break;
        case ErrorCode::NO_FILE_PATH_FOUND:
            oss << "caminho do arquivo fonte nao encontrado";
            break;
        case ErrorCode::ENCEEDED_LIMIT_IDENTIFIER_MAX_SIZE:
            oss << "Identificadores podem somente possui " << CONSTANTS_IDENTIFIER_MAX_SIZE << " caracteres";
            break;
        case ErrorCode::OVERFLOW_ACCURACY_LENGTH:
            oss << "Valor de acuracia maximo excedido";
        case ErrorCode::OVERFLOW_SIZE_REAL:
            oss << "Valor maximo excedido, valor maximo suportado "<<CONSTANTS_REAL_MAX_VALUE;
        case ErrorCode::UNDERFLOW_SIZE_REAL:
            oss << "Valor minimo nao suportado "<<CONSTANTS_REAL_MIN_VALUE;
        case ErrorCode::OVERFLOW_SIZE_INTEGER:
            oss << "Valor maximo excedido, valor maximo suportado "<<CONSTANTS_INTEGER_MAX_VALUE;
        case ErrorCode::UNDERFLOW_SIZE_INTEGER:
            oss << "Valor minimo nao suportado "<<CONSTANTS_INTEGER_MIN_VALUE;
        case ErrorCode::FILE_OPENNING_FAIL:
            oss << "Falha ao abrir arquivo";
        default:
            oss << "Unknown error";
            break;
        }
        if (!mOptional_.empty())
        {
            oss << " - " << mOptional_;
        }
        return oss.str();
    }
};
#endif