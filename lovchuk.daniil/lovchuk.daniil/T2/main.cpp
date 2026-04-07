#include "DataStruct.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <clocale>
#include <cstdlib>

using namespace std;

const string ERROR_PREFIX = "Ошибка: ";

void runTest();

int main() {
    setlocale(LC_ALL, "Russian");

    try {
        runTest();

        return EXIT_SUCCESS;

    }
    catch (const exception& e) {
        cerr << ERROR_PREFIX << e.what() << '\n';
        return EXIT_FAILURE;
    }
}

void runTest() {
    vector<DataStruct> dataVector;

    //читаем с помощью итераторов потока
    istream_iterator<DataStruct> inputStart(cin);
    istream_iterator<DataStruct> inputEnd;

    copy(inputStart, inputEnd, back_inserter(dataVector));

    if (dataVector.empty()) {
        return;
    }

    sort(dataVector.begin(), dataVector.end());

    //выводим с помощью итераторов потока
    ostream_iterator<DataStruct> outputStart(cout, "\n");
    copy(dataVector.begin(), dataVector.end(), outputStart);
}
