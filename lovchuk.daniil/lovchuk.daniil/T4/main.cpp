#include <iostream>
#include <vector>
#include <memory>
#include <clocale>
#include <cstdlib>
#include <iomanip>

#include "point.h"
#include "shape.h"
#include "rectangle.h"
#include "additional_shapes.h"

using namespace std;

//общая константа
const string ERROR_PREFIX = "Ошибка: ";

void runCorrectTest();

void runErrorTest();

int main() {

    setlocale(LC_ALL, "RUSSIAN");

    try {
        runCorrectTest();
        runErrorTest();

        cout << "Все тесты завершены" << '\n';
        return EXIT_SUCCESS;

    }
    catch (const exception& e) {
        cerr << ERROR_PREFIX << e.what() << '\n';
        return EXIT_FAILURE;
    }
}

// Тест 1: все хорошо
void runCorrectTest() {
    cout << "ТЕСТ 1" << '\n';

    vector<unique_ptr<Shape>> shapes;

    //создаем фгуры
    shapes.push_back(make_unique<Rectangle>(Point(0, 0), Point(4, 2)));
    shapes.push_back(make_unique<Circle>(Point(5, 5), 2.0));
    shapes.push_back(make_unique<Rectangle>(Point(10, 0), Point(12, 3)));
    shapes.push_back(make_unique<Circle>(Point(0, 10), 1.5));

    //создаем составную фигуру
    auto composite = make_unique<CompositeShape>();
    composite->addShape(make_unique<Rectangle>(Point(20, 20), Point(22, 24)));
    composite->addShape(make_unique<Circle>(Point(25, 22), 2.0));
    shapes.push_back(move(composite));

    //до масштабирования
    cout << "Информация о фигурах до масштабирования:" << '\n';
    for (const auto& shape : shapes) {
        shape->print(cout);
        cout << '\n';
    }

    //масштабируем в 2 раза
    cout << "Масштабирование в 2 раза..." << '\n';
    for (auto& shape : shapes) {
        shape->scale(2.0);
    }

    //после масштабирования
    cout << "\nИнформация о фигурах после масштабирования:" << '\n';
    for (const auto& shape : shapes) {
        shape->print(cout);
        cout << '\n';
    }

    cout << "Тест 1 завершён успешно" << '\n';
}

// Тест 2: с ошибкой
void runErrorTest() {
    cout << "ТЕСТ 2:" << '\n';

    try {
        Rectangle invalidRect(Point(10, 10), Point(5, 5));

        cout << "Эта строка не должна выполниться" << '\n';

    }
    catch (const exception& e) {
        cerr << ERROR_PREFIX << e.what() << '\n';
        cout << "Ошибка успешно перехвачена" << '\n';
    }
}
