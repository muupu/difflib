#pragma once
#ifndef DIFFLIB_SEQUENCEMATCHER_H_
#define DIFFLIB_SEQUENCEMATCHER_H_

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <queue>
#include <tuple>
#include <algorithm>

namespace difflib{

    struct Match {
        int besti;
        int bestj;
        int bestsize;
    };

    struct QueryRange {
        int alow;
        int ahigh;
        int blow;
        int bhigh;
    };

    struct Opcode {
        std::string tag;
        int a1;
        int a2;
        int b1;
        int b2;
    };

    template<typename Vector>
    void print_vector(Vector v)
    {
       std::cout << '{';
       for(auto& p: v)
            std::cout << *p << ' ';
       std::cout << "}\n";
    }

    template<typename Map>
    void print_map(Map& m)
    {
       std::cout << '{';
       for(auto& p: m) 
       {
            std::cout << p.first << ':' << p.second << ' ';
       }
       std::cout << "}\n";
    }

    
    template <typename ContainerT>
	class SequenceMatcher
	{
	public:
        
		SequenceMatcher(const ContainerT &a, const ContainerT &b)
        {
            SetSeqs(a, b);
        }

        ~SequenceMatcher() 
        {
        }

		// Set the two sequences to be compared.
		void SetSeqs(const ContainerT &a, const ContainerT &b)
        {
            SetSeq1(a);
            SetSeq2(b);
        }

		// Set the first sequence to be compared.
		void SetSeq1(const ContainerT &a)
        {
            if (a == _a)
                return;
            _a = a;
        }

		// Set the second sequence to be compared.
		void SetSeq2(const ContainerT &b)
        {
            if (b == _b)
                return;
            _b = b;
            ChainB();
        }

		// Find longest matching block in a[alow:ahigh] and b[blow:bhigh].
		Match FindLongestMatch(int alow, int ahigh, int blow, int bhigh)
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

		// Return list of triples describing matching subsequences.
		std::vector<Match> GetMatchingBlocks()
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

		// Return list of 5-tuples describing how to turn a into b.
        std::vector<Opcode> GetOpcodes()
        {
            if (opcodes.size() > 0)
                return opcodes;
            int i = 0, j = 0;
            for (auto& match : GetMatchingBlocks())
            {
                std::string tag = "";
                if (i < match.besti && j < match.bestj)
                {
                    tag = "replace";
                }
                else if ( i < match.besti)
                {
                    tag = "delete";
                }
                else if (j < match.bestj)
                {
                    tag = "insert";
                }
                else {}
            
                if (tag.size() > 0)
                {
                    Opcode op = {tag, i, match.besti, j, match.bestj};
                    opcodes.push_back(op);
                }
                i = match.besti + match.bestsize;
                j = match.bestj + match.bestsize;
                if (match.bestsize)
                {
                    Opcode op = {"equal", match.besti, i, match.bestj, j};
                    opcodes.push_back(op);
                }
            }
            return opcodes;
        }
		void GetGroupedOpcodes(int n = 3);

		// Return a measure of the sequences' similarity (float in [0,1]).
		float Ratio();

		// Return an upper bound on .ratio() relatively quickly.
		float QuickRatio();

		// Return an upper bound on ratio() very quickly.
		float RealQuickRatio();

		void GetCloseMatches();

	private:
		void ChainB()
        {
            auto b = _b;
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

		void CountLeading(const std::string line, const char ch);

        static bool MatchCompare(Match a, Match b)
        {
            if (a.besti != b.besti) return (a.besti < b.besti);
            if (a.bestj != b.bestj) return (a.bestj < b.bestj);
            if (a.bestsize != b.bestsize) return (a.bestsize < b.bestsize);
            return true;
        }


		float CalculateRatio(int matches, int length);

	private:
        typedef typename ContainerT::value_type ElementT; // element type of ContainerT
		ContainerT _a; // first sequence
		ContainerT _b; // second sequence
        std::map<ElementT, std::vector<int>> b2j;
        std::vector<Match> matchingBlocks;
        std::vector<Opcode> opcodes;
	};

}



#endif  // DIFFLIB_SEQUENCEMATCHER_H_
