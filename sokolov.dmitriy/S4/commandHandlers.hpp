#ifndef COMMAND_HANDLERS_H
#define COMMAND_HANDLERS_H

#include <list>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

using ERDict = std::map<std::wstring, std::list<std::wstring>>;

void handleHelp();

void handleInsert(std::wistringstream &args, ERDict &dict);

void handleRemoveTranslation(std::wistringstream &args, ERDict &dict);

void handleRemoveWord(std::wistringstream &args, ERDict &dict);

void handleSearch(std::wistringstream &args, const ERDict &dict);

void handleGet(std::wistringstream &args, const ERDict &dict);

void handleNumberOfNodes(const ERDict &dict);

void handleOut(const ERDict &dict);

void handleLoad(std::wistringstream &args, ERDict &dict);

bool streamIsEmpty(std::wistream &stream);

std::string wstring_to_string(const std::wstring &wstr);

#endif
