#ifndef UTILS_CONSTANTES
#define UTILS_CONSTANTES
/**
 * @brief classe responsável por armazenar constantes
 *
 */

/// @brief Número maximo de caracteres de um identificador
const static int CONSTANTS_IDENTIFIER_MAX_SIZE = 32;
/// @brief Limite da casas de precissão de FLOT
const static int CONSTANTS_MAXIMUM_ACCURACY_LENGTH = 6;
/// @brief Valores minimo(-2^(31)) e maximo((2^(31))-1) de que um valor inteiro pode possuir
const static int CONSTANTS_INTEGER_MIN_VALUE = -2147483648;
const static int CONSTANTS_INTEGER_MAX_VALUE = 2147483647;

/// @brief Valores minimo e maximo de que um valor inteiro pode possuir
const static float CONSTANTS_REAL_MIN_VALUE = -99999.9;
const static float CONSTANTS_REAL_MAX_VALUE = 99999.9;

/// @brief espaço ocupado por cada variavel
const static int CONSTANTS_CHAR_SIZE = 0x1;
const static int CONSTANTS_BOOLEAN_SIZE = 0X1;
const static int CONSTANTS_STRING_SIZE = 0x1;
const static int CONSTANTS_NUMERIC_SIZE = 0x4;

#endif