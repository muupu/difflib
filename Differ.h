#ifndef DIFFLIB_DIFFER_H_
#define DIFFLIB_DIFFER_H_

#include <vector>
#include <string>
#include "SequenceMatcher.h"

using namespace std;

namespace difflib{

	class Differ
	{
	public:
		Differ()
		{

		}
		~Differ()
		{

		}

		vector<string> Compare(vector<string> alines, vector<string> blines)
		{
			SequenceMatcher<vector<string>> cruncher(alines, blines);
			vector<string> lines;
			for (auto op : cruncher.GetOpcodes())
			{
				vector<string> g;
				if (op.tag == "replace")
				{
					g = FancyReplace(alines, op.a1, op.a2, blines, op.b1, op.b2);
				}
				else if (op.tag == "delete")
				{
					g = Dump("-", alines, op.a1, op.a2);
				}
				else if (op.tag == "insert")
				{
					g = Dump("+", blines, op.b1, op.b2);
				}
				else if (op.tag == "equal")
				{
					g = Dump(" ", alines, op.a1, op.a2);
				}
				else
				{

				}
				for (auto line : g)
				{
					lines.push_back(line);
				}
			}
			return lines;
		}

	private:
		vector<string> Dump(string tag, vector<string> x, int low, int high)
		{
			vector<string> g;
			for (int i = low; i < high; ++i)
			{
				g.push_back(tag + " " + x[i]);
			}
			return g;
		}

		vector<string> PlainReplace(vector<string> alines, int alow, int ahigh, vector<string> blines, int blow, int bhigh)
		{
			vector<string> first;
			vector<string> second;
			if (bhigh - blow < ahigh - alow)
			{
				first = Dump("+", blines, blow, bhigh);
				second = Dump("-", alines, alow, ahigh);
			}
			else
			{
				first = Dump("-", alines, alow, ahigh);
				second = Dump("+", blines, blow, bhigh);
			}
			vector<string> lines;
			for (auto line : first)
			{
				lines.push_back(line);
			}
			for (auto line : second)
			{
				lines.push_back(line);
			}
			return lines;
		}

		vector<string> FancyReplace(vector<string> alines, int alow, int ahigh, vector<string> blines, int blow, int bhigh)
		{
			double best_ratio = 0.74;
			double cutoff = 0.75;
			SequenceMatcher<vector<string>> cruncher;
			int eqi;
			int eqj;
			for (int j = blow; j < bhigh; j++)
			{

			}

			vector<string> a;
			return a;
		}
		void FancyHelper(vector<string> alines, int alow, int ahigh, vector<string> blines, int blow, int bhigh)
		{

		}

		void Qformat(string aline, string bline, string atags, string btags)
		{

		}

	private:
		vector<string> _alines;
		vector<string> _blines;
	};

}

#endif // DIFFLIB_DIFFER_H_
