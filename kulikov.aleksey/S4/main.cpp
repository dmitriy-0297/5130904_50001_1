#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <stdexcept>

#include "Commands.hpp"

int main() {
    FrequencyDictionary dictionary;
    std::string line;

    while (true) {
        std::cout << "> ";

        try {
            if (!std::getline(std::cin, line)) {
                break;
            }

            if (line.empty()) {
                continue;
            }

            std::istringstream input(line);
            std::vector<std::string> args;
            std::string argument;

            while (input >> argument) {
                args.push_back(argument);
            }

            if (args.empty()) {
                continue;
            }

            CommandType command = parseCommand(args[0]);

            switch (command) {
                case CommandType::HELP:
                    if (args.size() != 1) {
                        throw std::runtime_error(ERR_INVALID_COMMAND);
                    }
                    printHelp();
                    break;

                case CommandType::INSERT: {
                    if (args.size() != 2) {
                        throw std::runtime_error(ERR_INVALID_COMMAND);
                    }

                    std::string word = cleanWord(args[1]);

                    if (word.empty()) {
                        throw std::runtime_error(ERR_INVALID_COMMAND);
                    }

                    insertWord(dictionary, word);
                    break;
                }

                case CommandType::REMOVE: {
                    if (args.size() != 2) {
                        throw std::runtime_error(ERR_INVALID_COMMAND);
                    }

                    std::string word = cleanWord(args[1]);

                    if (word.empty()) {
                        throw std::runtime_error(ERR_INVALID_COMMAND);
                    }

                    if (!removeWord(dictionary, word)) {
                        throw std::runtime_error(ERR_WORD_NOT_FOUND);
                    }

                    break;
                }

                case CommandType::SEARCH: {
                    if (args.size() != 2) {
                        throw std::runtime_error(ERR_INVALID_COMMAND);
                    }

                    std::string word = cleanWord(args[1]);

                    if (word.empty()) {
                        throw std::runtime_error(ERR_INVALID_COMMAND);
                    }

                    if (!searchWord(dictionary, word)) {
                        throw std::runtime_error(ERR_NOT_FOUND);
                    }

                    std::cout << "FOUND\n";
                    break;
                }

                case CommandType::TOP3:
                    if (args.size() != 1) {
                        throw std::runtime_error(ERR_INVALID_COMMAND);
                    }
                    printTop3(dictionary);
                    break;

                case CommandType::PRINT:
                    if (args.size() != 1) {
                        throw std::runtime_error(ERR_INVALID_COMMAND);
                    }
                    printAll(dictionary);
                    break;

                case CommandType::LOAD: {
                    if (args.size() > 2) {
                        throw std::runtime_error(ERR_INVALID_COMMAND);
                    }

                    std::string filename = DEFAULT_INPUT_FILE;

                    if (args.size() == 2) {
                        filename = args[1];
                    }

                    loadFromFile(dictionary, filename);
                    break;
                }

                case CommandType::CLEAR:
                    if (args.size() != 1) {
                        throw std::runtime_error(ERR_INVALID_COMMAND);
                    }
                    dictionary.clear();
                    break;

                case CommandType::TEST:
                    if (args.size() != 1) {
                        throw std::runtime_error(ERR_INVALID_COMMAND);
                    }
                    runTests();
                    break;

                case CommandType::INFO:
                    if (args.size() != 1) {
                        throw std::runtime_error(ERR_INVALID_COMMAND);
                    }
                    printInfo(dictionary);
                    break;

                case CommandType::INVALID:
                default:
                    throw std::runtime_error(ERR_INVALID_COMMAND);
            }
        } catch (const std::exception& error) {
            std::cerr << error.what() << "\n";
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
