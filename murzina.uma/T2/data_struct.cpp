#include "data_struct.hpp"
#include <iomanip>
#include <sstream>
#include <string>
#include <cctype>
#include <regex>

namespace murzina
{
    std::istream& operator>>(std::istream& in, DataStruct& dest)
    {
        std::string line;
        if (!std::getline(in, line)) {
            return in;
        }

        // Пропускаем пустые строки
        if (line.empty()) {
            return in;
        }

        // Регулярное выражение для поиска ключей
        std::regex key1_regex(R"(key1\s+([0-9]+\.?[0-9]*)(?:d|D)?)");
        std::regex key2_regex(R"(key2\s+\(\s*:\s*N\s+(-?[0-9]+)\s*:\s*D\s+([0-9]+)\s*:\s*\))");
        std::regex key3_regex(R"(key3\s+\"([^\"]*)\")");

        std::smatch match;
        double key1_val = 0.0;
        long long num = 0;
        unsigned long long den = 1;
        std::string key3_val;

        bool key1_found = false;
        bool key2_found = false;
        bool key3_found = false;

        // Ищем key1
        if (std::regex_search(line, match, key1_regex)) {
            key1_val = std::stod(match[1].str());
            key1_found = true;
        }

        // Ищем key2
        if (std::regex_search(line, match, key2_regex)) {
            num = std::stoll(match[1].str());
            den = std::stoull(match[2].str());
            if (den == 0) den = 1;
            key2_found = true;
        }

        // Ищем key3
        if (std::regex_search(line, match, key3_regex)) {
            key3_val = match[1].str();
            key3_found = true;
        }

        // Если нашли все три ключа - заполняем структуру
        if (key1_found && key2_found && key3_found) {
            dest.key1 = key1_val;
            dest.key2 = {num, den};
            dest.key3 = key3_val;
        } else {
            // Неверный формат - пропускаем
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& src)
    {
        out << "(:";
        out << "key1 " << std::fixed << std::setprecision(1) << src.key1 << "d:";
        out << "key2 (:N " << src.key2.first << ":D " << src.key2.second << ":):";
        out << "key3 \"" << src.key3 << "\":";
        out << ")";
        return out;
    }

    bool compareDataStruct(const DataStruct& lhs, const DataStruct& rhs)
    {
        if (std::abs(lhs.key1 - rhs.key1) > 1e-9) {
            return lhs.key1 < rhs.key1;
        }

        double lhs_ratio = (lhs.key2.second != 0) ?
            static_cast<double>(lhs.key2.first) / lhs.key2.second : 0.0;
        double rhs_ratio = (rhs.key2.second != 0) ?
            static_cast<double>(rhs.key2.first) / rhs.key2.second : 0.0;

        if (std::abs(lhs_ratio - rhs_ratio) > 1e-9) {
            return lhs_ratio < rhs_ratio;
        }

        return lhs.key3.length() < rhs.key3.length();
    }
}
