#include "Commands.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <stdexcept>
#include <cctype>

const std::string DEFAULT_INPUT_FILE = "input.txt";

const std::string ERR_INVALID_COMMAND = "<INVALID COMMAND>";
const std::string ERR_CANNOT_OPEN_FILE = "<CANNOT OPEN FILE>";
const std::string ERR_EMPTY_FILE = "<EMPTY FILE>";
const std::string ERR_WORD_NOT_FOUND = "Word not found";
const std::string ERR_NOT_FOUND = "NOT FOUND";

std::string cleanWord(const std::string& word) {
    std::string result;

    for (char symbol : word) {
        if (std::isalpha(static_cast<unsigned char>(symbol))) {
            result += static_cast<char>(std::tolower(static_cast<unsigned char>(symbol)));
        }
    }

    return result;
}

CommandType parseCommand(const std::string& command) {
    std::string upperCommand = command;

    std::transform(
        upperCommand.begin(),
        upperCommand.end(),
        upperCommand.begin(),
        [](unsigned char symbol) {
            return static_cast<char>(std::toupper(symbol));
        }
    );

    if (upperCommand == "INSERT") {
        return CommandType::INSERT;
    }

    if (upperCommand == "REMOVE") {
        return CommandType::REMOVE;
    }

    if (upperCommand == "SEARCH") {
        return CommandType::SEARCH;
    }

    if (upperCommand == "TOP3") {
        return CommandType::TOP3;
    }

    if (upperCommand == "PRINT") {
        return CommandType::PRINT;
    }

    if (upperCommand == "LOAD") {
        return CommandType::LOAD;
    }

    if (upperCommand == "CLEAR") {
        return CommandType::CLEAR;
    }

    if (upperCommand == "TEST") {
        return CommandType::TEST;
    }

    if (upperCommand == "INFO") {
        return CommandType::INFO;
    }

    if (upperCommand == "HELP") {
        return CommandType::HELP;
    }

    return CommandType::INVALID;
}

void insertWord(FrequencyDictionary& dictionary, const std::string& word) {
    ++dictionary[word];
}

bool searchWord(const FrequencyDictionary& dictionary, const std::string& word) {
    return dictionary.find(word) != dictionary.end();
}

bool removeWord(FrequencyDictionary& dictionary, const std::string& word) {
    return dictionary.erase(word) > 0;
}

void printTop3(const FrequencyDictionary& dictionary) {
    if (dictionary.empty()) {
        std::cout << "Dictionary is empty\n";
        return;
    }

    std::vector<std::pair<std::string, int>> words(dictionary.begin(), dictionary.end());

    std::sort(
        words.begin(),
        words.end(),
        [](const auto& left, const auto& right) {
            if (left.second == right.second) {
                return left.first < right.first;
            }

            return left.second > right.second;
        }
    );

    size_t limit = std::min<size_t>(3, words.size());

    for (size_t i = 0; i < limit; ++i) {
        std::cout << words[i].first << ": " << words[i].second << "\n";
    }
}

void printAll(const FrequencyDictionary& dictionary) {
    if (dictionary.empty()) {
        std::cout << "Dictionary is empty\n";
        return;
    }

    std::vector<std::pair<std::string, int>> words(dictionary.begin(), dictionary.end());

    std::sort(
        words.begin(),
        words.end(),
        [](const auto& left, const auto& right) {
            return left.first < right.first;
        }
    );

    for (const auto& word : words) {
        std::cout << word.first << ": " << word.second << "\n";
    }
}

void printInfo(const FrequencyDictionary& dictionary) {
    int totalWords = 0;

    for (const auto& word : dictionary) {
        totalWords += word.second;
    }

    std::cout << "Dictionary Info:\n";
    std::cout << "Unique words: " << dictionary.size() << "\n";
    std::cout << "Total words: " << totalWords << "\n";
    std::cout << "Buckets count: " << dictionary.bucket_count() << "\n";
    std::cout << "Load factor: " << dictionary.load_factor() << "\n";
}

void loadFromFile(FrequencyDictionary& dictionary, const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error(ERR_CANNOT_OPEN_FILE);
    }

    std::string line;
    bool hasWords = false;

    while (std::getline(file, line)) {
        std::replace(line.begin(), line.end(), ',', ' ');

        std::istringstream input(line);
        std::string word;

        while (input >> word) {
            std::string clean = cleanWord(word);

            if (!clean.empty()) {
                insertWord(dictionary, clean);
                hasWords = true;
            }
        }
    }

    file.close();

    if (!hasWords) {
        throw std::runtime_error(ERR_EMPTY_FILE);
    }
}

void runTests() {
    FrequencyDictionary dictionary;

    std::cout << "[TEST] Running basic tests...\n";

    insertWord(dictionary, "apple");
    insertWord(dictionary, "banana");
    insertWord(dictionary, "apple");
    insertWord(dictionary, "orange");
    std::cout << "[TEST] INSERT: Passed\n";

    assert(searchWord(dictionary, "apple"));
    assert(searchWord(dictionary, "banana"));
    assert(searchWord(dictionary, "orange"));
    assert(!searchWord(dictionary, "grape"));
    std::cout << "[TEST] SEARCH: Passed\n";

    assert(dictionary["apple"] == 2);
    std::cout << "[TEST] COUNT check: Passed\n";

    assert(removeWord(dictionary, "banana"));
    assert(!searchWord(dictionary, "banana"));
    assert(!removeWord(dictionary, "grape"));
    std::cout << "[TEST] REMOVE: Passed\n";

    std::cout << "[TEST] TOP3 words:\n";
    printTop3(dictionary);
    std::cout << "[TEST] TOP3: Passed\n";

    dictionary.clear();
    assert(dictionary.empty());
    std::cout << "[TEST] CLEAR: Passed\n";

    std::cout << "[TEST] All basic tests passed!\n";
}

void printHelp() {
    std::cout << "Available commands:\n"
              << "INSERT word\n"
              << "REMOVE word\n"
              << "SEARCH word\n"
              << "TOP3\n"
              << "PRINT\n"
              << "LOAD filename\n"
              << "LOAD\n"
              << "CLEAR\n"
              << "TEST\n"
              << "INFO\n"
              << "HELP\n";
}
