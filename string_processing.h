#pragma once
#include <string>
#include <vector>
#include <set>

#include "search_server.h"

std::vector<std::string> SplitIntoWords(const std::string& text);

std::string GetPlusWords(std::vector<std::string> vec);

template <typename StringContainer>
std::set<std::string> MakeUniqueNonEmptyStrings(const StringContainer& strings) 
{
    std::set<std::string> non_empty_strings;
    for (const std::string& str : strings) 
    {
        if (!str.empty()) 
        {
            non_empty_strings.insert(str);
        }
    }
    return non_empty_strings;
}