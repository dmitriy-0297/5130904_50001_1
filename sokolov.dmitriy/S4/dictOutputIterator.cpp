#include "dictOutputIterator.hpp"

#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <string>
#include <utility>

using ERDict = std::map<std::wstring, std::list<std::wstring>>;

DictOutputIterator::DictOutputIterator(std::wostream &stream) :
    stream_(&stream)
{}

DictOutputIterator &DictOutputIterator::operator=
    (const std::pair<const std::wstring, std::list<std::wstring>> &pair) {

    std::wostream::sentry sentry(*stream_);

    if (!sentry) {
        return *this;
    }

    *stream_ << L"- " << pair.first << L": ";

    std::copy(
        pair.second.cbegin(),
        pair.second.cend(),
        std::ostream_iterator<std::wstring, wchar_t>(*stream_, L" ")
    );

    *stream_ << L"\n";

    return *this;
}
