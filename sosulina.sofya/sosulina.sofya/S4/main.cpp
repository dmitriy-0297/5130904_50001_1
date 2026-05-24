#include <iostream>
#include <string>
#include <cstdlib>
#include <exception>
#include "data_manager.hpp"
#include "text_utils.hpp"

const std::string ERROR_PREFIX = "ERROR: ";
const std::string ERR_EMPTY_WORD = ERROR_PREFIX + "No valid letters in the word.";
const std::string ERR_NOT_FOUND = ERROR_PREFIX + "Word not found.";
const std::string ERR_INVALID_CMD = ERROR_PREFIX + "Invalid command.";
const std::string ERR_GENERAL = ERROR_PREFIX + "An unexpected error occurred.";

const std::string MSG_READY = "[INFO] Ready for input. Type HELP for commands or Ctrl+D (Ctrl+Z on Windows) to exit.\n";
const std::string MSG_EXIT = "[INFO] Program terminated successfully.";

void interactiveMode(DataManager& manager);


int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--help" || arg == "-h") {
            std::cout << "Cross-References Program (STL version)\n";
            std::cout << "Based on AVL Tree coursework\n\n";
            std::cout << "Usage: " << argv[0] << " [--help]\n";
            std::cout << "  Program creates cross-references for text using std::map\n";
            std::cout << "  Interactive mode: type commands or Ctrl+D (Ctrl+Z) to exit\n";
            return EXIT_SUCCESS;
        }
    }

    try {
        DataManager manager;
        std::cout << MSG_READY;
        showHelp();

        interactiveMode(manager);

        std::cout << MSG_EXIT << "\n";
        return EXIT_SUCCESS;
    }
    catch (const std::exception& e) {
        std::cerr << ERR_GENERAL << " " << e.what() << "\n";
        return EXIT_FAILURE;
    }
}

void interactiveMode(DataManager& manager) {
    std::string line;
    bool running = true;

    std::cout << "> ";
    while (running && std::getline(std::cin, line)) {
        processCommand(line, manager, running);
        if (running) {
            std::cout << "> ";
        }
    }
}
