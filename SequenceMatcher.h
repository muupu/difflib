#pragma once
#ifndef DIFFLIB_SEQUENCEMATCHER_H_
#define DIFFLIB_SEQUENCEMATCHER_H_

#include <string>
#include <vector>

namespace difflib{

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
		void FindLongestMatch(int alow, int ahigh, int blow, int bhigh);

		// Return list of triples describing matching subsequences.
		void GetMatchingBlocks();

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

		// 
		float CalculateRatio(int matches, int length);

	private:
		std::string _a; // first sequence
		std::string _b; // second sequence; differences are computed as 
		                // "what do we need to do to 'a' to change it into 'b' ? "
	};

}



#endif  // DIFFLIB_SEQUENCEMATCHER_H_
