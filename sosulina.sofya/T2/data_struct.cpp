#include "data_struct.hpp"
#include "parser.hpp"

#include <iomanip>
#include <string>

bool compareData(const DataStruct& left, const DataStruct& right)
{
    if (left.key1 != right.key1)
    {
        return left.key1 < right.key1;
    }

    if (left.key2 != right.key2)
    {
        return left.key2 < right.key2;
    }

    return left.key3.length() < right.key3.length();
}

std::istream& operator>>(std::istream& stream, DataStruct& data)
{
    std::string rawLine;

    while (std::getline(stream, rawLine))
    {
        std::size_t startTrim = rawLine.find_first_not_of(" \t\n\r\f\v");

        if (std::string::npos == startTrim)
        {
            continue;
        }

        std::size_t endTrim = rawLine.find_last_not_of(" \t\n\r\f\v");
        std::string cleanLine = rawLine.substr(startTrim, endTrim - startTrim + 1);

        if (cleanLine.empty())
        {
            continue;
        }

        bool parseSuccess = parseRecordString(cleanLine, data);

        if (parseSuccess)
        {
            return stream;
        }
    }

    stream.setstate(std::ios::failbit);
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const DataStruct& data)
{
    stream << "(:key1 ";
    // Формат DBL LIT требует точку и десятичные доли
    stream << std::fixed << std::setprecision(10) << data.key1;
    stream << "d";
    stream << ":key2 ";
    // Формат ULL LIT требует суффикс ull
    stream << data.key2;
    stream << "ull";
    stream << ":key3 \"";
    stream << data.key3;
    stream << "\":)";

    return stream;
}
