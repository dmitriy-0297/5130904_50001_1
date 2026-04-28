#ifndef PARSING_UTILS_HPP
#define PARSING_UTILS_HPP

#include <iostream>
#include <string>
#include <utility>

namespace murzina
{
    struct DelimiterIO
    {
        char exp;
    };

    struct DoubleLitIO
    {
        double& ref;
    };

    struct StringIO
    {
        std::string& ref;
    };

    struct LabelIO
    {
        std::string exp;
    };

    struct RationalIO
    {
        std::pair<long long, unsigned long long>& ref;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, DoubleLitIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, LabelIO&& dest);
    std::istream& operator>>(std::istream& in, RationalIO&& dest);
}

#endif
