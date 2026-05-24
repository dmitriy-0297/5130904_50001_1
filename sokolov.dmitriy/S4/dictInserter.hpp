#ifndef DICT_INSERTER_H
#define DICT_INSERTER_H

#include "dictPair.hpp"
#include <iterator>
#include <list>
#include <map>
#include <string>

using ERDict = std::map<std::wstring, std::list<std::wstring>>;

class DictInserter {
public:

    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;

    explicit DictInserter(ERDict &dict);

    DictInserter &operator=(const DictPair &pair);

    DictInserter &operator*() { return *this; }
    DictInserter &operator++() { return *this; }
    DictInserter &operator++(int) { return *this; }

private:

    ERDict *dict_;

};

#endif
