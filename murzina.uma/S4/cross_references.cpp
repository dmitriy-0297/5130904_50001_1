#include "cross_references.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <stdexcept>

std::string CrossReferences::cleanWord(const std::string& raw) const {
    std::string res;
    for (char ch : raw) {
        if (std::isalnum(static_cast<unsigned char>(ch))) {
            res += std::tolower(static_cast<unsigned char>(ch));
        }
    }
    return res;
}

void CrossReferences::addText(const std::string& text) {
    if (text.empty()) {
        throw std::invalid_argument("Cannot add empty text");
    }

    lines.push_back(text);
    int lineNum = lines.size();

    std::istringstream iss(text);
    std::string word;
    while (iss >> word) {
        std::string cleaned = cleanWord(word);
        if (!cleaned.empty()) {
            index[cleaned].insert(lineNum);
            fastIndex[cleaned].insert(lineNum);
        }
    }
}

void CrossReferences::printReferences() const {
    if (index.empty()) {
        std::cout << "No data.\n";
        return;
    }

    for (std::map<std::string, std::set<int> >::const_iterator it = index.begin(); it != index.end(); ++it) {
        std::cout << it->first << ": ";
        bool first = true;
        for (std::set<int>::const_iterator sit = it->second.begin(); sit != it->second.end(); ++sit) {
            if (!first) std::cout << ", ";
            std::cout << *sit;
            first = false;
        }
        std::cout << "\n";
    }
}

void CrossReferences::saveToFile(const std::string& filename) const {
    if (filename.empty()) {
        throw std::invalid_argument("Filename cannot be empty");
    }

    std::ofstream out(filename);
    if (!out) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    for (std::map<std::string, std::set<int> >::const_iterator it = index.begin(); it != index.end(); ++it) {
        out << it->first << ": ";
        bool first = true;
        for (std::set<int>::const_iterator sit = it->second.begin(); sit != it->second.end(); ++sit) {
            if (!first) out << ", ";
            out << *sit;
            first = false;
        }
        out << "\n";
    }
    std::cout << "Saved to " << filename << "\n";
}

void CrossReferences::clear() {
    index.clear();
    fastIndex.clear();
    lines.clear();
}

bool CrossReferences::empty() const {
    return index.empty();
}

std::set<int> CrossReferences::getLinesForWord(const std::string& word) const {
    if (word.empty()) {
        throw std::invalid_argument("Word cannot be empty");
    }

    std::string cleaned = cleanWord(word);
    std::unordered_map<std::string, std::set<int> >::const_iterator it = fastIndex.find(cleaned);
    if (it != fastIndex.end()) {
        return it->second;
    }
    return std::set<int>();
}

void CrossReferences::runTests() {
    std::cout << "\nRUNNING TESTS\n";

    std::cout << "\nTest 1: Adding lines\n";
    addText("Hello world");
    addText("Hello again");
    addText("This is a test line with world");
    std::cout << "Added 3 lines. Lines count: " << lines.size() << "\n";

    std::cout << "\nTest 2: Finding words\n";
    std::set<int> linesForHello = getLinesForWord("hello");
    std::cout << "Lines for 'hello': ";
    for (std::set<int>::iterator it = linesForHello.begin(); it != linesForHello.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    std::set<int> linesForWorld = getLinesForWord("world");
    std::cout << "Lines for 'world': ";
    for (std::set<int>::iterator it = linesForWorld.begin(); it != linesForWorld.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    std::cout << "\nTest 3: All cross-references\n";
    printReferences();

    std::cout << "\nTest 4: Saving to file\n";
    saveToFile("test_output.txt");

    std::cout << "\nTest 5: Clearing data\n";
    clear();
    std::cout << "Is empty? " << (empty() ? "Yes" : "No") << "\n";

    std::cout << "\nTest 6: Adding after clear\n";
    addText("Fresh start");
    printReferences();

    std::cout << "\nTESTS FINISHED\n";
}
