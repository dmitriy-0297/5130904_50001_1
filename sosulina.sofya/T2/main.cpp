#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstdlib>

#include "data_struct.hpp"
#include "parser.hpp"

void runProgramLogic();

int main()
{
    try
    {
        runProgramLogic();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Critical error: " << ex.what() << "\n";
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "Unknown critical error occurred." << "\n";
        return EXIT_FAILURE;
    }
}

void runProgramLogic()
{
    std::vector<DataStruct> dataArray;

    std::copy(std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(dataArray));

    if (dataArray.empty())
    {
        return;
    }

    std::sort(dataArray.begin(),
        dataArray.end(),
        compareData);

    std::copy(dataArray.begin(),
        dataArray.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n"));
}
