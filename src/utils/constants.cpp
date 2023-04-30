#ifndef UTILS_CONSTANTES
#define UTILS_CONSTANTES
/**
 * @brief classe responsável por armazenar constantes
 *
 */
class Constants
{
public:
    /// @brief Número maximo de caracteres de um identificador
    const int IDENTIFIER_MAX_SIZE = 32;

    /// @brief Valores minimo(-2^(31)) e maximo((2^(31))-1) de que um valor inteiro pode possuir
    const int INTEGER_MIN_VALUE = -2147483648;
    const int INTEGER_MAX_VALUE = 2147483647;

    /// @brief Valores minimo e maximo de que um valor inteiro pode possuir
    const float REAL_MIN_VALUE = -99999.9;
    const float REAL_MAX_VALUE = 99999.9;

    /// @brief espaço ocupado por cada variavel 
    int CHAR_SIZE = 0x1;
    int BOOLEAN_SIZE = 0X1;
    int STRING_SIZE = 0x1;
    int NUMERIC_SIZE = 0x4;
};

#endif
