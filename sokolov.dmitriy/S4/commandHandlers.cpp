#include "commandHandlers.hpp"
#include "dictInserter.hpp"
#include "dictOutputIterator.hpp"
#include "dictPair.hpp"
#include "dictPairIn.hpp"
#include "toUpperCase.hpp"
#include <algorithm>
#include <cwchar>
#include <cwctype>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <locale>
#include <map>
#include <sstream>
#include <string>
#include <utility>

using ERDict = std::map<std::wstring, std::list<std::wstring>>;

const std::wstring ERROR_INVALID_COMMAND = L"- <INVALID COMMAND>\n\n";
const std::wstring ERROR_CANNOT_OPEN_FILE = L"- <CANNOT OPEN FILE>\n\n";

void handleHelp() {

    std::wcout << L"====================\n";
    std::wcout << L"Available commands:\n";
    std::wcout << L"====================\n";
    std::wcout << L"HELP\n";
    std::wcout << L"  Prints this list\n\n";
    std::wcout << L"INSERT <ENGLISH WORD> <TRANSLATION>\n";
    std::wcout << L"  Adds a translation to the dictionary\n\n";
    std::wcout << L"RMTR <ENGLISH WORD> <TRANSLATION>\n";
    std::wcout << L"  Removes a translation from the dictionary\n\n";
    std::wcout << L"RMEW <ENGLISH WORD>\n";
    std::wcout << L"  Removes an English word and all its translations from the dictionary\n\n";
    std::wcout << L"SEARCH <ENGLISH WORD> <TRANSLATION>\n";
    std::wcout << L"  Searches if the translation of a word exists in the dictionary\n\n";
    std::wcout << L"GET <ENGLISH WORD>\n";
    std::wcout << L"  Gets the translations of a word from the dictionary\n\n";
    std::wcout << L"NON\n";
    std::wcout << L"  Gets the number of nodes in the tree\n\n";
    std::wcout << L"OUT\n";
    std::wcout << L"  Prints the whole tree\n\n";
    std::wcout << L"LOAD <FILENAME>\n";
    std::wcout << L"  Loads translations from a file\n\n";
    std::wcout << L"====================\n";
    std::wcout << L"QUIT - Quit\n";
    std::wcout << L"====================\n\n";

}

void handleInsert(std::wistringstream &args, ERDict &dict) {

    std::wstring word = L"";
    std::wstring translation = L"";

    if (!(args >> word)) {
        std::wcerr << ERROR_INVALID_COMMAND;
        return;
    }

    if (!(args >> translation)) {
        std::wcerr << ERROR_INVALID_COMMAND;
        return;
    }

    if (!streamIsEmpty(args)) {
        std::wcerr << ERROR_INVALID_COMMAND;
        return;
    }

    toUpperCase(word);
    toUpperCase(translation);

    std::pair<ERDict::iterator, bool> insertion = dict.insert(
        std::pair<std::wstring, std::list<std::wstring>>{ word, { translation } });

    if (insertion.second == true) {
        std::wcout << L"- Inserted successfully\n\n";
        return;
    }

    std::list<std::wstring> &translations = insertion.first->second;

    std::list<std::wstring>::iterator pos = std::find_if(
        translations.begin(),
        translations.end(),
        [&translation](const std::wstring &s) {
            return translation <= s;
        }
    );

    if (pos != translations.end() && *pos == translation) {
        std::wcout << L"- This translation is already in the dictionary\n\n";
        return;
    }

    translations.insert(pos, translation);

    std::wcout << L"- Inserted successfully\n\n";
}

void handleRemoveTranslation(std::wistringstream &args, ERDict &dict) {

    std::wstring word = L"";
    std::wstring translation = L"";

    if (!(args >> word)) {
        std::wcerr << ERROR_INVALID_COMMAND;
        return;
    }

    if (!(args >> translation)) {
        std::wcerr << ERROR_INVALID_COMMAND;
        return;
    }

    if (!streamIsEmpty(args)) {
        std::wcerr << ERROR_INVALID_COMMAND;
        return;
    }

    toUpperCase(word);
    toUpperCase(translation);

    ERDict::iterator pair = dict.find(word);

    if (pair == dict.end()) {
        std::wcout << L"- This translation does not exist in the dictionary\n\n";
        return;
    }

    std::list<std::wstring> &values = pair->second;

    if (values.size() == 1) {
        std::wcout << L"- <NODE VALUES UNDERFLOW>\n\n";
        return;
    }

    std::list<std::wstring>::iterator i = std::find(
        values.begin(),
        values.end(),
        translation
    );

    if (i == values.end()) {
        std::wcout << L"- This translation does not exist in the dictionary\n\n";
        return;
    }

    values.erase(i);

    std::wcout << L"- The translation was removed successfully\n\n";
}

void handleRemoveWord(std::wistringstream &args, ERDict &dict) {

    std::wstring word = L"";

    if (!(args >> word)) {
        std::wcerr << ERROR_INVALID_COMMAND;
        return;
    }

    if (!streamIsEmpty(args)) {
        std::wcerr << ERROR_INVALID_COMMAND;
        return;
    }

    toUpperCase(word);

    if (dict.erase(word) == 1) {
        std::wcout << L"- The word was removed successfully\n\n";
    }
    else {
        std::wcout << L"- This word does not exist in the dictionary\n\n";
    }
}

void handleSearch(std::wistringstream &args, const ERDict &dict) {

    std::wstring word = L"";
    std::wstring translation = L"";

    if (!(args >> word)) {
        std::wcerr << ERROR_INVALID_COMMAND;
        return;
    }

    if (!(args >> translation)) {
        std::wcerr << ERROR_INVALID_COMMAND;
        return;
    }

    if (!streamIsEmpty(args)) {
        std::wcerr << ERROR_INVALID_COMMAND;
        return;
    }

    toUpperCase(word);
    toUpperCase(translation);

    ERDict::const_iterator pair = dict.find(word);

    if (pair == dict.cend()) {
        std::wcout << L"- This english word does not exist in the dictionary\n\n";
        return;
    }

    const std::list<std::wstring> &translations = pair->second;

    std::list<std::wstring>::const_iterator tr = std::find(
        translations.cbegin(),
        translations.cend(),
        translation
    );

    if (tr != translations.cend()) {
        std::wcout << L"- This translation exists in the dictionary\n\n";
    }
    else {
        std::wcout << L"- This translation does not exist in the dictionary\n\n";
    }
}

void handleGet(std::wistringstream &args, const ERDict &dict) {

    std::wstring word = L"";

    if (!(args >> word)) {
        std::wcerr << ERROR_INVALID_COMMAND;
        return;
    }

    if (!streamIsEmpty(args)) {
        std::wcerr << ERROR_INVALID_COMMAND;
        return;
    }

    toUpperCase(word);

    ERDict::const_iterator pair = dict.find(word);

    if (pair == dict.cend()) {
        std::wcout << L"- <INVALID KEY>\n\n";
        return;
    }

    const std::list<std::wstring> &translations = pair->second;

    std::wcout << L"- ";

    std::copy(
        translations.cbegin(),
        translations.cend(),
        std::ostream_iterator<std::wstring, wchar_t>(std::wcout, L" ")
    );

    std::wcout << L"\n\n";
}

void handleNumberOfNodes(const ERDict &dict) {
    std::wcout << L"- " << dict.size() << L"\n\n";
}

void handleOut(const ERDict &dict) {

    if (dict.empty()) {
        std::wcout << L"- #\n";
    }
    else {
        std::copy(
            dict.cbegin(),
            dict.cend(),
            DictOutputIterator(std::wcout)
        );
    }

    std::wcout << L"\n";
}

void handleLoad(std::wistringstream &args, ERDict &dict) {

    std::wstring filename = L"";

    if (!(args >> filename)) {
        std::wcerr << ERROR_INVALID_COMMAND;
        return;
    }

    if (!streamIsEmpty(args)) {
        std::wcerr << ERROR_INVALID_COMMAND;
        return;
    }

    std::wifstream ifs;
    ifs.imbue(std::locale(""));

    ifs.open(filename);

    if (!ifs.is_open()) {
        std::wcerr << ERROR_CANNOT_OPEN_FILE;
        return;
    }

    while (!ifs.eof()) {

        std::copy(
            std::istream_iterator<DictPair, wchar_t>(ifs),
            std::istream_iterator<DictPair, wchar_t>(),
            DictInserter(dict)
        );

        if (!ifs.eof() && ifs.fail()) {
            ifs.clear();
        }
    }

    ifs.close();

    std::wcout << L"- Loaded successfully\n\n";
}

bool streamIsEmpty(std::wistream &stream) {
    return (stream >> std::ws).peek() == std::char_traits<wchar_t>::eof();
}
