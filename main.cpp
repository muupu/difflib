#include <iostream>
#include <string>
#include "SequenceMatcher.h"

int main()
{
    std::string a = "abxcd";
    std::string b = "abcd";
    difflib::SequenceMatcher sm(a, b);
    // auto match = sm.FindLongestMatch(0, 9, 0, 8);
    auto matches = sm.GetMatchingBlocks();
    for (auto match : matches)
    {
        std::cout << match.besti << " " << match.bestj << " " << match.bestsize << std::endl;
    }
    return 0;
}

