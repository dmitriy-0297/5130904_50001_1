#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include "data_struct.hpp"

double parseDoubleLiteral(const std::string& valueText);
unsigned long long parseUllLiteral(const std::string& valueText);
std::string parseQuotedString(const std::string& valueText);
bool parseRecordString(const std::string& recordStr, DataStruct& data);

#endif
