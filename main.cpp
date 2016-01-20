#include <iostream>
#include <string>
#include "SequenceMatcher.h"

int main()
{
    std::string a = "qabxcd";
    std::string b = "abycdf";
    difflib::SequenceMatcher sm(a, b);

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
            << op.tag << " a[" << op.a1 << ":" << op.a2 << "] (" << a.substr(op.a1, op.a2-op.a1)
            << ") b[" << op.b1 << ":" <<  op.b2 << "] (" << b.substr(op.b1, op.b2-op.b1) << ")" << std::endl;
    }

    return 0;
}

