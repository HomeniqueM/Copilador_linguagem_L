/**
 * Pontificia Universidade Católica de Minas Gerais
 * ICEI - Departamento de Ciência da Computação
 * Disciplina de Compiladores
 * Prof Alexei Machado
 * @authors Guilherme Côsso Lima Pimenta, Homenique Vieira Martins, Iago Augusto Coelho Morgado
 */
#ifndef ERROR_ERROR_CODE
#define ERROR_ERROR_CODE
#include <string>
#include <exception>
#include <sstream>
#include <map>
#include "../utils/constants.hpp"

enum class ErrorCode
{
    // Erros solicitados
    INVALIDCHARACTER, // Pdf 01
    INVALIDLEX,       // PDF 01
    UNEXPECTED_EOF,   // PDF 01
    UNEXPECTED_TOKEN, // PDF 02
    UNDECLARED_IDENTIFIER, // PDF 03
    IDENTIFIER_ALREADY_DECLARED, // PDF 03
    MISMATCHED_IDENTIFIER,
    INCOMPATIBLE_TYPES,
    // Erros Customizados
    NO_FILE_PATH_FOUND,
    UNEXPECTED_CHARACTER,
    OVERFLOW_SIZE_INTEGER,
    OVERFLOW_SIZE_REAL,
    UNDERFLOW_SIZE_INTEGER,
    UNDERFLOW_SIZE_REAL,
    OVERFLOW_ACCURACY_LENGTH,
    ENCEEDED_LIMIT_IDENTIFIER_MAX_SIZE,
    FILE_OPENNING_FAIL,
    UNKNOWN
};

// Mapeamento de códigos de erro para mensagens de erro
const std::map<ErrorCode, std::string_view> errorMessages = {
    {ErrorCode::INVALIDCHARACTER, "character invalido"},
    {ErrorCode::UNEXPECTED_CHARACTER, "character nao esperado"},
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
        oss << line_ << ": ";
        switch (code_)
        {
        case ErrorCode::INVALIDCHARACTER:
            oss << "caractere invalido";
            break;
        case ErrorCode::UNEXPECTED_CHARACTER:
            oss << "caractere invalido";
            break;
        case ErrorCode::INVALIDLEX:
            oss << "lexema nao identificado";
            break;
        case ErrorCode::UNEXPECTED_TOKEN:
            oss << "token nao esperado";
            break;
        case ErrorCode::UNEXPECTED_EOF:
            oss << "fim de arquivo nao esperado";
            break;
        case ErrorCode::NO_FILE_PATH_FOUND:
            oss << "caminho do arquivo fonte nao encontrado";
            break;
        case ErrorCode::ENCEEDED_LIMIT_IDENTIFIER_MAX_SIZE:
            oss << "identificadores podem somente possui " << CONSTANTS_IDENTIFIER_MAX_SIZE << " caracteres";
            break;
        case ErrorCode::OVERFLOW_ACCURACY_LENGTH:
            oss << "Valor de acuracia maximo excedido";
            break;
        case ErrorCode::OVERFLOW_SIZE_REAL:
            oss << "Valor maximo excedido, valor maximo suportado " << CONSTANTS_REAL_MAX_VALUE;
            break;
        case ErrorCode::UNDERFLOW_SIZE_REAL:
            oss << "Valor minimo nao suportado " << CONSTANTS_REAL_MIN_VALUE;
            break;
        case ErrorCode::OVERFLOW_SIZE_INTEGER:
            oss << "Valor maximo excedido, valor maximo suportado " << CONSTANTS_INTEGER_MAX_VALUE;
            break;
        case ErrorCode::UNDERFLOW_SIZE_INTEGER:
            oss << "Valor minimo nao suportado " << CONSTANTS_INTEGER_MIN_VALUE;
            break;
        case ErrorCode::FILE_OPENNING_FAIL:
            oss << "Falha ao abrir arquivo";
            break;
        case ErrorCode::UNDECLARED_IDENTIFIER:
            oss << "identificador nao declarado";
            break;
        case ErrorCode::IDENTIFIER_ALREADY_DECLARED:
            oss << "identificador ja declarado";
            break;
        case ErrorCode::MISMATCHED_IDENTIFIER:
            oss << "classe de identificador incompatível";
            break;
        case ErrorCode::INCOMPATIBLE_TYPES:
            oss << "ipos incompativeis.";
            break;
        default:
            oss << "Unknown error";
            break;
        }
        if (!mOptional_.empty())
        {
            oss << " [" << mOptional_ << "].";
        }
        return oss.str();
    }
};
#endif