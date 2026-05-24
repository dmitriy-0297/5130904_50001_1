
// Yaroslavsky Oleg 5130904/50001 Laboratory work STL containers

#include "CrossReference.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>

const std::string ERROR_EMPTY_COMMAND = ">> ОШИБКА: Пустая команда!";
const std::string ERROR_UNKNOWN_COMMAND = ">> ОШИБКА: Неизвестная команда!";
const std::string ERROR_EMPTY_WORD = ">> ОШИБКА: Пустое слово!";
const std::string ERROR_WRONG_LINE = ">> ОШИБКА: Некорректный номер строки!";

void printPrompt() {
    std::cout << "\n";
}

void processCommand(CrossReference& cr, const std::string& commandLine) {
    if (commandLine.empty()) {
        throw std::invalid_argument(ERROR_EMPTY_COMMAND);
    }

    std::istringstream iss(commandLine);
    std::string cmd;
    iss >> cmd;

    if (cmd == "load") {
        std::string filename;
        iss >> filename;
        if (filename.empty()) {
            std::cout << "Использование: load <filename>\n";
            return;
        }
        cr.loadFromFile(filename);
    }

    else if (cmd == "show") {
        cr.show();
    }

    else if (cmd == "search") {
        std::string word;
        iss >> word;
        if (word.empty()) {
            std::cout << "Использование: search <word>\n";
            return;
        }
        cr.search(word);
    }

    else if (cmd == "add") {
        std::string word;
        int lineNum;
        iss >> word >> lineNum;
        if (word.empty()) {
            std::cout << "Использование: add <word> <line_number>\n";
            return;
        }
        if (lineNum <= 0) {
            throw std::out_of_range(ERROR_WRONG_LINE);
        }
        cr.add(word, lineNum);
    }

    else if (cmd == "remove") {
        std::string word;
        iss >> word;
        if (word.empty()) {
            std::cout << "Использование: remove <word>\n";
            return;
        }
        cr.remove(word);
    }

    else if (cmd == "save") {
        std::string filename;
        iss >> filename;
        if (filename.empty()) {
            std::cout << "Использование: save <filename>\n";
            return;
        }
        cr.saveToFile(filename);
    }

    else if (cmd == "stats") {
        cr.stats();
    }

    else if (cmd == "clear") {
        cr.clear();
    }

    else if (cmd == "help") {
        cr.help();
    }

    else if (cmd == "exit") {
        exit(0);
    }

    else {
        throw std::runtime_error(ERROR_UNKNOWN_COMMAND + " '" + cmd + "'");
    }

}


int main() {

    setlocale(LC_ALL, "Russian");

    std::cout << "Привет! [=^.^=]\n";

    CrossReference cr;
    std::string commandLine;

    cr.help();

    while (true) {
        printPrompt();

        if (!std::getline(std::cin, commandLine)) {
            break;
        }

        try {
            processCommand(cr, commandLine);
        }

        catch (const std::invalid_argument& err) {
            std::cerr << err.what() << "\n";
        }

        catch (const std::out_of_range& err) {
            std::cerr << err.what() << "\n";
        }

        catch (const std::runtime_error& err) {
            std::cerr << err.what() << "\n";
        }

        catch (const std::exception& err) {
            std::cerr << ">> ОШИБКА: " << err.what() << "\n";
        }

    }

    return EXIT_SUCCESS;
}

