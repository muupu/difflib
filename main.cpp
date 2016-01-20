#include <iostream>
#include <string>
#include "SequenceMatcher.h"

int main()
{
    std::string a = "abcdeabce";
    std::string b = "abcecabd";
    difflib::SequenceMatcher sm(a, b);
    auto match = sm.FindLongestMatch(0, 9, 0, 8);


    std::cout << match.besti << " " << match.bestj << " " << match.bestsize << std::endl;
    return 0;
}

