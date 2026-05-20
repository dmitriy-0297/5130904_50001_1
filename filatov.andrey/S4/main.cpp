// main.cpp
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "PhoneBook.h"

int main() {
PhoneBook pb;
std::string line;
std::cout « "Телефонная книга. Вводите команды. Ctrl+D (Linux) или Ctrl+Z (Windows) для выхода." « std::endl;
std::cout « "Команды: add, remove, find, findphone, list, save, load, help" « std::endl;

while (std::getline(std::cin, line)) {
if (line.empty()) continue;

std::istringstream iss(line);
std::string cmd;
iss » cmd;

try {
if (cmd == "add") {
std::string name, phone;
// Простой способ чтения (имя может содержать пробелы до телефона)
std::getline(iss, name, '"'); // если имя в кавычках, но упростим
std::getline(iss, name);
std::getline(iss, phone);
// trim
name.erase(0, name.find_first_not_of(" \t"));
name.erase(name.find_last_not_of(" \t") + 1);
phone.erase(0, phone.find_first_not_of(" \t"));
phone.erase(phone.find_last_not_of(" \t") + 1);
if (!name.empty() && !phone.empty()) {
pb.addContact(name, phone);
std::cout « "Контакт добавлен." « std::endl;
} else {
throw std::runtime_error("Неверный формат add");
}
}
else if (cmd == "remove") {
std::string name;
std::getline(iss, name);
name.erase(0, name.find_first_not_of(" \t"));
name.erase(name.find_last_not_of(" \t") + 1);
if (pb.removeContact(name)) {
std::cout « "Контакт удален." « std::endl;
} else {
std::cout « "Контакт не найден." « std::endl;
}
}
else if (cmd == "find") {
std::string name;
std::getline(iss, name);
name.erase(0, name.find_first_not_of(" \t"));
name.erase(name.find_last_not_of(" \t") + 1);
std::string phone = pb.findByName(name);
if (!phone.empty()) {
std::cout « "Телефон: " « phone « std::endl;
} else {
std::cout « "Контакт не найден." « std::endl;
}
}
else if (cmd == "findphone") {
std::string phone;
std::getline(iss, phone);
phone.erase(0, phone.find_first_not_of(" \t"));
phone.erase(phone.find_last_not_of(" \t") + 1);
std::string name = pb.findByPhone(phone);
if (!name.empty()) {
std::cout « "Имя: " « name « std::endl;
} else {
std::cout « "Контакт не найден." « std::endl;
}
}
else if (cmd == "list") {
pb.listAll();
}
else if (cmd == "save") {
std::string filename;
iss » filename;
if (pb.saveToFile(filename)) {
std::cout « "Сохранено в " « filename « std::endl;
} else {
std::cout « "Ошибка сохранения." « std::endl;
}
}
else if (cmd == "load") {
std::string filename;
iss » filename;
if (pb.loadFromFile(filename)) {
std::cout « "Загружено из " « filename « std::endl;
} else {
std::cout « "Ошибка загрузки." « std::endl;
}
}
else if (cmd == "help") {
std::cout « "Команды:\n"
« "add Имя Телефон\n"
« "remove Имя\n"
« "find Имя\n"
« "findphone Телефон\n"
« "list\n"
« "save файл.txt\n"
« "load файл.txt\n"
« "help\n" « std::endl;
}
else {
std::cout « "Неизвестная команда. Напишите help." « std::endl;
}
} catch (const std::exception& e) {
std::cerr « "Ошибка: " « e.what() « std::endl;
}
}

std::cout « "Программа завершена." « std::endl;
return EXIT_SUCCESS;
}
