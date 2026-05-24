
#include "CrossReference.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <set>

const std::string ERROR_OPEN_FILE = ">> ОШИБКА: Не удалось найти или открыть файл!";
const std::string ERROR_EMPTY_FILE = ">> ОШИБКА: Файл пустой!";
const std::string ERROR_WORD_NOT_FOUND = ">> ОШИБКА: Слово не найдено!";
const std::string ERROR_EMPTY_WORD = ">> ОШИБКА: Пустое слово!";
const std::string ERROR_WRONG_LINE = ">> ОШИБКА: Номер строки должен быть положительным!";

CrossReference::CrossReference() {}

CrossReference::~CrossReference() {}

bool CrossReference::isWordChar(char c) const {
    return std::isalpha(static_cast<unsigned char>(c)) ||
        std::isdigit(static_cast<unsigned char>(c));
}

std::string CrossReference::cleanWord(const std::string& word) const {
    std::string result;

    for (char c : word) {
        if (isWordChar(c)) {
            result += std::tolower(static_cast<unsigned char>(c));
        }
    }

    return result;
}

void CrossReference::parseAndInsert(const std::string& line, int lineNumber) {
    std::string word;
    std::istringstream lineStream(line);

    while (lineStream >> word) {
        std::string cleaned = cleanWord(word);
        if (!cleaned.empty()) {
            index[cleaned].insert(lineNumber);
        }
    }
}

void CrossReference::loadFromFile(const std::string& filename) {

    std::ifstream fileIn(filename);

    if (!fileIn.is_open()) {
        throw std::runtime_error(ERROR_OPEN_FILE);
    }

    lines.clear();
    index.clear();
    file = filename;

    std::string line;
    int lineNumber = 0;

    while (std::getline(fileIn, line)) {
        ++lineNumber;
        lines.push_back(line);
        parseAndInsert(line, lineNumber);
    }

    fileIn.close();

    if (lineNumber == 0) {
        throw std::runtime_error(ERROR_EMPTY_FILE);
    }

    std::cout << "Загружено " << lineNumber << " строк, найдено "
        << index.size() << " уникальных слов\n";

}

void CrossReference::show() const {
    if (lines.empty()) {
        std::cout << "Нет загруженных данных. Используйте load <filename>\n";
        return;
    }

    std::cout << "\n=== Пронумерованный текст ===\n";
    for (size_t i = 0; i < lines.size(); ++i) {
        std::cout << (i + 1) << ": " << lines[i] << "\n";
    }

    std::cout << "\n=== Таблица перекрестных ссылок ===\n";

    if (index.empty()) {
        std::cout << "(нет слов)\n";
        return;
    }

    for (const auto& entry : index) {
        std::cout << entry.first << ": ";
        for (int lineNumber : entry.second) {
            std::cout << lineNumber << " ";
        }
        std::cout << "\n";
    }
}

void CrossReference::search(const std::string& word) const {
    if (word.empty()) {
        throw std::invalid_argument(ERROR_EMPTY_WORD);
    }

    std::string cleaned = cleanWord(word);

    auto it = index.find(cleaned);
    if (it == index.end()) {
        std::cout << "Слово \"" << word << "\" не найдено\n";
        return;
    }

    std::cout << "Слово \"" << word << "\" встречается в строках: ";
    for (int lineNumber : it->second) {
        std::cout << lineNumber << " ";
    }
    std::cout << "\n";
}

void CrossReference::add(const std::string& word, int lineNumber) {
    if (word.empty()) {
        throw std::invalid_argument(ERROR_EMPTY_WORD);
    }

    if (lineNumber <= 0) {
        throw std::out_of_range(ERROR_WRONG_LINE);
    }

    std::string cleaned = cleanWord(word);
    index[cleaned].insert(lineNumber);

    std::cout << "Слово \"" << word << "\" добавлено в строку " << lineNumber << "\n";
}

void CrossReference::remove(const std::string& word) {
    if (word.empty()) {
        throw std::invalid_argument(ERROR_EMPTY_WORD);
    }

    std::string cleaned = cleanWord(word);

    auto it = index.find(cleaned);
    if (it == index.end()) {
        throw std::runtime_error(ERROR_WORD_NOT_FOUND + " '" + word + "'");
    }

    index.erase(it);
    std::cout << "Слово \"" << word << "\" удалено из словаря\n";
}

void CrossReference::saveToFile(const std::string& filename) const {
    std::ofstream fileOut(filename);

    if (!fileOut.is_open()) {
        throw std::runtime_error(ERROR_OPEN_FILE);
    }

    for (size_t i = 0; i < lines.size(); ++i) {
        fileOut << (i + 1) << ": " << lines[i] << "\n";
    }

    fileOut << "\nТаблица перекрестных ссылок\n";

    for (const auto& entry : index) {
        fileOut << entry.first << ": ";
        for (int lineNumber : entry.second) {
            fileOut << lineNumber << " ";
        }
        fileOut << "\n";
    }

    fileOut.close();
    std::cout << "Результат сохранен в " << filename << "\n";
}

void CrossReference::stats() const {
    std::cout << "\n=== Статистика ===\n";
    std::cout << "Текущий файл: " << (file.empty() ? "(не загружен)" : file) << "\n";
    std::cout << "Всего строк: " << lines.size() << "\n";
    std::cout << "Уникальных слов: " << index.size() << "\n";

    size_t totalOccurrences = 0;
    for (const auto& entry : index) {
        totalOccurrences += entry.second.size();
    }
    std::cout << "Всего вхождений: " << totalOccurrences << "\n";

    if (!index.empty()) {
        std::cout << "Слова в словаре:\n  ";
        size_t count = 0;
        for (const auto& entry : index) {
            std::cout << entry.first;
            ++count;
            if (count < index.size() && count % 10 != 0) {
                std::cout << ", ";
            }
            else if (count % 10 == 0) {
                std::cout << "\n  ";
            }
        }
        std::cout << "\n";
    }
}

void CrossReference::clear() {
    index.clear();
    lines.clear();
    file.clear();
    std::cout << "Все данные очищены!\n";
}

void CrossReference::help() const {
    std::cout << "\n------------- Список команд -------------\n";
    std::cout << "  load - загрузить текст из файла\n";
    std::cout << "  show - показать текст и таблицу ссылок\n";
    std::cout << "  search - найти слово в словаре\n";
    std::cout << "  add - добавить слово в строку\n";
    std::cout << "  remove - удалить слово из словаря\n";
    std::cout << "  save - сохранить результат в файл\n";
    std::cout << "  stats - показать статистику\n";
    std::cout << "  clear - очистить данные\n";
    std::cout << "  help - показать справку\n";
    std::cout << "  exit - выход";
    std::cout << "\n-----------------------------------------\n";
}






