#include "parsing_utils.hpp"
#include <iostream>

namespace murzina
{
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        char c = '0';
        in >> c;

        if (in && (c != dest.exp))
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, DoubleLitIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        in >> dest.ref;

        if (in)
        {
            char suffix = '0';
            in >> suffix;

            if (suffix != 'd' && suffix != 'D')
            {
                in.setstate(std::ios::failbit);
            }
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, LabelIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        std::string data = "";
        if ((in >> StringIO{ data }) && (data != dest.exp))
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, RationalIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        long long numerator = 0;
        unsigned long long denominator = 0;

        // Формат: (:N -2:D 3:)
        in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' } >> DelimiterIO{ 'N' } >> DelimiterIO{ ' ' } >> numerator
           >> DelimiterIO{ ':' } >> DelimiterIO{ 'D' } >> DelimiterIO{ ' ' } >> denominator
           >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };

        if (in)
        {
            dest.ref = { numerator, denominator };
        }
        return in;
    }
}
