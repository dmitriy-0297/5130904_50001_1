#include "cross_references.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cstdlib>

void showHelp() {
    std::cout << "\nCommands:\n"
              << "  add <text>          - add line of text\n"
              << "  print               - show cross-references\n"
              << "  find <word>         - show lines for word\n"
              << "  save <filename>     - save to file\n"
              << "  clear               - clear all data\n"
              << "  test                - run built-in tests\n"
              << "  help                - this help\n"
              << "  exit                - exit program\n"
              << "EOF (Ctrl+Z/Ctrl+D)   - also exit\n\n";
}

void processCommand(CrossReferences& cr, const std::string& line) {
    if (line.empty()) return;
    
    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;
    
    if (cmd == "exit") {
        throw std::runtime_error("exit");
    }
    else if (cmd == "help") {
        showHelp();
    }
    else if (cmd == "test") {
        cr.runTests();
    }
    else if (cmd == "add") {
        std::string text;
        std::getline(iss, text);
        if (!text.empty() && text[0] == ' ') text.erase(0, 1);
        if (!text.empty()) {
            try {
                cr.addText(text);
                std::cout << "Line added successfully.\n";
            } catch (const std::invalid_argument& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        } else {
            std::cout << "Error: empty text.\n";
        }
    }
    else if (cmd == "print") {
        cr.printReferences();
    }
    else if (cmd == "find") {
        std::string word;
        iss >> word;
        if (word.empty()) {
            std::cout << "Usage: find <word>\n";
        } else {
            try {
                auto linesSet = cr.getLinesForWord(word);
                if (linesSet.empty()) {
                    std::cout << "Word '" << word << "' not found.\n";
                } else {
                    std::cout << word << ": ";
                    bool first = true;
                    for (int lineNum : linesSet) {
                        if (!first) std::cout << ", ";
                        std::cout << lineNum;
                        first = false;
                    }
                    std::cout << "\n";
                }
            } catch (const std::invalid_argument& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        }
    }
    else if (cmd == "save") {
        std::string filename;
        iss >> filename;
        if (filename.empty()) {
            std::cout << "Usage: save <filename>\n";
        } else {
            try {
                cr.saveToFile(filename);
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        }
    }
    else if (cmd == "clear") {
        cr.clear();
        std::cout << "All data cleared.\n";
    }
    else {
        std::cout << "Unknown command. Type 'help'.\n";
    }
}

int main() {
    CrossReferences cr;
    std::string line;
    
    std::cout << "Cross-References Laboratory Work\n";
    std::cout << "Using: std::map (red-black tree)\n";
    std::cout << "       std::unordered_map (hash table)\n\n";
    showHelp();
    
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) {
            std::cout << "\nEOF received. Exiting.\n";
            break;
        }
        
        try {
            processCommand(cr, line);
        } catch (const std::runtime_error& e) {
            if (std::string(e.what()) == "exit") {
                break;
            }
            std::cout << "Error: " << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cout << "Unexpected error: " << e.what() << "\n";
        }
    }
    
    return EXIT_SUCCESS;
}
