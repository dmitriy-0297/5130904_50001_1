#ifndef DICT_OUTPUT_ITERATOR_H
#define DICT_OUTPUT_ITERATOR_H

#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <string>
#include <utility>

using ERDict = std::map<std::wstring, std::list<std::wstring>>;

class DictOutputIterator {
public:

    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;

    explicit DictOutputIterator(std::wostream &stream);

    DictOutputIterator &operator=(const std::pair<const std::wstring, std::list<std::wstring>> &pair);

    DictOutputIterator &operator*() { return *this; }
    DictOutputIterator &operator++() { return *this; }
    DictOutputIterator &operator++(int) { return *this; }

private:

    std::wostream *stream_;

};

#endif
