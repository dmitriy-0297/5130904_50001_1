#include "parser.hpp"

#include <cstdlib>
#include <stdexcept>
#include <cctype>
#include <string>
#include <vector>

static std::string trimString(const std::string& str)
{
    std::size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos)
    {
        return std::string();
    }
    std::size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

double parseDoubleLiteral(const std::string& valueText)
{
    if (valueText.empty())
    {
        throw std::runtime_error("Empty value for double literal.");
    }

    char suffix = std::tolower(static_cast<unsigned char>(valueText.back()));

    if (suffix != 'd')
    {
        throw std::runtime_error("Double literal must end with 'd' suffix.");
    }

    std::string numericPart = valueText.substr(0, valueText.length() - 1);

    std::size_t dotPosition = numericPart.find('.');

    if (dotPosition == std::string::npos)
    {
        throw std::runtime_error("Double literal must contain a decimal point.");
    }

    if (dotPosition == 0 || dotPosition == numericPart.length() - 1)
    {
        throw std::runtime_error("Digits must exist before and after decimal point.");
    }

    char* conversionEnd = nullptr;
    double parsedValue = std::strtod(numericPart.c_str(), &conversionEnd);

    if (conversionEnd == numericPart.c_str())
    {
        throw std::runtime_error("Failed to convert numeric part to double.");
    }

    return parsedValue;
}

unsigned long long parseUllLiteral(const std::string& valueText)
{
    if (valueText.length() < 4)
    {
        throw std::runtime_error("ULL literal is too short.");
    }

    std::string suffixPart = valueText.substr(valueText.length() - 3);

    for (std::size_t index = 0; index < suffixPart.length(); ++index)
    {
        suffixPart[index] = std::tolower(static_cast<unsigned char>(suffixPart[index]));
    }

    if (suffixPart != "ull")
    {
        throw std::runtime_error("ULL literal must end with 'ull' suffix.");
    }

    std::string numericPart = valueText.substr(0, valueText.length() - 3);

    if (numericPart.empty())
    {
        throw std::runtime_error("Numeric part of ULL literal is empty.");
    }

    char* conversionEnd = nullptr;
    unsigned long long parsedValue = std::strtoull(numericPart.c_str(), &conversionEnd, 10);

    if (conversionEnd == numericPart.c_str() || conversionEnd != numericPart.c_str() + numericPart.length())
    {
        throw std::runtime_error("Invalid characters in ULL numeric part.");
    }

    return parsedValue;
}

std::string parseQuotedString(const std::string& valueText)
{
    if (valueText.length() < 2)
    {
        throw std::runtime_error("String literal must contain opening and closing quotes.");
    }

    char firstChar = valueText.front();
    char lastChar = valueText.back();

    if (firstChar != '"' || lastChar != '"')
    {
        throw std::runtime_error("String literal must be enclosed in double quotes.");
    }

    return valueText.substr(1, valueText.length() - 2);
}

bool parseRecordString(const std::string& recordStr, DataStruct& data)
{
    std::size_t openBracketPos = recordStr.find("(:");
    std::size_t closeBracketPos = recordStr.rfind(":)");

    if (openBracketPos == std::string::npos || closeBracketPos == std::string::npos)
    {
        return false;
    }

    if (closeBracketPos <= openBracketPos + 1)
    {
        return false;
    }

    std::string innerContent = recordStr.substr(openBracketPos + 2, closeBracketPos - openBracketPos - 2);

    std::vector<std::string> tokens;
    std::size_t startPos = 0;
    std::size_t colonPos;

    while ((colonPos = innerContent.find(':', startPos)) != std::string::npos)
    {
        if (colonPos > startPos)
        {
            std::string token = innerContent.substr(startPos, colonPos - startPos);
            std::string trimmedToken = trimString(token);
            if (!trimmedToken.empty())
            {
                tokens.push_back(trimmedToken);
            }
        }
        startPos = colonPos + 1;
    }

    if (startPos < innerContent.length())
    {
        std::string token = innerContent.substr(startPos);
        std::string trimmedToken = trimString(token);
        if (!trimmedToken.empty())
        {
            tokens.push_back(trimmedToken);
        }
    }

    std::string key1Value;
    std::string key2Value;
    std::string key3Value;
    bool foundKey1 = false;
    bool foundKey2 = false;
    bool foundKey3 = false;

    for (const auto& token : tokens)
    {
        std::size_t spacePos = token.find(' ');
        if (spacePos == std::string::npos)
        {
            continue;
        }

        std::string key = token.substr(0, spacePos);
        std::string value = trimString(token.substr(spacePos + 1));

        if (key == "key1")
        {
            key1Value = value;
            foundKey1 = true;
        }
        else if (key == "key2")
        {
            key2Value = value;
            foundKey2 = true;
        }
        else if (key == "key3")
        {
            key3Value = value;
            foundKey3 = true;
        }
    }

    if (!foundKey1 || !foundKey2 || !foundKey3)
    {
        return false;
    }

    try
    {
        data.key1 = parseDoubleLiteral(key1Value);
        data.key2 = parseUllLiteral(key2Value);
        data.key3 = parseQuotedString(key3Value);
    }
    catch (const std::exception&)
    {
        return false;
    }

    return true;
}
