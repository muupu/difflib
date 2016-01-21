#include <iostream>
#include <string>
#include "SequenceMatcher.h"
#include <list>

int main()
{
    /*std::string a = "qabxcd";
    std::string b = "abycdf";
    difflib::SequenceMatcher<std::string> sm(a, b);*/

    char arra[] = {'q', 'a', 'b', 'x', 'c', 'd'};
    char arrb[] = {'a', 'b', 'y', 'c', 'd', 'f'};
    std::vector<char> a(arra, arra+6);
    std::vector<char> b(arrb, arrb+6);
    difflib::SequenceMatcher<std::vector<char>> sm(a, b);

    /* 暂不支持list容器（无法使用operator[]）
    std::list<char> a(arra, arra+6);
    std::list<char> b(arrb, arrb+6);
    difflib::SequenceMatcher<std::list<char>> sm(a, b);*/

    // auto match = sm.FindLongestMatch(0, 9, 0, 8);
    /*auto matches = sm.GetMatchingBlocks();
    for (auto match : matches)
    {
        std::cout << match.besti << " " << match.bestj << " " << match.bestsize << std::endl;
    }*/

    auto ops = sm.GetOpcodes();
    for (auto op : ops)
    {
        std::cout /*<< "%7s a[%d:%d] (%s) b[%d:%d] (%s)" */
            << op.tag << " a[" << op.a1 << ":" << op.a2 << "] (" /*<< a.substr(op.a1, op.a2-op.a1)*/
            << ") b[" << op.b1 << ":" <<  op.b2 << "] (" /*<< b.substr(op.b1, op.b2-op.b1)*/ << ")" << std::endl;
    }

    return 0;
}

