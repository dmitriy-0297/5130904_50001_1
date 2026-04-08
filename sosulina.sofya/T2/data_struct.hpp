#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <string>
#include <iostream>

struct DataStruct
{
    double key1;
    unsigned long long key2;
    std::string key3;
};

bool compareData(const DataStruct& left, const DataStruct& right);
std::istream& operator>>(std::istream& stream, DataStruct& data);
std::ostream& operator<<(std::ostream& stream, const DataStruct& data);

#endif
