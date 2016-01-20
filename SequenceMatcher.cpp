#include "SequenceMatcher.h"

namespace difflib{

    SequenceMatcher::SequenceMatcher(std::string a, std::string b)
    {
        SetSeqs(a, b);
    }


    SequenceMatcher::~SequenceMatcher()
    {
    }

    // Set the two sequences to be compared.
    void SequenceMatcher::SetSeqs(std::string a, std::string b)
    {
        SetSeq1(a);
        SetSeq2(b);
    }

    // Set the first sequence to be compared.
    void SequenceMatcher::SetSeq1(std::string a)
    {
        if (a == _a)
            return;
        _a = a;
    }

    // Set the second sequence to be compared.
    void SequenceMatcher::SetSeq2(std::string b)
    {
        if (b == _b)
            return;
        _b = b;
        ChainB();
    }

    void SequenceMatcher::ChainB()
    {
        std::string b = _b;
        for (auto iter = b.begin(); iter != b.end(); ++iter)
        {
            b2j[*iter].push_back(iter-b.begin());

            // print b2j
            //std::cout << "i:"  << iter-b.begin() << " elt:" << *iter << std::endl;
            //std::cout << "b2j = "; //print_map(b2j);
            //std::cout << "{";
            //for(auto& p: b2j)
            //{
            //    std::cout << p.first << ": { ";
            //    for(auto& i: p.second)
            //    {
            //        std::cout << i << ':';
            //    }
            //    std::cout << " }";
            //}
            //std::cout << "}\n";
        }
    }

    Match SequenceMatcher::FindLongestMatch(int alow, int ahigh, int blow, int bhigh)
    {
        Match bestMatch = {alow, blow, 0};
        std::map<int, int> j2len;
        for (int i = alow; i < ahigh; ++i)
        {
            std::cout << "i:" << i << std::endl;

            std::map<int, int> newj2len;
            auto indices = b2j[_a[i]];
            for (auto& j : indices)
            {
                std::cout << "j:" << j << std::endl;
                if (j < blow)
                    continue;
                if (j >= bhigh)
                    break;
                std::cout << "j2len = "; print_map(j2len);
                std::cout << "newj2len = "; print_map(newj2len);
                int k = newj2len[j] = j2len[j-1] + 1;
                std::cout << "k:" << k << std::endl;
                std::cout << "newj2len = "; print_map(newj2len);
                if (k > bestMatch.bestsize)
                {
                    bestMatch.besti = i - k + 1;
                    bestMatch.bestj = j - k + 1;
                    bestMatch.bestsize = k;
                    std::cout << "besti, bestj, bestsize =" << bestMatch.besti << bestMatch.bestj << bestMatch.bestsize << std::endl;
                }
            }
            j2len = newj2len;
        }


        return bestMatch;
    }

}
