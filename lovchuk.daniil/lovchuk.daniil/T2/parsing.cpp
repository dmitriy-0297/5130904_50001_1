#include "parsing.hpp"
#include <sstream>

using namespace std;

bool parseHexLiteral(const string& value, unsigned long long& result) {
    if (value.length() < 3) {
        return false;
    }
    if (value[0] != '0' || (value[1] != 'x' && value[1] != 'X')) {
        return false;
    }
    try {
        size_t pos = 0;
        result = stoull(value.substr(2), &pos, 16);
        return pos == value.length() - 2;
    }
    catch (...) {
        return false;
    }
}

bool parseComplexLiteral(const string& value, complex<double>& result) {
    if (value.length() < 5) {
        return false;
    }
    if (value[0] != '#' || value[1] != 'c' || value[2] != '(') {
        return false;
    }
    if (value.back() != ')') {
        return false;
    }
    string inner = value.substr(3, value.length() - 4);
    istringstream iss(inner);
    double real, imag;
    if (!(iss >> real >> imag)) {
        return false;
    }
    result = complex<double>(real, imag);
    return true;
}

bool parseStringLiteral(const string& value, string& result) {
    if (value.length() < 2) {
        return false;
    }
    if (value.front() != '"' || value.back() != '"') {
        return false;
    }
    result = value.substr(1, value.length() - 2);
    return true;
}

bool extractFieldValue(const string& token, string& fieldName,
    string& fieldValue) {
    if (token.length() < 3) {
        return false;
    }
    if (token[0] != ':') {
        return false;
    }
    size_t spacePos = token.find(' ', 1);
    if (spacePos == string::npos) {
        return false;
    }
    fieldName = token.substr(1, spacePos - 1);
    fieldValue = token.substr(spacePos + 1);
    // Убираем завершающее двоеточие если есть
    if (!fieldValue.empty() && fieldValue.back() == ':') {
        fieldValue.pop_back();
    }
    return true;
}
