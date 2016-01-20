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

    

	class SequenceMatcher
	{
	public:
		SequenceMatcher(std::string a, std::string b);
		~SequenceMatcher();

		// Set the two sequences to be compared.
		void SetSeqs(std::string a, std::string b);

		// Set the first sequence to be compared.
		void SetSeq1(std::string a);

		// Set the second sequence to be compared.
		void SetSeq2(std::string b);

		// Find longest matching block in a[alow:ahigh] and b[blow:bhigh].
		Match FindLongestMatch(int alow, int ahigh, int blow, int bhigh);

		// Return list of triples describing matching subsequences.
		std::vector<Match> GetMatchingBlocks();

		// Return list of 5-tuples describing how to turn a into b.
		void GetOpcodes();
		void GetGroupedOpcodes(int n = 3);

		// Return a measure of the sequences' similarity (float in [0,1]).
		float Ratio();

		// Return an upper bound on .ratio() relatively quickly.
		float QuickRatio();

		// Return an upper bound on ratio() very quickly.
		float RealQuickRatio();

		void GetCloseMatches();

	private:
		void ChainB();
		void CountLeading(const std::string line, const char ch);
        static bool MatchCompare(Match a, Match b);
		// 
		float CalculateRatio(int matches, int length);

	private:
		std::string _a; // first sequence
		std::string _b; // second sequence; differences are computed as 
		                // "what do we need to do to 'a' to change it into 'b' ? "
        std::map<char, std::vector<int>> b2j;
        std::vector<Match> matchingBlocks;
	};

}



#endif  // DIFFLIB_SEQUENCEMATCHER_H_
