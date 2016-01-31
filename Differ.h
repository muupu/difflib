#ifndef DIFFLIB_DIFFER_H_
#define DIFFLIB_DIFFER_H_

#include <vector>
#include <string>
#include <iostream>
#include "SequenceMatcher.h"

using namespace std;

namespace difflib{

    inline int _CountLeading(string line, char ch)
    {
        int i = 0;
        int len = line.length();
        while (i < len && line[i] == ch)
        {
            i++;
        }
        return i;
    }

    // trim from end
	inline std::string &rtrim(std::string &str) {
		std::string whitespaces(" \t\f\v\n\r");
		size_t found = str.find_last_not_of(whitespaces);
		if (found != std::string::npos)
        {
			str.erase(found + 1);
        }
		else
		{
			str.clear(); // str is all whitespace
		}
        return str;
    }

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
                cout << "tag, alo, ahi, blo, bhi:"<< op.tag << ", "<< op.a1 << ", " << op.a2 << ", " <<  op.b1 << ", " <<  op.b2 << endl;
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
                    cout << "line1: " << line << endl;
					lines.push_back(line);
				}
			}
			return lines;
		}

	public:
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
            int i, j;
			for (j = blow; j < bhigh; j++)
			{
                cout << "j: " << j << endl; 
				string bj = blines[j];
                cout << "bj: " << bj << endl; 
				cruncher.SetSeq2(bj);
				for (i = alow; i < ahigh; i++)
				{
                    cout << "i: " << i << endl; 
					string ai = alines[i];
					if (ai == bj)
					{
						if (eqi == -1)
						{
							eqi = i;
							eqj = j;
                            cout << "eqi, eqj: " << eqi << " " << eqj << endl; 
						}
						continue;
					}
					cruncher.SetSeq1(ai);
                    cout << "ai: " << ai << endl; 
                    cout << "RealQuickRatio, QuickRatio, Ratio: " << cruncher.RealQuickRatio() << " " << cruncher.QuickRatio() << " " << cruncher.Ratio() << endl; 
                    if (cruncher.RealQuickRatio() > bestratio 
                        && cruncher.QuickRatio() > bestratio
                        && cruncher.Ratio() > bestratio)
                    {
                        bestratio = cruncher.Ratio();
                        besti = i;
                        bestj = j;
                        cout << "best_ratio, best_i, best_j: " << bestratio << " " << besti << " " << bestj << endl; 
                    }
				}
			}
            if (bestratio < cutoff)
            {
                if (eqi == -1)
                {
                    for(auto& line : PlainReplace(alines, alow, ahigh, blines, blow, bhigh))
                    {
                        cout << "line2: " << line << endl;
                        lines.push_back(line);
                    }
                    return lines;
                }
                besti = eqi;
                besti = eqj;
                bestratio = 1.0;
                cout << "best_ratio, best_i, best_j: " << bestratio << " " << besti << " " << bestj << endl; 
            }
            else
            {
                eqi = -1;
            }

            for (auto& line : FancyHelper(alines, alow, besti, blines, blow, bestj))
            {
                cout << "line3: " << line << endl;
                lines.push_back(line);
            }

            string aelt = alines[besti];
            string belt = blines[bestj];
            cout << "aelt, belt: " << aelt << ", " << belt << endl; 
            if (eqi == -1)
            {
                string atags = "";
                string btags = "";
                cruncher.SetSeqs(aelt, belt);
                for (auto op : cruncher.GetOpcodes())
                {
                    cout << "tag, alo, ahi, blo, bhi:"<< op.tag << ", "<< op.a1 << ", " << op.a2 << ", " <<  op.b1 << ", " <<  op.b2 << endl;
                    int la = op.a2 - op.a1;
                    int lb = op.b2 - op.b1;
                    cout << "la, lb: " << la << ", " << lb << endl; 
                    if (op.tag == "replace")
                    {
                        for (int i = 0; i < la; i++) 
                            atags += "^";
                        for (int i = 0; i < lb; i++) 
                            btags += "^";
                    }
                    else if (op.tag == "delete")
                    {
                        for (int i = 0; i < la; i++) 
                            atags += "-";
                    }
                    else if (op.tag == "insert")
                    {
                        for (int i = 0; i < lb; i++) 
                            btags += "+";
                    }
                    else if (op.tag == "equal")
                    {
                        for (int i = 0; i < la; i++) 
                            atags += " ";
                        for (int i = 0; i < lb; i++) 
                            btags += " ";
                    }
                    else
                    {
                    }
                    cout << "atags: " << atags << endl;
                    cout << "btags: " << btags << endl;
                }
                cout << "aelt: " << aelt << endl;
                cout << "belt: " << belt << endl;
                cout << "atags: " << atags << endl;
                cout << "btags: " << btags << endl;
                for (auto line : Qformat(aelt, belt, atags, btags))
                {
                    cout << "line4: " << line << endl;
                    lines.push_back(line);
                }
            }
            else
            {
                cout << "  " + aelt << endl;
                lines.push_back("  " + aelt);
            }

            for (auto line : FancyHelper(alines, besti + 1, ahigh, blines, bestj + 1, bhigh))
            {
                cout << "line5: " << line << endl;
                lines.push_back(line);
            }
			return lines;
		}

		vector<string> FancyHelper(vector<string> alines, int alow, int ahigh, vector<string> blines, int blow, int bhigh)
		{
            vector<string> g;
            if (alow < ahigh)
            {
                if (blow < bhigh)
                {
                    g = FancyReplace(alines, alow, ahigh, blines, blow, bhigh);
                }
                else
                {
                    g = Dump("-", alines, alow, ahigh);
                }
            }
            else if (blow < bhigh)
            {
                g = Dump("+", blines, blow, bhigh);
            }
            return g;
		}

		vector<string> Qformat(string aline, string bline, string atags, string btags)
		{
            vector<string> lines;
            int common;
            common = min(_CountLeading(aline, '\t'), _CountLeading(bline, '\t'));
			cout << "common:" << common << endl;
            common = min(common, _CountLeading(atags.substr(0, common), ' '));
			cout << "common:" << common << endl;
            common = min(common, _CountLeading(btags.substr(0, common), ' '));
			cout << "common:" << common << endl;
            atags = rtrim(atags.substr(common, atags.size()));
			cout << "atags:" << atags << endl;
            btags = rtrim(btags.substr(common, btags.size()));
			cout << "btags:" << btags << endl;

            lines.push_back("- " + aline);
            if (atags.length() > 0)
            {
                string line = "? ";
                for (int i = 0; i < common; i++) 
                    line += "\t";
                line = line + atags + "\n";
                lines.push_back(line);
            }
            lines.push_back("+ " + bline);
            if (btags.length() > 0)
            {
                string line = "? ";
                for (int i = 0; i < common; i++) 
                    line += "\t";
                line = line + btags + "\n";
                lines.push_back(line);
            }
            return lines;
		}

	private:
		vector<string> _alines;
		vector<string> _blines;
	};

}

#endif // DIFFLIB_DIFFER_H_
