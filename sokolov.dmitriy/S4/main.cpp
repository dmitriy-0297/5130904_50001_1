#include "commandHandlers.hpp"
#include "toUpperCase.hpp"
#include <clocale>
#include <list>
#include <iostream>
#include <locale>
#include <map>
#include <sstream>
#include <string>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

using ERDict = std::map<std::wstring, std::list<std::wstring>>;

const std::wstring ERROR_INVALID_COMMAND = L"- <INVALID COMMAND>\n\n";

int main() {

    setlocale(LC_ALL, "Russian");

#ifdef _WIN32
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
#else
    std::locale::global(std::locale(""));
    std::wcin.imbue(std::locale());
    std::wcout.imbue(std::locale());
#endif

    std::wcout << L"- Type \"HELP\" to get the list of commands, \"QUIT\" or EOF to quit.\n\n";

    std::wstring command = L"";
    ERDict dictionary;
    std::wstring cmd = L"";

    while (cmd != L"QUIT" && std::getline(std::wcin, command)) {

        std::wistringstream commandLine(command);

        if (!(commandLine >> cmd)) {
            continue;
        }

        toUpperCase(cmd);

        if (cmd == L"QUIT") {

            if (!streamIsEmpty(commandLine)) {
                std::wcerr << ERROR_INVALID_COMMAND;
                cmd = L"";
                continue;
            }

            continue;
        }
        else if (cmd == L"HELP") {

            if (!streamIsEmpty(commandLine)) {
                std::wcerr << ERROR_INVALID_COMMAND;
                continue;
            }

            handleHelp();
        }
        else if (cmd == L"INSERT") {
            handleInsert(commandLine, dictionary);
        }
        else if (cmd == L"RMTR") {
            handleRemoveTranslation(commandLine, dictionary);
        }
        else if (cmd == L"RMEW") {
            handleRemoveWord(commandLine, dictionary);
        }
        else if (cmd == L"SEARCH") {
            handleSearch(commandLine, dictionary);
        }
        else if (cmd == L"GET") {
            handleGet(commandLine, dictionary);
        }
        else if (cmd == L"NON") {

            if (!streamIsEmpty(commandLine)) {
                std::wcerr << ERROR_INVALID_COMMAND;
                continue;
            }

            handleNumberOfNodes(dictionary);
        }
        else if (cmd == L"OUT") {

            if (!streamIsEmpty(commandLine)) {
                std::wcerr << ERROR_INVALID_COMMAND;
                continue;
            }

            handleOut(dictionary);
        }
        else if (cmd == L"LOAD") {
            handleLoad(commandLine, dictionary);
        }
        else {
            std::wcerr << ERROR_INVALID_COMMAND;
            continue;
        }
    }

    return EXIT_SUCCESS;
}
