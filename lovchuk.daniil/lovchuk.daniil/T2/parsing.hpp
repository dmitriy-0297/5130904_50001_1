#ifndef PARSING_HPP
#define PARSING_HPP

#include <string>
#include <vector>
#include <complex>

//парсинг 0xFF, 0Xff
bool parseHexLiteral(const std::string& value, unsigned long long& result);

//парсинг #c(real imag) или #c(real-imag)
bool parseComplexLiteral(const std::string& value,
    std::complex<double>& result);

//парсинг "text"
bool parseStringLiteral(const std::string& value, std::string& result);

//извлекаем значения поля из формата :keyName value:
bool extractFieldValue(const std::string& token, std::string& fieldName,
    std::string& fieldValue);

#endif
