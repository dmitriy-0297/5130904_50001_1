#include "data_struct.hpp"
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

int main()
{
    using murzina::DataStruct;

    std::vector<DataStruct> data;

    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data)
    );

    std::cout << "Прочитано объектов: " << data.size() << std::endl;
    std::cout << "Результат:" << std::endl;

    std::sort(data.begin(), data.end(), murzina::compareDataStruct);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
