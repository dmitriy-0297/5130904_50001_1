#include "dictInserter.hpp"
#include <algorithm>
#include <iterator>
#include <list>
#include <map>
#include <string>
#include <utility>

using ERDict = std::map<std::wstring, std::list<std::wstring>>;

DictInserter::DictInserter(ERDict &dict) :
    dict_(&dict)
{}

DictInserter &DictInserter::operator=(const DictPair &pair) {
    if (!pair.word.empty()) {
        std::pair<ERDict::iterator, bool> insertion = dict_->insert(
            std::pair<std::wstring, std::list<std::wstring>>{ pair.word, { pair.translation } });

        if (insertion.second == false) {

            std::list<std::wstring> &translations = insertion.first->second;

            std::list<std::wstring>::iterator pos = std::find_if(
                translations.begin(),
                translations.end(),
                [&pair](const std::wstring &s) {
                    return pair.translation <= s;
                }
            );

            if (pos == translations.end() || *pos != pair.translation) {
                translations.insert(pos, pair.translation);
            }
        }
    }

    return *this;
}
