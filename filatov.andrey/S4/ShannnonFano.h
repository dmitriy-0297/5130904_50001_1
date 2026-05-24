#ifndef SHANNON_FANO_H
#define SHANNON_FANO_H

#include <string>
#include <vector>
#include <map>

struct Symbol {
    char ch;
    int freq;
    std::string code;
};

class ShannonFano {
private:
    std::vector<Symbol> symbols;
    std::string originalText;

    void sortSymbols();
    void buildCodes(int start, int end, std::string currentCode);

public:
    ShannonFano();

    void setText(const std::string& text);
    void countFrequencies();
    void generateCodes();
    std::string encode(const std::string& text) const;
    std::string decode(const std::string& encodedText) const;

    void printCodes() const;
    void printStatistics() const;

    const std::string& getText() const;
};

#endif
