#include "text_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>

std::string normalizeWord(const std::string& raw) {
    std::string result;
    result.reserve(raw.length());

    std::transform(raw.begin(), raw.end(), std::back_inserter(result),
        [](unsigned char ch) -> char {
            if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
                return static_cast<char>(std::tolower(ch));
            }
            else if ((ch >= 'А' && ch <= 'Я') || (ch >= 'а' && ch <= 'я')) {
                return static_cast<char>(std::tolower(ch));
            }
            else if (ch == 'Ё') return 'ё';
            else if (ch == 'ё') return 'ё';
            return '\0';
        });

    result.erase(std::remove(result.begin(), result.end(), '\0'), result.end());
    return result;
}

std::vector<std::string> tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream stream(line);
    std::copy(std::istream_iterator<std::string>(stream),
        std::istream_iterator<std::string>(),
        std::back_inserter(tokens));
    return tokens;
}

void showHelp() {
    std::cout << "\n=== AVAILABLE COMMANDS ===\n";
    std::cout << "INSERT <word>  - Add word with current line number\n";
    std::cout << "SEARCH <word>  - Search for word\n";
    std::cout << "REMOVE <word>  - Remove word\n";
    std::cout << "PRINT          - Show all cross-references\n";
    std::cout << "CLEAR          - Clear all data\n";
    std::cout << "COUNT          - Show number of unique words\n";
    std::cout << "LINES <word>   - Show line numbers for word\n";
    std::cout << "HELP           - Show this help\n";
    std::cout << "EXIT           - Exit program\n";
    std::cout << "========================\n\n";
}

void processCommand(const std::string& line, DataManager& manager, bool& running) {
    auto tokens = tokenize(line);

    if (tokens.empty()) {
        return;
    }

    std::string cmd = tokens[0];
    std::transform(cmd.begin(), cmd.end(), cmd.begin(),
        [](unsigned char c) { return static_cast<char>(std::toupper(c)); });

    try {
        if (cmd == "EXIT" || cmd == "QUIT") {
            running = false;
        }
        else if (cmd == "HELP") {
            showHelp();
        }
        else if (cmd == "INSERT" || cmd == "ADD") {
            if (tokens.size() < 2) {
                std::cerr << "ERROR: Invalid command. Usage: INSERT <word>\n";
                return;
            }
            manager.incrementLine();
            std::string word = normalizeWord(tokens[1]);
            if (manager.insert(word)) {
                std::cout << "[OK] Word inserted at line " << manager.getCurrentLine() << "\n";
            }
        }
        else if (cmd == "SEARCH" || cmd == "FIND") {
            if (tokens.size() < 2) {
                std::cerr << "ERROR: Invalid command. Usage: SEARCH <word>\n";
                return;
            }
            std::string word = normalizeWord(tokens[1]);
            if (manager.search(word)) {
                std::cout << "[OK] Word '" << word << "' found\n";
            }
            else {
                std::cout << "[INFO] Word '" << word << "' not found\n";
            }
        }
        else if (cmd == "REMOVE" || cmd == "DELETE") {
            if (tokens.size() < 2) {
                std::cerr << "ERROR: Invalid command. Usage: REMOVE <word>\n";
                return;
            }
            std::string word = normalizeWord(tokens[1]);
            if (manager.remove(word)) {
                std::cout << "[OK] Word '" << word << "' removed\n";
            }
            else {
                std::cerr << "ERROR: Word not found.\n";
            }
        }
        else if (cmd == "PRINT" || cmd == "SHOW") {
            manager.print(std::cout);
        }
        else if (cmd == "CLEAR") {
            manager.clear();
            std::cout << "[OK] All data cleared\n";
        }
        else if (cmd == "COUNT") {
            std::cout << "[INFO] Unique words: " << manager.count() << "\n";
        }
        else if (cmd == "LINES") {
            if (tokens.size() < 2) {
                std::cerr << "ERROR: Invalid command. Usage: LINES <word>\n";
                return;
            }
            std::string word = normalizeWord(tokens[1]);
            if (!manager.showLines(word, std::cout)) {
                std::cerr << "ERROR: Word not found.\n";
            }
        }
        else {
            std::cerr << "ERROR: Invalid command. Type HELP for available commands.\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\n";
    }
}
