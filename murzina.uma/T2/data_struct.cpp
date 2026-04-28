#include "data_struct.hpp"
#include "parsing_utils.hpp"
#include <iomanip>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <cctype>

namespace murzina
{
    std::istream& operator>>(std::istream& in, DataStruct& dest)
    {
        std::string line;
        if (!std::getline(in, line))
        {
            return in;
        }

        if (line.empty())
        {
            return in;
        }

        size_t pos = 0;

        double key1_val = 0.0;
        long long num = 0;
        unsigned long long den = 0;
        std::string key3_val;

        bool key1_found = false;
        bool key2_found = false;
        bool key3_found = false;

        while (pos < line.length())
        {
            if (line[pos] == ':' && pos + 4 < line.length())
            {
                if (line.substr(pos + 1, 4) == "key1")
                {
                    pos += 5;
                    while (pos < line.length() && std::isspace(line[pos])) pos++;

                    std::string num_str;
                    while (pos < line.length() && (std::isdigit(line[pos]) || line[pos] == '.' || line[pos] == '-' || line[pos] == '+'))
                    {
                        num_str += line[pos];
                        pos++;
                    }

                    if (pos < line.length() && (line[pos] == 'd' || line[pos] == 'D'))
                    {
                        pos++;
                    }

                    key1_val = std::stod(num_str);
                    key1_found = true;
                }
                else if (line.substr(pos + 1, 4) == "key2")
                {
                    pos += 5;
                    while (pos < line.length() && std::isspace(line[pos])) pos++;

                    if (pos < line.length() && line[pos] == '(')
                    {
                        pos++;
                    }

                    while (pos < line.length() && (line[pos] == ':' || std::isspace(line[pos]))) pos++;

                    if (pos + 1 < line.length() && line[pos] == 'N')
                    {
                        pos++;
                        while (pos < line.length() && std::isspace(line[pos])) pos++;

                        std::string num_str;
                        while (pos < line.length() && (std::isdigit(line[pos]) || line[pos] == '-'))
                        {
                            num_str += line[pos];
                            pos++;
                        }
                        num = std::stoll(num_str);
                    }

                    while (pos < line.length() && (line[pos] == ':' || std::isspace(line[pos]))) pos++;

                    if (pos + 1 < line.length() && line[pos] == 'D')
                    {
                        pos++;
                        while (pos < line.length() && std::isspace(line[pos])) pos++;

                        std::string den_str;
                        while (pos < line.length() && std::isdigit(line[pos]))
                        {
                            den_str += line[pos];
                            pos++;
                        }
                        den = std::stoull(den_str);
                    }

                    key2_found = true;
                }
                else if (line.substr(pos + 1, 4) == "key3")
                {
                    pos += 5;
                    while (pos < line.length() && std::isspace(line[pos])) pos++;

                    if (pos < line.length() && line[pos] == '"')
                    {
                        pos++;
                        key3_val.clear();
                        while (pos < line.length() && line[pos] != '"')
                        {
                            key3_val += line[pos];
                            pos++;
                        }
                        if (pos < line.length() && line[pos] == '"')
                        {
                            pos++;
                        }
                    }

                    key3_found = true;
                }
                else
                {
                    pos++;
                }
            }
            else
            {
                pos++;
            }
        }

        if (key1_found && key2_found && key3_found)
        {
            dest.key1 = key1_val;
            dest.key2 = { num, den };
            dest.key3 = key3_val;
        }
        else
        {
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
        if (std::abs(lhs.key1 - rhs.key1) > 1e-9)
        {
            return lhs.key1 < rhs.key1;
        }

        double lhs_ratio = static_cast<double>(lhs.key2.first) / lhs.key2.second;
        double rhs_ratio = static_cast<double>(rhs.key2.first) / rhs.key2.second;

        if (std::abs(lhs_ratio - rhs_ratio) > 1e-9)
        {
            return lhs_ratio < rhs_ratio;
        }

        return lhs.key3.length() < rhs.key3.length();
    }
}
