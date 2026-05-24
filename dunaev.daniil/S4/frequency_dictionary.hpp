#ifndef FREQUENCY_DICTIONARY_HPP
#define FREQUENCY_DICTIONARY_HPP

#include <cstddef>
#include <map>
#include <string>
#include <vector>

struct WordRecord {
    std::string word;
    int count;
};

class FrequencyDictionary {
public:
    void insert(const std::string& key, int countDelta = 1);
    bool search(const std::string& key, int& count) const;
    bool contains(const std::string& key) const;
    bool remove(const std::string& key);
    void clear() noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;
    int totalWords() const noexcept;

    std::vector<WordRecord> toVector() const;
    std::vector<WordRecord> topThree() const;

private:
    std::map<std::string, int> records_;
    int totalWords_ = 0;

    static void validateWord(const std::string& key);
    static void validateCountDelta(int countDelta);
    static bool isBetterTopRecord(const WordRecord& left,
                                  const WordRecord& right) noexcept;
};

std::string readTextFile(const std::string& fileName);
void writeFrequencyReport(const std::string& fileName,
                          const FrequencyDictionary& dictionary);

std::string normalizeWord(const std::string& word);
std::vector<std::string> extractWords(const std::string& text);
void addTextToDictionary(const std::string& text, FrequencyDictionary& dictionary);
FrequencyDictionary buildDictionaryFromText(const std::string& text);

bool runAllTests();

#endif
