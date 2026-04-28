#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <string>
#include <complex>
#include <iostream>

//key1 - ULL HEX
//key2 - CMP LSP
//key3 - строка
struct DataStruct {
    unsigned long long key1_;
    std::complex<double> key2_;
    std::string key3_;

    DataStruct();

    DataStruct(unsigned long long key1,
        std::complex<double> key2,
        std::string key3);

    bool operator<(const DataStruct& other) const;

    bool operator==(const DataStruct& other) const;
};

std::istream& operator>>(std::istream& is, DataStruct& data);

std::ostream& operator<<(std::ostream& os, const DataStruct& data);

#endif
