// PhoneBook.h
#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include <map>
#include <string>

class PhoneBook {
private:
    std::map<std::string, std::string> contactsByName;
    std::map<std::string, std::string> contactsByPhone;

public:
    PhoneBook();
    void addContact(const std::string& name, const std::string& phone);
    bool removeContact(const std::string& name);
    std::string findByName(const std::string& name) const;
    std::string findByPhone(const std::string& phone) const;
    void listAll() const;
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
};

#endif
