#include "toUpperCase.hpp"
#include <algorithm>
#include <locale>
#include <string>

void toUpperCase(std::wstring &s) {

    static std::locale loc("");

    std::transform(
        s.cbegin(),
        s.cend(),
        s.begin(),
        [](const wchar_t c) {
            return std::toupper(c, loc);
        }
    );
}
