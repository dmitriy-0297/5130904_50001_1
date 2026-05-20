// PhoneBook.cpp
#include "PhoneBook.h"
#include <iostream>
#include <fstream>
#include <algorithm>

PhoneBook::PhoneBook() {}

void PhoneBook::addContact(const std::string& name, const std::string& phone) {
    if (name.empty() || phone.empty()) {
        throw std::invalid_argument("Имя и телефон не могут быть пустыми");
    }
    // Удаляем старый, если есть
    auto it = contactsByName.find(name);
    if (it != contactsByName.end()) {
        contactsByPhone.erase(it->second);
    }
    contactsByName[name] = phone;
    contactsByPhone[phone] = name;
}

bool PhoneBook::removeContact(const std::string& name) {
    auto it = contactsByName.find(name);
    if (it != contactsByName.end()) {
        contactsByPhone.erase(it->second);
        contactsByName.erase(it);
        return true;
    }
    return false;
}

std::string PhoneBook::findByName(const std::string& name) const {
    auto it = contactsByName.find(name);
    if (it != contactsByName.end()) {
        return it->second;
    }
    return "";
}

std::string PhoneBook::findByPhone(const std::string& phone) const {
    auto it = contactsByPhone.find(phone);
    if (it != contactsByPhone.end()) {
        return it->second;
    }
    return "";
}

void PhoneBook::listAll() const {
    if (contactsByName.empty()) {
        std::cout << "Телефонная книга пуста." << std::endl;
        return;
    }
    std::cout << "Список контактов:" << std::endl;
    // Используем range-based for (минимально)
    for (const auto& p : contactsByName) {
        std::cout << p.first << " : " << p.second << std::endl;
    }
}

bool PhoneBook::saveToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) return false;
    for (const auto& p : contactsByName) {
        out << p.first << "|" << p.second << std::endl;
    }
    return true;
}

bool PhoneBook::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) return false;
    contactsByName.clear();
    contactsByPhone.clear();
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        size_t pos = line.find('|');
        if (pos != std::string::npos) {
            std::string name = line.substr(0, pos);
            std::string phone = line.substr(pos + 1);
            addContact(name, phone);
        }
    }
    return true;
}
