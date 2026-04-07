#include "DataStruct.hpp"
#include "parsing.hpp"
#include <iomanip>
#include <cmath>

using namespace std;

DataStruct::DataStruct()
    : key1_(0)
    , key2_(0.0, 0.0)
    , key3_("") {
}

DataStruct::DataStruct(unsigned long long key1,
    complex<double> key2,
    string key3)
    : key1_(key1)
    , key2_(key2)
    , key3_(key3) {
}

bool DataStruct::operator<(const DataStruct& other) const {
    //сортировка по key1 (по возрастанию)
    if (key1_ != other.key1_) {
        return key1_ < other.key1_;
    }
    //сортировка по key2 (по модулю комплексного числа)
    double mod1 = abs(key2_);
    double mod2 = abs(other.key2_);
    if (mod1 != mod2) {
        return mod1 < mod2;
    }
    //сортировка по длине key3
    return key3_.length() < other.key3_.length();
}

bool DataStruct::operator==(const DataStruct& other) const {
    return key1_ == other.key1_
        && key2_ == other.key2_
        && key3_ == other.key3_;
}

istream& operator>>(istream& is, DataStruct& data) {
    string line;
    if (!getline(is, line)) {
        is.setstate(ios::failbit);
        return is;
    }

    //проверка на скобочки ()
    if (line.empty() || line[0] != '(' || line.back() != ')') {
        is.setstate(ios::failbit);
        return is;
    }

    //убираем внешние скобки
    string content = line.substr(1, line.length() - 2);

    vector<string> tokens;
    string token;
    for (size_t i = 0; i < content.length(); ++i) {
        if (content[i] == ':') {
            if (!token.empty()) {
                tokens.push_back(":" + token + ":");
            }
            token.clear();
        }
        else {
            token += content[i];
        }
    }
    if (!token.empty()) {
        tokens.push_back(":" + token + ":");
    }

    //парсим каждое из полей
    bool hasKey1 = false;
    bool hasKey2 = false;
    bool hasKey3 = false;

    for (const auto& tok : tokens) {
        string fieldName, fieldValue;
        if (!extractFieldValue(tok, fieldName, fieldValue)) {
            continue;
        }

        if (fieldName == "key1") {
            if (parseHexLiteral(fieldValue, data.key1_)) {
                hasKey1 = true;
            }
        }
        else if (fieldName == "key2") {
            if (parseComplexLiteral(fieldValue, data.key2_)) {
                hasKey2 = true;
            }
        }
        else if (fieldName == "key3") {
            if (parseStringLiteral(fieldValue, data.key3_)) {
                hasKey3 = true;
            }
        }
    }

    //ошибка выводится, если хотя бы одно поле не распарисилось
    if (!hasKey1 || !hasKey2 || !hasKey3) {
        is.setstate(ios::failbit);
    }

    return is;
}

ostream& operator<<(ostream& os, const DataStruct& data) {
    os << "(:key1 " << hex << uppercase << data.key1_
        << ":key2 #c(" << fixed << setprecision(1)
        << data.key2_.real();
    if (data.key2_.imag() >= 0) {
        os << " ";
    }
    os << data.key2_.imag() << "):key3 \""
        << data.key3_ << "\":)";
    return os;
}
