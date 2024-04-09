#include "read_input_functions.h"

std::string BringingStringToLowerCase(std::string& str)
{
    for_each(str.begin(), str.end(), [](char& c)
        {
            c = ::tolower(static_cast<unsigned char>(c));
        });

    return str;
}

std::string ReadLine()
{
    std::string s;
    getline(std::cin, s);

    return BringingStringToLowerCase(s);
}

int ReadLineWithNumber()
{
    int result;
    std::cin >> result;

    ReadLine();

    return result;
}