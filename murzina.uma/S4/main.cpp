#include "cross_references.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cstdlib>

void showHelp() {
    std::cout << "\nCommands:\n"
              << "  1 - add <text>          - add line of text\n"
              << "  2 - print               - show cross-references\n"
              << "  3 - find <word>         - show lines for word\n"
              << "  4 - save <filename>     - save to file\n"
              << "  5 - clear               - clear all data\n"
              << "  6 - test                - run built-in tests\n"
              << "  0 - help                - this help\n"
              << "  9 - exit                - exit program\n"
              << "EOF (Ctrl+Z/Ctrl+D)       - also exit\n\n";
}

int getCommandCode(const std::string& cmd) {
    if (cmd == "add" || cmd == "1") return 1;
    if (cmd == "print" || cmd == "2") return 2;
    if (cmd == "find" || cmd == "3") return 3;
    if (cmd == "save" || cmd == "4") return 4;
    if (cmd == "clear" || cmd == "5") return 5;
    if (cmd == "test" || cmd == "6") return 6;
    if (cmd == "help" || cmd == "0") return 0;
    if (cmd == "exit" || cmd == "9") return 9;
    return -1;
}

void processCommand(CrossReferences& cr, const std::string& line) {
    if (line.empty()) return;
    
    std::istringstream iss(line);
    std::string cmdStr;
    iss >> cmdStr;
    
    int cmd = getCommandCode(cmdStr);
    
    switch (cmd) {
        case 9:
            throw std::runtime_error("exit");
            
        case 0:
            showHelp();
            break;
            
        case 6:
            cr.runTests();
            break;
            
        case 5:
            cr.clear();
            std::cout << "All data cleared.\n";
            break;
            
        case 4: {
            std::string filename;
            iss >> filename;
            if (filename.empty()) {
                std::cout << "Usage: save <filename> or 4 <filename>\n";
            } else {
                try {
                    cr.saveToFile(filename);
                } catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
            }
            break;
        }
            
        case 3: {
            std::string word;
            iss >> word;
            if (word.empty()) {
                std::cout << "Usage: find <word> or 3 <word>\n";
            } else {
                try {
                    std::set<int> linesSet = cr.getLinesForWord(word);
                    if (linesSet.empty()) {
                        std::cout << "Word '" << word << "' not found.\n";
                    } else {
                        std::cout << word << ": ";
                        bool first = true;
                        for (std::set<int>::iterator it = linesSet.begin(); it != linesSet.end(); ++it) {
                            if (!first) std::cout << ", ";
                            std::cout << *it;
                            first = false;
                        }
                        std::cout << "\n";
                    }
                } catch (const std::invalid_argument& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
            }
            break;
        }
            
        case 2:
            cr.printReferences();
            break;
            
        case 1: {
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
            break;
        }
            
        default:
            std::cout << "Unknown command. Type 'help' or '0'.\n";
            break;
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
