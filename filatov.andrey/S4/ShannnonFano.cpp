#include "ShannonFano.h"
#include <iostream>
#include <algorithm>

ShannonFano::ShannonFano() {}

void ShannonFano::setText(const std::string& text) {
    originalText = text;
}

const std::string& ShannonFano::getText() const {
    return originalText;
}

void ShannonFano::sortSymbols() {
    for (size_t i = 0; i < symbols.size(); i++) {
        for (size_t j = i + 1; j < symbols.size(); j++) {
            if (symbols[i].freq < symbols[j].freq) {
                std::swap(symbols[i], symbols[j]);
            }
        }
    }
}

void ShannonFano::buildCodes(int start, int end, std::string currentCode) {
    if (start > end) return;
    if (start == end) {
        symbols[start].code = currentCode;
        return;
    }

    int total = 0;
    for (int i = start; i <= end; i++) {
        total += symbols[i].freq;
    }

    int half = total / 2;
    int sum = 0;
    int split = start;

    for (int i = start; i <= end; i++) {
        sum += symbols[i].freq;
        if (sum >= half) {
            split = i;
            break;
        }
    }
    if (split == start && start != end) split++;

    buildCodes(start, split, currentCode + "0");
    buildCodes(split + 1, end, currentCode + "1");
}

void ShannonFano::countFrequencies() {
    std::map<char, int> freqMap;
    for (char c : originalText) {
        if (c != '\n' && c != '\r' && c != '\0') {
            freqMap[c]++;
        }
    }

    symbols.clear();
    for (auto& p : freqMap) {
        symbols.push_back({p.first, p.second, ""});
    }

    sortSymbols();
}

void ShannonFano::generateCodes() {
    if (symbols.empty()) return;
    buildCodes(0, (int)symbols.size() - 1, "");
}

std::string ShannonFano::encode(const std::string& text) const {
    std::map<char, std::string> codeMap;
    for (const auto& s : symbols) {
        codeMap[s.ch] = s.code;
    }

    std::string result = "";
    for (char c : text) {
        if (codeMap.count(c)) {
            result += codeMap[c];
        }
    }
    return result;
}

std::string ShannonFano::decode(const std::string& encodedText) const {
    std::string result = "";
    std::string current = "";

    for (char bit : encodedText) {
        current += bit;
        bool found = false;
        for (const auto& s : symbols) {
            if (s.code == current) {
                result += s.ch;
                current = "";
                found = true;
                break;
            }
        }
    }
    return result;
}

void ShannonFano::printCodes() const {
    std::cout << "\n=== Коды Шеннона-Фано ===\n";
    for (const auto& s : symbols) {
        std::cout << "'" << s.ch << "' (" << s.freq << ") -> " << s.code << "\n";
    }
}

void ShannonFano::printStatistics() const {
    if (originalText.empty()) return;
    std::string encoded = encode(originalText);

    std::cout << "\n=== Статистика сжатия ===\n";
    std::cout << "Исходный размер:     " << originalText.length() << " байт\n";
    std::cout << "Сжатый размер:       " << encoded.length() << " бит\n";

    double compression = (1.0 - (double)encoded.length() / (originalText.length() * 8)) * 100.0;
    std::cout << "Степень сжатия:      " << compression << " %\n";
}
