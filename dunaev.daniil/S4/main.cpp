#include "frequency_dictionary.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <exception>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {

const std::string TEST_ARGUMENT = "--test";
const std::string ERROR_PREFIX = "ОШИБКА: ";
const std::string PROMPT = "> ";
const std::string MAIN_SEPARATOR = "============================================================";
const std::string SMALL_SEPARATOR = "------------------------------------------------------------";
const std::string UNKNOWN_COMMAND_ERROR = "неизвестная команда. Введите help для списка команд";
const std::string COMMAND_NAME_ERROR = "команда не введена";
const std::string COUNT_FORMAT_ERROR = "частота должна быть целым положительным числом";
const std::string DEFAULT_INPUT_FILE_PATH = "input.txt";
const std::string DEFAULT_OUTPUT_FILE_PATH = "output.txt";

using CommandArguments = std::vector<std::string>;
using CommandHandler = std::function<void(FrequencyDictionary&, const CommandArguments&)>;

struct CommandDescription {
    std::string name;
    std::string syntax;
    std::string description;
};

std::ostream& operator<<(std::ostream& output, const CommandDescription& command)
{
    output << "  " << std::left << std::setw(24) << command.syntax
           << " — " << command.description;
    return output;
}

void configureConsoleEncoding();
void printGreeting();
void printHelp();
void printStats(const FrequencyDictionary& dictionary);
void printTable(const FrequencyDictionary& dictionary);
void printTopWords(const FrequencyDictionary& dictionary);
void printLines(const std::vector<std::string>& lines);

std::vector<CommandDescription> commandDescriptions();
std::map<std::string, CommandHandler> commandHandlers();
std::vector<std::string> splitCommandLine(const std::string& line);
std::string toLowerAscii(std::string value);
CommandArguments makeArguments(const std::vector<std::string>& tokens);
int parsePositiveInteger(const std::string& value);
void requireArgumentCount(const CommandArguments& arguments,
                          std::size_t minCount,
                          std::size_t maxCount,
                          const std::string& syntax);

bool processCommandLine(const std::string& line, FrequencyDictionary& dictionary);
void processCommands(FrequencyDictionary& dictionary);

void handleHelp(FrequencyDictionary& dictionary, const CommandArguments& arguments);
void handleLoad(FrequencyDictionary& dictionary, const CommandArguments& arguments);
void handleSave(FrequencyDictionary& dictionary, const CommandArguments& arguments);
void handleInsert(FrequencyDictionary& dictionary, const CommandArguments& arguments);
void handleSearch(FrequencyDictionary& dictionary, const CommandArguments& arguments);
void handleRemove(FrequencyDictionary& dictionary, const CommandArguments& arguments);
void handleTable(FrequencyDictionary& dictionary, const CommandArguments& arguments);
void handleTop(FrequencyDictionary& dictionary, const CommandArguments& arguments);
void handleStats(FrequencyDictionary& dictionary, const CommandArguments& arguments);
void handleExit(FrequencyDictionary& dictionary, const CommandArguments& arguments);

}

int main(int argc, char* argv[])
{
    configureConsoleEncoding();

    try {
        if (argc >= 2 && std::string(argv[1]) == TEST_ARGUMENT) {
            return runAllTests() ? EXIT_SUCCESS : EXIT_FAILURE;
        }

        FrequencyDictionary dictionary;
        printGreeting();
        printHelp();
        processCommands(dictionary);
        return EXIT_SUCCESS;
    } catch (const std::exception& exception) {
        std::cerr << ERROR_PREFIX << exception.what() << "\n";
        return EXIT_FAILURE;
    }
}

namespace {

void configureConsoleEncoding()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

void printGreeting()
{
    std::cout << MAIN_SEPARATOR << "\n"
              << "Задание 4. Частотный словарь.\n"
              << "Вариант курсовой: 1.3.3\n"
              << "Файл по умолчанию для чтения: " << DEFAULT_INPUT_FILE_PATH << "\n"
              << "Файл по умолчанию для записи: " << DEFAULT_OUTPUT_FILE_PATH << "\n"
              << "Конец ввода: Ctrl+D на Linux или Ctrl+Z, затем Enter на Windows.\n"
              << MAIN_SEPARATOR << "\n";
}

std::vector<CommandDescription> commandDescriptions()
{
    return {
        {"help", "help", "показать реализованный список команд"},
        {"load", "load [file]", "прочитать текстовый файл и заново построить словарь"},
        {"save", "save [file]", "записать отчет по текущему словарю в файл"},
        {"insert", "insert <word> [count]", "добавить слово или увеличить его частоту"},
        {"search", "search <word>", "найти слово и вывести его частоту"},
        {"remove", "remove <word>", "удалить слово из словаря"},
        {"table", "table", "вывести всю таблицу частот в алфавитном порядке"},
        {"top", "top", "вывести три наиболее частых слова"},
        {"stats", "stats", "показать общее число слов и число уникальных слов"},
        {"exit", "exit", "завершить программу без ожидания EOF"}
    };
}

void printHelp()
{
    std::cout << "\nРеализованный список команд:\n";
    std::vector<CommandDescription> commands = commandDescriptions();
    std::copy(commands.cbegin(), commands.cend(),
              std::ostream_iterator<CommandDescription>(std::cout, "\n"));
    std::cout << SMALL_SEPARATOR << "\n";
}

void printStats(const FrequencyDictionary& dictionary)
{
    std::cout << "Всего слов: " << dictionary.totalWords() << "\n"
              << "Уникальных слов: " << dictionary.size() << "\n"
              << "Контейнер: std::map<std::string, int>\n";
}

std::string makeRecordLine(const WordRecord& record)
{
    std::ostringstream line;
    line << std::left << std::setw(24) << record.word << " " << record.count;
    return line.str();
}

std::string makeTopLine(const WordRecord& record, std::size_t position)
{
    std::ostringstream line;
    line << position << ") " << record.word << " — " << record.count;
    return line.str();
}

std::vector<std::string> makeTableLines(const FrequencyDictionary& dictionary)
{
    std::vector<WordRecord> records = dictionary.toVector();
    std::vector<std::string> lines;
    lines.reserve(records.size());

    std::transform(records.cbegin(), records.cend(), std::back_inserter(lines), makeRecordLine);
    return lines;
}

std::vector<std::string> makeTopLines(const FrequencyDictionary& dictionary)
{
    std::vector<WordRecord> top = dictionary.topThree();
    std::vector<std::string> lines;
    lines.reserve(top.size());

    std::size_t position = 1;
    std::transform(top.cbegin(), top.cend(), std::back_inserter(lines),
                   [&position](const WordRecord& record) {
                       return makeTopLine(record, position++);
                   });
    return lines;
}

void printLines(const std::vector<std::string>& lines)
{
    std::copy(lines.cbegin(), lines.cend(), std::ostream_iterator<std::string>(std::cout, "\n"));
}

void printTable(const FrequencyDictionary& dictionary)
{
    std::vector<std::string> lines = makeTableLines(dictionary);
    if (lines.empty()) {
        std::cout << "Словарь пуст.\n";
        return;
    }

    std::cout << "Таблица частот:\n";
    printLines(lines);
}

void printTopWords(const FrequencyDictionary& dictionary)
{
    std::vector<std::string> lines = makeTopLines(dictionary);
    if (lines.empty()) {
        std::cout << "Слова не найдены.\n";
        return;
    }

    std::cout << "Три наиболее частых слова:\n";
    printLines(lines);
}

std::map<std::string, CommandHandler> commandHandlers()
{
    return {
        {"help", handleHelp},
        {"load", handleLoad},
        {"save", handleSave},
        {"insert", handleInsert},
        {"search", handleSearch},
        {"remove", handleRemove},
        {"table", handleTable},
        {"top", handleTop},
        {"stats", handleStats},
        {"exit", handleExit}
    };
}

std::vector<std::string> splitCommandLine(const std::string& line)
{
    std::istringstream input(line);
    return std::vector<std::string>(std::istream_iterator<std::string>(input),
                                    std::istream_iterator<std::string>());
}

std::string toLowerAscii(std::string value)
{
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char character) {
                       return static_cast<char>(std::tolower(character));
                   });
    return value;
}

CommandArguments makeArguments(const std::vector<std::string>& tokens)
{
    CommandArguments arguments;
    if (tokens.size() <= 1) {
        return arguments;
    }

    arguments.reserve(tokens.size() - 1);
    std::copy(std::next(tokens.cbegin()), tokens.cend(), std::back_inserter(arguments));
    return arguments;
}

int parsePositiveInteger(const std::string& value)
{
    if (value.empty() ||
        !std::all_of(value.cbegin(), value.cend(),
                     [](unsigned char character) { return std::isdigit(character) != 0; })) {
        throw std::invalid_argument(COUNT_FORMAT_ERROR);
    }

    int result = std::stoi(value);
    if (result <= 0) {
        throw std::invalid_argument(COUNT_FORMAT_ERROR);
    }
    return result;
}

void requireArgumentCount(const CommandArguments& arguments,
                          std::size_t minCount,
                          std::size_t maxCount,
                          const std::string& syntax)
{
    if (arguments.size() < minCount || arguments.size() > maxCount) {
        throw std::invalid_argument("формат команды: " + syntax);
    }
}

bool processCommandLine(const std::string& line, FrequencyDictionary& dictionary)
{
    std::vector<std::string> tokens = splitCommandLine(line);
    if (tokens.empty()) {
        return true;
    }

    std::string commandName = toLowerAscii(tokens.front());
    if (commandName.empty()) {
        throw std::invalid_argument(COMMAND_NAME_ERROR);
    }

    std::map<std::string, CommandHandler> handlers = commandHandlers();
    auto handler = handlers.find(commandName);
    if (handler == handlers.end()) {
        throw std::invalid_argument(UNKNOWN_COMMAND_ERROR);
    }

    CommandArguments arguments = makeArguments(tokens);
    handler->second(dictionary, arguments);
    return commandName != "exit";
}

void processCommands(FrequencyDictionary& dictionary)
{
    std::cout << PROMPT;

    std::string line;
    if (!std::getline(std::cin, line)) {
        std::cout << "\nДостигнут EOF. Работа программы завершена.\n";
        return;
    }

    try {
        if (!processCommandLine(line, dictionary)) {
            return;
        }
    } catch (const std::exception& exception) {
        std::cerr << ERROR_PREFIX << exception.what() << "\n";
        std::cerr << "Исключительная ситуация обработана, программа продолжает работу.\n";
    }

    processCommands(dictionary);
}

void handleHelp(FrequencyDictionary&, const CommandArguments& arguments)
{
    requireArgumentCount(arguments, 0, 0, "help");
    printHelp();
}

void handleLoad(FrequencyDictionary& dictionary, const CommandArguments& arguments)
{
    requireArgumentCount(arguments, 0, 1, "load [file]");

    const std::string FILE_PATH = arguments.empty() ? DEFAULT_INPUT_FILE_PATH : arguments[0];
    std::string text = readTextFile(FILE_PATH);
    dictionary = buildDictionaryFromText(text);

    std::cout << "Файл прочитан: " << FILE_PATH << "\n";
    std::cout << "Словарь построен.\n";
    printStats(dictionary);
}

void handleSave(FrequencyDictionary& dictionary, const CommandArguments& arguments)
{
    requireArgumentCount(arguments, 0, 1, "save [file]");

    const std::string FILE_PATH = arguments.empty() ? DEFAULT_OUTPUT_FILE_PATH : arguments[0];
    writeFrequencyReport(FILE_PATH, dictionary);
    std::cout << "Отчет записан в файл: " << FILE_PATH << "\n";
}

void handleInsert(FrequencyDictionary& dictionary, const CommandArguments& arguments)
{
    requireArgumentCount(arguments, 1, 2, "insert <word> [count]");

    std::string word = normalizeWord(arguments[0]);
    int count = arguments.size() == 2 ? parsePositiveInteger(arguments[1]) : 1;
    dictionary.insert(word, count);

    std::cout << "Слово '" << word << "' добавлено. Частота увеличена на " << count << ".\n";
}

void handleSearch(FrequencyDictionary& dictionary, const CommandArguments& arguments)
{
    requireArgumentCount(arguments, 1, 1, "search <word>");

    std::string word = normalizeWord(arguments[0]);
    int count = 0;
    if (dictionary.search(word, count)) {
        std::cout << "Слово '" << word << "' найдено. Частота: " << count << "\n";
    } else {
        std::cout << "Слово '" << word << "' не найдено.\n";
    }
}

void handleRemove(FrequencyDictionary& dictionary, const CommandArguments& arguments)
{
    requireArgumentCount(arguments, 1, 1, "remove <word>");

    std::string word = normalizeWord(arguments[0]);
    if (dictionary.remove(word)) {
        std::cout << "Слово '" << word << "' удалено.\n";
    } else {
        std::cout << "Слово '" << word << "' отсутствует в словаре.\n";
    }
}

void handleTable(FrequencyDictionary& dictionary, const CommandArguments& arguments)
{
    requireArgumentCount(arguments, 0, 0, "table");
    printTable(dictionary);
}

void handleTop(FrequencyDictionary& dictionary, const CommandArguments& arguments)
{
    requireArgumentCount(arguments, 0, 0, "top");
    printTopWords(dictionary);
}

void handleStats(FrequencyDictionary& dictionary, const CommandArguments& arguments)
{
    requireArgumentCount(arguments, 0, 0, "stats");
    printStats(dictionary);
}

void handleExit(FrequencyDictionary&, const CommandArguments& arguments)
{
    requireArgumentCount(arguments, 0, 0, "exit");
    std::cout << "Работа программы завершена.\n";
}

}
