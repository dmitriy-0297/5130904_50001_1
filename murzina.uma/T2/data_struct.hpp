#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <iostream>
#include <string>
#include <utility>

namespace murzina
{
    struct DataStruct
    {
        double key1;
        std::pair<long long, unsigned long long> key2;
        std::string key3;
    };

    std::istream& operator>>(std::istream& in, DataStruct& dest);
    std::ostream& operator<<(std::ostream& out, const DataStruct& src);
    bool compareDataStruct(const DataStruct& lhs, const DataStruct& rhs);
}

#endif
