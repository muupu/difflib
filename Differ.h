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
			double bestratio = 0.74;
			double cutoff = 0.75;
			SequenceMatcher<string> cruncher;
			int eqi = -1;
			int eqj = -1;
            int besti = -1;
            int bestj = -1;
            vector<string> lines;
			for (int j = blow; j < bhigh; j++)
			{
				string bj = blines[j];
				cruncher.SetSeq2(bj);
				for (int i = alow; i < ahigh; i++)
				{
					string ai = alines[i];
					if (ai == bj)
					{
						if (eqi == -1)
						{
							eqi = i;
							eqj = j;
						}
						continue;
					}
					cruncher.SetSeq1(ai);
				}
                if (cruncher.RealQuickRatio() > bestratio 
                    && cruncher.QuickRatio() > bestratio
                    && cruncher.Ratio() > bestratio)
                {
                    bestratio = cruncher.Ratio();
                    besti = i;
                    bestj = j;
                }
			}
            if (bestratio < cutoff)
            {
                if (eqi == -1)
                {
                    for(auto& line : PlainReplace(alines, alow, ahigh, blines, blow, bhigh))
                    {
                        lines.push_back(line);
                    }
                    return lines;
                }
                besti = eqi;
                besti = eqj;
                bestratio = 1.0;
            }
            else
            {
                eqi = -1;
            }

            for (auto& line : FancyHelper(alines, alow, ahigh, blines, blow, bhigh))
            {
                lines.push_back(line);
            }

            string aelt = alines[besti];
            string belt = blines[bestj];
            if (eqi == -1)
            {
                string atags = "";
                string btags = "";
                cruncher.SetSeqs(aelt, belt);
                for (auto& op : cruncher.GetOpcodes())
                {
                    int la = op.a2 - op.a1;
                    int lb = op.b2 - op.b1;
                    if (op.tag == "replace")
                    {
                        for (int i = 0; i <= la; i++) 
                            atags += "^";
                        for (int i = 0; i <= lb; i++) 
                            btags += "^";
                    }
                    else if (op.tag == "delete")
                    {
                        for (int i = 0; i <= la; i++) 
                            atags += "-";
                    }
                    else if (op.tag == "insert")
                    {
                        for (int i = 0; i <= lb; i++) 
                            btags += "+";
                    }
                    else if (op.tag == "equal")
                    {
                        for (int i = 0; i <= la; i++) 
                            atags += " ";
                        for (int i = 0; i <= lb; i++) 
                            btags += " ";
                    }
                    else
                    {
                    }
                }

                for (auto& line : Qformat(aelt, belt, atags, btags))
                {
                    lines.push_back(line);
                }
            }
            else
            {
                lines.push_back("  " + aelt);
            }

            for (auto& line : FancyHelper(alines, besti + 1, ahigh, blines, bestj + 1, bhigh))
            {
                lines.push_back(line);
            }
			return lines;
		}
		vector<string> FancyHelper(vector<string> alines, int alow, int ahigh, vector<string> blines, int blow, int bhigh)
		{

		}

		vector<string> Qformat(string aline, string bline, string atags, string btags)
		{

		}

	private:
		vector<string> _alines;
		vector<string> _blines;
	};

}

#endif // DIFFLIB_DIFFER_H_