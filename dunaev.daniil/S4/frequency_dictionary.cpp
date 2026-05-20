#include "frequency_dictionary.hpp"

#include <algorithm>
#include <cassert>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <stdexcept>

namespace {

const std::string EMPTY_WORD_ERROR = "слово не должно быть пустым";
const std::string POSITIVE_COUNT_ERROR = "частота должна быть положительной";
const std::string INPUT_FILE_OPEN_ERROR = "не удалось открыть входной файл: ";
const std::string OUTPUT_FILE_OPEN_ERROR = "не удалось открыть выходной файл: ";
const std::string INPUT_FILE_CLOSE_ERROR = "не удалось закрыть входной файл: ";
const std::string OUTPUT_FILE_CLOSE_ERROR = "не удалось закрыть выходной файл: ";
const std::string TEST_SEPARATOR = "============================================================";
const std::string TEST_ERROR_PREFIX = "ОШИБКА: ";

WordRecord toWordRecord(const std::pair<const std::string, int>& item)
{
    return WordRecord{item.first, item.second};
}

bool decodeContinuationBytes(const std::string& text,
                             std::size_t position,
                             std::size_t remaining,
                             char32_t& value)
{
    if (remaining == 0) {
        return true;
    }

    unsigned char current = static_cast<unsigned char>(text[position]);
    if ((current & 0xC0) != 0x80) {
        return false;
    }

    value = (value << 6) | (current & 0x3F);
    return decodeContinuationBytes(text, position + 1, remaining - 1, value);
}

bool decodeNextCodePoint(const std::string& text, std::size_t& index, char32_t& codePoint)
{
    if (index >= text.size()) {
        return false;
    }

    unsigned char first = static_cast<unsigned char>(text[index]);
    if (first < 0x80) {
        codePoint = first;
        ++index;
        return true;
    }

    std::size_t length = 0;
    char32_t value = 0;
    if ((first & 0xE0) == 0xC0) {
        length = 2;
        value = first & 0x1F;
    } else if ((first & 0xF0) == 0xE0) {
        length = 3;
        value = first & 0x0F;
    } else if ((first & 0xF8) == 0xF0) {
        length = 4;
        value = first & 0x07;
    } else {
        ++index;
        return false;
    }

    if (index + length > text.size()) {
        index = text.size();
        return false;
    }

    if (!decodeContinuationBytes(text, index + 1, length - 1, value)) {
        ++index;
        return false;
    }

    index += length;
    codePoint = value;
    return true;
}

void appendCodePoint(std::string& text, char32_t codePoint)
{
    if (codePoint <= 0x7F) {
        text.push_back(static_cast<char>(codePoint));
    } else if (codePoint <= 0x7FF) {
        text.push_back(static_cast<char>(0xC0 | ((codePoint >> 6) & 0x1F)));
        text.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
    } else if (codePoint <= 0xFFFF) {
        text.push_back(static_cast<char>(0xE0 | ((codePoint >> 12) & 0x0F)));
        text.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
        text.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
    } else {
        text.push_back(static_cast<char>(0xF0 | ((codePoint >> 18) & 0x07)));
        text.push_back(static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F)));
        text.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
        text.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
    }
}

bool isWordCodePoint(char32_t codePoint)
{
    return (codePoint >= U'a' && codePoint <= U'z') ||
           (codePoint >= U'A' && codePoint <= U'Z') ||
           (codePoint >= U'0' && codePoint <= U'9') ||
           (codePoint >= U'А' && codePoint <= U'Я') ||
           (codePoint >= U'а' && codePoint <= U'я') ||
           codePoint == U'Ё' || codePoint == U'ё';
}

char32_t toLowerCodePoint(char32_t codePoint)
{
    if (codePoint >= U'A' && codePoint <= U'Z') {
        return codePoint - U'A' + U'a';
    }
    if (codePoint >= U'А' && codePoint <= U'Я') {
        return codePoint - U'А' + U'а';
    }
    if (codePoint == U'Ё') {
        return U'ё';
    }
    return codePoint;
}

void pushWordIfNotEmpty(const std::string& currentWord, std::vector<std::string>& words)
{
    if (!currentWord.empty()) {
        words.push_back(currentWord);
    }
}

void normalizeWordRecursive(const std::string& word,
                            std::size_t index,
                            std::string& result)
{
    if (index >= word.size()) {
        return;
    }

    char32_t codePoint = 0;
    std::size_t nextIndex = index;
    if (decodeNextCodePoint(word, nextIndex, codePoint) && isWordCodePoint(codePoint)) {
        appendCodePoint(result, toLowerCodePoint(codePoint));
    }

    normalizeWordRecursive(word, nextIndex, result);
}

void extractWordsRecursive(const std::string& text,
                           std::size_t index,
                           std::string currentWord,
                           std::vector<std::string>& words)
{
    if (index >= text.size()) {
        pushWordIfNotEmpty(currentWord, words);
        return;
    }

    char32_t codePoint = 0;
    std::size_t nextIndex = index;
    if (!decodeNextCodePoint(text, nextIndex, codePoint)) {
        pushWordIfNotEmpty(currentWord, words);
        extractWordsRecursive(text, nextIndex, std::string(), words);
        return;
    }

    if (isWordCodePoint(codePoint)) {
        appendCodePoint(currentWord, toLowerCodePoint(codePoint));
        extractWordsRecursive(text, nextIndex, currentWord, words);
        return;
    }

    pushWordIfNotEmpty(currentWord, words);
    extractWordsRecursive(text, nextIndex, std::string(), words);
}

std::string formatReportRecordLine(const WordRecord& record)
{
    std::ostringstream line;
    line << std::left << std::setw(24) << record.word << " " << record.count;
    return line.str();
}

std::vector<std::string> makeReportTableLines(const FrequencyDictionary& dictionary)
{
    std::vector<std::string> lines;
    std::vector<WordRecord> records = dictionary.toVector();
    lines.reserve(records.size());

    std::transform(records.cbegin(), records.cend(), std::back_inserter(lines), formatReportRecordLine);
    return lines;
}

std::vector<std::string> makeReportTopLines(const FrequencyDictionary& dictionary)
{
    std::vector<WordRecord> top = dictionary.topThree();
    std::vector<std::string> lines;
    lines.reserve(top.size());

    std::size_t position = 1;
    std::transform(top.cbegin(), top.cend(), std::back_inserter(lines),
                   [&position](const WordRecord& record) {
                       std::ostringstream line;
                       line << position << ") " << record.word << " — " << record.count;
                       ++position;
                       return line.str();
                   });
    return lines;
}

void writeLines(std::ostream& output, const std::vector<std::string>& lines)
{
    std::copy(lines.cbegin(), lines.cend(), std::ostream_iterator<std::string>(output, "\n"));
}

void addWordsRecursive(std::vector<std::string>::const_iterator current,
                       std::vector<std::string>::const_iterator last,
                       FrequencyDictionary& dictionary)
{
    if (current == last) {
        return;
    }

    dictionary.insert(*current);
    addWordsRecursive(std::next(current), last, dictionary);
}

void printTestTitle(int number, const std::string& title)
{
    std::cout << "Тест " << number << ": " << title << "\n";
}

void printCaughtException(const std::exception& exception)
{
    std::cerr << "  Поймано: >> " << TEST_ERROR_PREFIX << exception.what() << "\n";
}

void insertWordsRecursive(FrequencyDictionary& dictionary,
                          std::vector<std::string>::const_iterator current,
                          std::vector<std::string>::const_iterator last)
{
    if (current == last) {
        return;
    }

    dictionary.insert(*current);
    insertWordsRecursive(dictionary, std::next(current), last);
}

void insertWords(FrequencyDictionary& dictionary, const std::vector<std::string>& words)
{
    insertWordsRecursive(dictionary, words.cbegin(), words.cend());
}

void testInsertSearchAndDuplicateWords()
{
    FrequencyDictionary dictionary;
    dictionary.insert("data");
    dictionary.insert("tree");
    dictionary.insert("data", 2);

    int count = 0;
    assert(dictionary.search("data", count));
    assert(count == 3);
    assert(dictionary.search("tree", count));
    assert(count == 1);
    assert(dictionary.size() == 2);
    assert(dictionary.totalWords() == 4);
}

void testSearchMissingWord()
{
    FrequencyDictionary dictionary;
    dictionary.insert("tree");
    dictionary.insert("data");

    int count = 0;
    assert(!dictionary.search("algorithm", count));
    std::cout << "  Ожидаемый результат: слово 'algorithm' не найдено.\n";
}

void testRemoveExistingWord()
{
    FrequencyDictionary dictionary;
    insertWords(dictionary, {"m", "c", "t", "a", "e", "r", "z", "b", "d", "f"});

    assert(dictionary.remove("c"));
    assert(!dictionary.contains("c"));
    assert(dictionary.remove("m"));
    assert(!dictionary.contains("m"));
    assert(dictionary.size() == 8);
}

void testRemoveMissingWord()
{
    FrequencyDictionary dictionary;
    dictionary.insert("apple");

    assert(!dictionary.remove("orange"));
    std::cout << "  Ожидаемый результат: слово 'orange' отсутствует, удаление не выполнено.\n";
}

void testEmptyWordExceptions()
{
    FrequencyDictionary dictionary;

    try {
        dictionary.insert("");
        assert(false);
    } catch (const std::invalid_argument& exception) {
        printCaughtException(exception);
    }

    try {
        int count = 0;
        dictionary.search("", count);
        assert(false);
    } catch (const std::invalid_argument& exception) {
        printCaughtException(exception);
    }

    try {
        dictionary.remove("");
        assert(false);
    } catch (const std::invalid_argument& exception) {
        printCaughtException(exception);
    }
}

void testInvalidFrequencyExceptions()
{
    FrequencyDictionary dictionary;

    try {
        dictionary.insert("word", 0);
        assert(false);
    } catch (const std::invalid_argument& exception) {
        printCaughtException(exception);
    }

    try {
        dictionary.insert("word", -1);
        assert(false);
    } catch (const std::invalid_argument& exception) {
        printCaughtException(exception);
    }
}

void testAlphabeticalOutput()
{
    FrequencyDictionary dictionary;
    insertWords(dictionary, {"beta", "alpha", "gamma"});

    std::vector<WordRecord> records = dictionary.toVector();
    assert(records.size() == 3);
    assert(records[0].word == "alpha");
    assert(records[1].word == "beta");
    assert(records[2].word == "gamma");
}

void testTopThree()
{
    FrequencyDictionary dictionary;
    dictionary.insert("beta", 5);
    dictionary.insert("alpha", 5);
    dictionary.insert("gamma", 2);
    dictionary.insert("delta", 7);
    dictionary.insert("epsilon", 1);

    std::vector<WordRecord> top = dictionary.topThree();
    assert(top.size() == 3);
    assert(top[0].word == "delta" && top[0].count == 7);
    assert(top[1].word == "alpha" && top[1].count == 5);
    assert(top[2].word == "beta" && top[2].count == 5);
}

void testTextExtraction()
{
    std::vector<std::string> words = extractWords("Hello, HELLO! Мир мир. Ёж и ёж.");
    assert(words.size() == 7);
    assert(words[0] == "hello");
    assert(words[1] == "hello");
    assert(words[2] == "мир");
    assert(words[3] == "мир");
    assert(words[4] == "ёж");
    assert(words[5] == "и");
    assert(words[6] == "ёж");
}

void testDictionaryFromText()
{
    FrequencyDictionary dictionary = buildDictionaryFromText("Data data tree. Tree, graph!");

    int count = 0;
    assert(dictionary.search("data", count));
    assert(count == 2);
    assert(dictionary.search("tree", count));
    assert(count == 2);
    assert(dictionary.search("graph", count));
    assert(count == 1);
    assert(dictionary.size() == 3);
    assert(dictionary.totalWords() == 5);
}

void testInputFileOpenException()
{
    try {
        readTextFile("file_that_does_not_exist_123.txt");
        assert(false);
    } catch (const std::runtime_error& exception) {
        printCaughtException(exception);
    }
}

void testOutputFileOpenException()
{
    FrequencyDictionary dictionary;
    dictionary.insert("data");

    try {
        writeFrequencyReport(".", dictionary);
        assert(false);
    } catch (const std::runtime_error& exception) {
        printCaughtException(exception);
    }
}

void testClear()
{
    FrequencyDictionary dictionary;
    insertWords(dictionary, {"one", "two", "two"});

    dictionary.clear();
    assert(dictionary.empty());
    assert(dictionary.size() == 0);
    assert(dictionary.totalWords() == 0);
}

void testNormalizeWord()
{
    assert(normalizeWord("...ПРИВЕТ!!!") == "привет");
    assert(normalizeWord("Data-123") == "data123");
}

} 

void FrequencyDictionary::insert(const std::string& key, int countDelta)
{
    validateWord(key);
    validateCountDelta(countDelta);

    records_[key] += countDelta;
    totalWords_ += countDelta;
}

bool FrequencyDictionary::search(const std::string& key, int& count) const
{
    validateWord(key);

    auto iterator = records_.find(key);
    if (iterator == records_.end()) {
        return false;
    }

    count = iterator->second;
    return true;
}

bool FrequencyDictionary::contains(const std::string& key) const
{
    validateWord(key);
    return records_.find(key) != records_.end();
}

bool FrequencyDictionary::remove(const std::string& key)
{
    validateWord(key);

    auto iterator = records_.find(key);
    if (iterator == records_.end()) {
        return false;
    }

    totalWords_ -= iterator->second;
    records_.erase(iterator);
    return true;
}

void FrequencyDictionary::clear() noexcept
{
    records_.clear();
    totalWords_ = 0;
}

bool FrequencyDictionary::empty() const noexcept
{
    return records_.empty();
}

std::size_t FrequencyDictionary::size() const noexcept
{
    return records_.size();
}

int FrequencyDictionary::totalWords() const noexcept
{
    return totalWords_;
}

std::vector<WordRecord> FrequencyDictionary::toVector() const
{
    std::vector<WordRecord> result;
    result.reserve(records_.size());

    std::transform(records_.cbegin(), records_.cend(), std::back_inserter(result), toWordRecord);
    return result;
}

std::vector<WordRecord> FrequencyDictionary::topThree() const
{
    std::vector<WordRecord> records = toVector();
    std::vector<WordRecord> result(std::min<std::size_t>(3, records.size()));

    std::partial_sort_copy(records.cbegin(), records.cend(),
                           result.begin(), result.end(),
                           isBetterTopRecord);
    return result;
}

void FrequencyDictionary::validateWord(const std::string& key)
{
    if (key.empty()) {
        throw std::invalid_argument(EMPTY_WORD_ERROR);
    }
}

void FrequencyDictionary::validateCountDelta(int countDelta)
{
    if (countDelta <= 0) {
        throw std::invalid_argument(POSITIVE_COUNT_ERROR);
    }
}

bool FrequencyDictionary::isBetterTopRecord(const WordRecord& left,
                                            const WordRecord& right) noexcept
{
    if (left.count != right.count) {
        return left.count > right.count;
    }
    return left.word < right.word;
}

std::string readTextFile(const std::string& fileName)
{
    std::ifstream input(fileName, std::ios::binary);
    if (!input.is_open()) {
        throw std::runtime_error(INPUT_FILE_OPEN_ERROR + fileName);
    }

    std::ostringstream buffer;
    buffer << input.rdbuf();

    input.close();
    if (input.fail()) {
        throw std::runtime_error(INPUT_FILE_CLOSE_ERROR + fileName);
    }

    return buffer.str();
}

void writeFrequencyReport(const std::string& fileName,
                          const FrequencyDictionary& dictionary)
{
    std::ofstream output(fileName, std::ios::binary);
    if (!output.is_open()) {
        throw std::runtime_error(OUTPUT_FILE_OPEN_ERROR + fileName);
    }

    output << "ЧАСТОТНЫЙ СЛОВАРЬ НА STD::MAP\n";
    output << "=============================\n\n";
    output << "Всего слов: " << dictionary.totalWords() << "\n";
    output << "Уникальных слов: " << dictionary.size() << "\n";
    output << "Контейнер: std::map<std::string, int>\n\n";

    output << "Таблица частот:\n";
    std::vector<std::string> tableLines = makeReportTableLines(dictionary);
    if (tableLines.empty()) {
        output << "Словарь пуст.\n";
    } else {
        writeLines(output, tableLines);
    }

    output << "\nТри наиболее частых слова:\n";
    std::vector<std::string> topLines = makeReportTopLines(dictionary);
    if (topLines.empty()) {
        output << "Слова не найдены.\n";
    } else {
        writeLines(output, topLines);
    }

    output.close();
    if (output.fail()) {
        throw std::runtime_error(OUTPUT_FILE_CLOSE_ERROR + fileName);
    }
}

std::string normalizeWord(const std::string& word)
{
    std::string result;
    normalizeWordRecursive(word, 0, result);
    return result;
}

std::vector<std::string> extractWords(const std::string& text)
{
    std::vector<std::string> words;
    extractWordsRecursive(text, 0, std::string(), words);
    return words;
}

void addTextToDictionary(const std::string& text, FrequencyDictionary& dictionary)
{
    std::vector<std::string> words = extractWords(text);
    addWordsRecursive(words.cbegin(), words.cend(), dictionary);
}

FrequencyDictionary buildDictionaryFromText(const std::string& text)
{
    FrequencyDictionary dictionary;
    addTextToDictionary(text, dictionary);
    return dictionary;
}

bool runAllTests()
{
    std::cout << TEST_SEPARATOR << "\n";
    std::cout << "Тестирование STL-версии FrequencyDictionary\n";
    std::cout << TEST_SEPARATOR << "\n\n";

    printTestTitle(1, "Вставка, поиск и повторная вставка слова");
    testInsertSearchAndDuplicateWords();

    printTestTitle(2, "Поиск отсутствующего слова");
    testSearchMissingWord();

    printTestTitle(3, "Удаление существующего слова");
    testRemoveExistingWord();

    printTestTitle(4, "Удаление отсутствующего слова");
    testRemoveMissingWord();

    printTestTitle(5, "Обработка пустого слова в INSERT, SEARCH и REMOVE");
    testEmptyWordExceptions();

    printTestTitle(6, "Обработка некорректной частоты слова");
    testInvalidFrequencyExceptions();

    printTestTitle(7, "Сортировка слов в алфавитном порядке через std::map");
    testAlphabeticalOutput();

    printTestTitle(8, "Определение трех наиболее частых слов");
    testTopThree();

    printTestTitle(9, "Разбор текста на слова, обработка регистра и знаков препинания");
    testTextExtraction();

    printTestTitle(10, "Формирование словаря по тексту");
    testDictionaryFromText();

    printTestTitle(11, "Ошибка открытия входного файла");
    testInputFileOpenException();

    printTestTitle(12, "Ошибка открытия выходного файла");
    testOutputFileOpenException();

    printTestTitle(13, "Очистка словаря");
    testClear();

    printTestTitle(14, "Нормализация слова");
    testNormalizeWord();

    std::cout << "\n" << TEST_SEPARATOR << "\n";
    std::cout << "Все тесты успешно пройдены!\n";
    std::cout << TEST_SEPARATOR << "\n";

    return true;
}
