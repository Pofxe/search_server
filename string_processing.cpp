#include "string_processing.h"

std::string GetPlusWords(std::vector<std::string> vec)
{
    std::string str;

    for (const std::string& s : vec)
    {
        str += s + ",";
    }
    str.pop_back();

    return str;
}

std::vector<std::string> SplitIntoWords(const std::string& text)
{
    std::vector<std::string> words;
    std::string word;

    for (const char c : text) 
    {
        if (c == ' ') 
        {
            if (!word.empty()) 
            {
                words.push_back(word);
                word.clear();
            }
        }
        else 
        {
            word += c;
        }
    }
    if (!word.empty()) 
    {
        words.push_back(word);
    }

    return words;
}