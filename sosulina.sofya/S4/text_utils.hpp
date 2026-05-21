#ifndef TEXT_UTILS_HPP
#define TEXT_UTILS_HPP

#include "data_manager.hpp"
#include <string>
#include <vector>

std::string normalizeWord(const std::string& raw);
std::vector<std::string> tokenize(const std::string& line);
void processCommand(const std::string& line, DataManager& manager, bool& running);
void showHelp();

#endif // TEXT_UTILS_HPP