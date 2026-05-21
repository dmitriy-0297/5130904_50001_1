#include "data_manager.hpp"
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <iterator>

DataManager::DataManager() : currentLine_(0) {}

bool DataManager::insert(const std::string& word) {
    if (word.empty() || std::all_of(word.begin(), word.end(),
        [](unsigned char c) { return std::isspace(c); })) {
        throw std::invalid_argument("Attempt to insert empty word.");
    }

    std::string normalized;
    normalized.reserve(word.length());
    std::transform(word.begin(), word.end(), std::back_inserter(normalized),
        [](unsigned char c) -> char {
            if (std::isalpha(c)) {
                return static_cast<char>(std::tolower(c));
            }
            return '\0';
        });

    normalized.erase(std::remove(normalized.begin(), normalized.end(), '\0'), normalized.end());

    if (normalized.empty()) {
        throw std::invalid_argument("No valid letters in word.");
    }

    auto& lines = data_[normalized];
    if (lines.empty() || lines.back() != currentLine_) {
        lines.push_back(currentLine_);
    }

    return true;
}

bool DataManager::search(const std::string& word) const {
    return data_.find(word) != data_.end();
}

bool DataManager::remove(const std::string& word) {
    return data_.erase(word) > 0;
}

void DataManager::clear() {
    data_.clear();
    currentLine_ = 0;
}

size_t DataManager::count() const {
    return data_.size();
}

void DataManager::print(std::ostream& out) const {
    std::for_each(data_.begin(), data_.end(),
        [&out](const std::pair<const std::string, std::list<int>>& p) {
            out << p.first << ": ";
            std::copy(p.second.begin(), p.second.end(),
                std::ostream_iterator<int>(out, ", "));
            out << "\n";
        });
}

bool DataManager::showLines(const std::string& word, std::ostream& out) const {
    auto it = data_.find(word);
    if (it == data_.end()) {
        return false;
    }

    out << it->first << ": ";
    std::copy(it->second.begin(), it->second.end(),
        std::ostream_iterator<int>(out, ", "));
    out << "\n";
    return true;
}