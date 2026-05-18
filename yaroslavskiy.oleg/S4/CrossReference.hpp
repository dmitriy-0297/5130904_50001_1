
#ifndef CROSS_REFERENCE_HPP
#define CROSS_REFERENCE_HPP

#include <map>
#include <set>
#include <string>
#include <vector>

class CrossReference {

private:
    std::map<std::string, std::set<int>> index;
    std::vector<std::string> lines;
    std::string file;

    std::string cleanWord(const std::string& word) const;
    void parseAndInsert(const std::string& line, int lineNumber);
    bool isWordChar(char c) const;

public:

    CrossReference();
    ~CrossReference();

    void loadFromFile(const std::string& filename);
    void show() const;
    void search(const std::string& word) const;
    void add(const std::string& word, int lineNumber);
    void remove(const std::string& word);
    void saveToFile(const std::string& filename) const;
    void stats() const;
    void clear();
    void help() const;

    bool isEmpty() const { return lines.empty(); }
    size_t getUniqueWordsCount() const { return index.size(); }
    size_t getLinesCount() const { return lines.size(); }

};

#endif

