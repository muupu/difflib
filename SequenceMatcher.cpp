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
            /*std::cout << "i:" << i << std::endl;*/
            std::map<int, int> newj2len;
            auto indices = b2j[_a[i]];
            for (auto& j : indices)
            {
                /*std::cout << "j:" << j << std::endl;*/
                if (j < blow)
                    continue;
                if (j >= bhigh)
                    break;
                /*std::cout << "j2len = "; print_map(j2len);
                std::cout << "newj2len = "; print_map(newj2len);*/
                int k = newj2len[j] = j2len[j-1] + 1;
                /*std::cout << "k:" << k << std::endl;
                std::cout << "newj2len = "; print_map(newj2len);*/
                if (k > bestMatch.bestsize)
                {
                    bestMatch.besti = i - k + 1;
                    bestMatch.bestj = j - k + 1;
                    bestMatch.bestsize = k;
                    /*std::cout << "besti, bestj, bestsize =" << bestMatch.besti << bestMatch.bestj << bestMatch.bestsize << std::endl;*/
                }
            }
            j2len = newj2len;
        }
        return bestMatch;
    }

    std::vector<Match> SequenceMatcher::GetMatchingBlocks()
    {
        if (matchingBlocks.size() > 0)
            return matchingBlocks;
        int lenA = _a.size();
        int lenB = _b.size();

        std::queue<QueryRange> queue;
        QueryRange r = {0, lenA, 0, lenB};
        queue.push(r);
        while (!queue.empty())
        {
            QueryRange r = queue.front();
            queue.pop();
            Match match = FindLongestMatch(r.alow, r.ahigh, r.blow, r.bhigh);
            if (match.bestsize) // if k is 0, there was no matching block
            {
                matchingBlocks.push_back(match);
                if (r.alow < match.besti && r.blow < match.bestj)
                {
                    QueryRange newr = {r.alow, match.besti, r.blow, match.bestj};
                    queue.push(newr);
                }
                if ((match.besti + match.bestsize) < r.ahigh && (match.bestj + match.bestsize) < r.bhigh)
                {
                    QueryRange newr = {match.besti + match.bestsize, r.ahigh, match.bestj + match.bestsize, r.bhigh};
                    queue.push(newr);
                }
            }
        }
        std::sort(matchingBlocks.begin(), matchingBlocks.end(), MatchCompare);

        Match match1 = {0};
        std::vector<Match> non_adjacent;
        for (auto& match2 : matchingBlocks)
        {
            if ((match1.besti + match1.bestsize) == match2.besti && (match1.bestj + match1.bestsize) == match2.bestj)
            {
                match1.bestsize += match2.bestsize;
            }
            else
            {
                if (match1.bestsize)
                {
                    non_adjacent.push_back(match1);
                }
                match1 = match2;
            }
        }
        if (match1.bestsize)
        {
            non_adjacent.push_back(match1);
        }
        Match lastmatch = {lenA, lenB, 0};
        non_adjacent.push_back(lastmatch);
        matchingBlocks = non_adjacent;
        return matchingBlocks;
    }

    bool SequenceMatcher::MatchCompare(Match a, Match b)
    {
        if (a.besti != b.besti) return (a.besti < b.besti);
        if (a.bestj != b.bestj) return (a.bestj < b.bestj);
        if (a.bestsize != b.bestsize) return (a.bestsize < b.bestsize);
        return true;
    }
}
