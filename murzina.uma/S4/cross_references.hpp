#ifndef CROSS_REFERENCES_HPP
#define CROSS_REFERENCES_HPP

#include <map>
#include <unordered_map>
#include <set>
#include <vector>
#include <string>

class CrossReferences {
public:
    void addText(const std::string& text);
    void printReferences() const;
    void saveToFile(const std::string& filename) const;
    void clear();
    bool empty() const;
    std::set<int> getLinesForWord(const std::string& word) const;
    void runTests();

private:
    std::map<std::string, std::set<int>> index;
    std::unordered_map<std::string, std::set<int>> fastIndex;
    std::vector<std::string> lines;

    std::string cleanWord(const std::string& raw) const;
};

#endif
