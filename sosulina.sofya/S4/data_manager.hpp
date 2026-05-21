#ifndef DATA_MANAGER_HPP
#define DATA_MANAGER_HPP

#include <map>
#include <list>
#include <string>
#include <ostream>

class DataManager {
private:
    std::map<std::string, std::list<int>> data_;
    int currentLine_;

public:
    DataManager();

    bool insert(const std::string& word);
    bool search(const std::string& word) const;
    bool remove(const std::string& word);
    void clear();
    size_t count() const;
    void print(std::ostream& out) const;
    bool showLines(const std::string& word, std::ostream& out) const;
    int getCurrentLine() const { return currentLine_; }
    void incrementLine() { ++currentLine_; }
};

#endif // DATA_MANAGER_HPP