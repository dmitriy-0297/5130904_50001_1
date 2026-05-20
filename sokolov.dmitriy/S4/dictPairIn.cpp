#include "toUpperCase.hpp"
#include "dictPair.hpp"
#include "dictPairIn.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

std::wistream &operator>>(std::wistream &stream, DictPair &pair) {

    std::wistream::sentry sentry(stream);

    if (!sentry) {
        return stream;
    }

    std::wstring line;

    DictPair temp{ L"", L"" };

    if (std::getline(stream, line)) {

        std::wistringstream iws(line);

        if (!(iws >> temp.word)) {
            stream.setstate(std::ios::failbit);
            return stream;
        }
        if (!std::getline(iws >> std::ws, temp.translation)) {
            stream.setstate(std::ios::failbit);
            return stream;
        }

        toUpperCase(temp.word);
        toUpperCase(temp.translation);
    }

    if (stream) {
        pair = std::move(temp);
    }

    return stream;
}
