#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <unordered_map>

using FrequencyDictionary = std::unordered_map<std::string, int>;

enum class CommandType {
    INSERT,
    REMOVE,
    SEARCH,
    TOP3,
    PRINT,
    LOAD,
    CLEAR,
    TEST,
    INFO,
    HELP,
    INVALID
};

extern const std::string DEFAULT_INPUT_FILE;

extern const std::string ERR_INVALID_COMMAND;
extern const std::string ERR_CANNOT_OPEN_FILE;
extern const std::string ERR_EMPTY_FILE;
extern const std::string ERR_WORD_NOT_FOUND;
extern const std::string ERR_NOT_FOUND;

std::string cleanWord(const std::string& word);
CommandType parseCommand(const std::string& command);

void insertWord(FrequencyDictionary& dictionary, const std::string& word);
bool searchWord(const FrequencyDictionary& dictionary, const std::string& word);
bool removeWord(FrequencyDictionary& dictionary, const std::string& word);

void printTop3(const FrequencyDictionary& dictionary);
void printAll(const FrequencyDictionary& dictionary);
void printInfo(const FrequencyDictionary& dictionary);
void loadFromFile(FrequencyDictionary& dictionary, const std::string& filename);
void runTests();
void printHelp();

#endif
