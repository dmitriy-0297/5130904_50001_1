#include "ShannonFano.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "Russian");

    ShannonFano sf;
    std::string line;

    std::cout << "=== Коды Шеннона-Фано ===\n";
    std::cout << "Команды: load, text, freq, build, encode, decode, stats, help\n";
    std::cout << "Выход: Ctrl+D или Ctrl+Z\n\n";

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd == "help") {
            std::cout << "load <файл>   - загрузить текст из файла\n";
            std::cout << "text          - ввести текст вручную\n";
            std::cout << "freq          - посчитать частоты\n";
            std::cout << "build         - построить коды\n";
            std::cout << "encode        - закодировать текст\n";
            std::cout << "decode <код>  - раскодировать\n";
            std::cout << "stats         - статистика сжатия\n";
            std::cout << "help          - справка\n";
        }
        else if (cmd == "load") {
            std::string filename;
            iss >> filename;
            std::ifstream file(filename);
            if (file.is_open()) {
                std::string text((std::istreambuf_iterator<char>(file)),
                                 std::istreambuf_iterator<char>());
                sf.setText(text);
                std::cout << "Текст загружен (" << text.length() << " символов)\n";
            } else {
                std::cout << "Не удалось открыть файл!\n";
            }
        }
        else if (cmd == "text") {
            std::cout << "Введите текст (пустая строка — завершить ввод):\n";
            std::string text, temp;
            while (std::getline(std::cin, temp) && !temp.empty()) {
                text += temp + "\n";
            }
            sf.setText(text);
            std::cout << "Текст сохранён.\n";
        }
        else if (cmd == "freq") {
            sf.countFrequencies();
            std::cout << "Частоты посчитаны.\n";
        }
        else if (cmd == "build") {
            sf.countFrequencies();
            sf.generateCodes();
            sf.printCodes();
        }
        else if (cmd == "encode") {
            std::string encoded = sf.encode(sf.getText());
            std::cout << "Закодировано: " << encoded.substr(0, 120);
            if (encoded.length() > 120) std::cout << "...";
            std::cout << "\n";
        }
        else if (cmd == "decode") {
            std::string code;
            std::getline(iss, code);
            code.erase(0, code.find_first_not_of(" \t"));
            std::string decoded = sf.decode(code);
            std::cout << "Раскодировано: " << decoded << "\n";
        }
        else if (cmd == "stats") {
            sf.printStatistics();
        }
        else {
            std::cout << "Неизвестная команда. Введите help\n";
        }
    }

    std::cout << "\nПрограмма завершена.\n";
    return EXIT_SUCCESS;
}
