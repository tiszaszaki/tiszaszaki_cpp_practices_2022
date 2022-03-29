#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <algorithm>
#include <thread>
#include <chrono>

#include <vector>
#include <list>

#include <map>
#include <set>

using namespace std;
using namespace chrono_literals;

#define SZAMOSSAGTIPUS size_t
#define IGAZSAGTIPUS bool
#define ABECETIPUS char
#define SZOTIPUS string

#define MATRIXTIPUS(T) vector< vector< T > >
#define MATRIX_INIT(T,n) vector< vector< T > >(n, vector< T >(n))

int main(int argc, char** argv)
{
	string fnIn, fnLog="CYK_algoritmus_2021.log";
	SZAMOSSAGTIPUS cWid=0;
	ifstream inFile;
	ofstream logFile;

	map<ABECETIPUS, set<SZOTIPUS>> szabalyok;
	ABECETIPUS kezdoszo;
	SZOTIPUS szo;

	IGAZSAGTIPUS siker=true;
	IGAZSAGTIPUS logNyitva=false;

	siker &= (argc > 2);

	if (siker)
	{
		fnIn = argv[1];
		szo = argv[2];

		if (argc > 3) fnLog = argv[3];

		inFile = ifstream(fnIn);

		siker &= (inFile.good() && (szo != "") && (szo.length() <= 64));
	}

	if (siker)
	{
		set<ABECETIPUS> baloldaliSzavak;
		list<SZOTIPUS> szabalyNyers;
		SZOTIPUS s;

		for (SZAMOSSAGTIPUS i=0;i<szo.length();i++)
		{
			IGAZSAGTIPUS ervenyes=((szo[i] > 32) && (szo[i] < 128));

			if (!ervenyes)
				szo = szo.substr(0, i) + szo.substr(i+1);
		}

		while (!inFile.eof())
		{
			IGAZSAGTIPUS ervenyes=true;

			inFile >> s;

			for (ABECETIPUS k : s)
				ervenyes &= ((k > 32) && (k < 128));

			if (ervenyes)
				szabalyNyers.push_back(s);
		}

		auto it=szabalyNyers.end();
		SZOTIPUS szo1=*(--it);
		SZOTIPUS szo2=*(--it);

		if (szo1 == szo2)
			szabalyNyers.pop_back();

		siker &= (!szabalyNyers.empty());

		if (siker)
		{
			logFile = ofstream(fnLog);
			logNyitva = true;

			s = szabalyNyers.front();

			logFile << s << endl;

			siker &= (s.length() == 1);
		}

		if (siker)
		{
			kezdoszo = s[0];

			logFile << "\"" << kezdoszo << "\"" << endl;

			szabalyNyers.pop_front();

			logFile << endl;
		}
		else
			kezdoszo = '\0';

		while (siker && (!szabalyNyers.empty()))
		{
			s = szabalyNyers.front();

			szabalyNyers.pop_front();

			logFile << s << endl;

			siker &= ((s.length() == 2) || (s.length() == 3));

			if (siker)
			{
				ABECETIPUS kar=s[0];
				set<SZOTIPUS> &temp1=szabalyok[kar];
				SZOTIPUS temp2=s.substr(1);

				baloldaliSzavak.insert(kar);

				temp1.insert(temp2);

				logFile << "\"" << kar << "\" -> \"" << temp2 << "\"" << endl;
			}
		}

		siker &= (!szabalyok.empty());

		if (siker)
		{
			cWid = baloldaliSzavak.size();
		}
	}

	inFile.close();

	if (siker)
	{
		SZAMOSSAGTIPUS n=szo.length();
		SZAMOSSAGTIPUS n2=n-1;
		MATRIXTIPUS(set<ABECETIPUS>) CYK_matrix=MATRIX_INIT(set<ABECETIPUS>, n);
		set<ABECETIPUS> &CYK_kezdosarok=CYK_matrix[0][0];
		vector<SZOTIPUS> felismertReszek;

		SZAMOSSAGTIPUS i,j;

		j = 0;
		for (ABECETIPUS k : szo)
		{
			SZOTIPUS s(1,k);

			set<ABECETIPUS> nemterm;
			stringstream ss;

			for (auto it : szabalyok)
			{
				ABECETIPUS bal=it.first;
				set<SZOTIPUS> jobb=it.second;

				if (jobb.find(s) != jobb.end())
					nemterm.insert(bal);
			}

			CYK_matrix[n-j-1][j] = nemterm;

			j++;
		}
		logFile << endl;

		for (i=n2;i>0;i--)
		{
			for (j=0;j<i;j++)
			{
				SZAMOSSAGTIPUS idx1=i-j-1, idx2=j;
				SZAMOSSAGTIPUS n3=n2-i+1;

				set<ABECETIPUS> ered;

				for (SZAMOSSAGTIPUS l=0;l<n3;l++)
				{
					set<ABECETIPUS> &nt1=CYK_matrix[idx1 + (n3-l)][idx2];
					set<ABECETIPUS> &nt2=CYK_matrix[idx1][idx2 + (l+1)];

					//logFile << i << "," << j << "," << l << " -> " << nt1.size() << "," << nt2.size() << endl;

					for (ABECETIPUS k1 : nt1)
					{
						for (ABECETIPUS k2 : nt2)
						{
							SZOTIPUS s=SZOTIPUS(1,k1) + SZOTIPUS(1,k2);
							IGAZSAGTIPUS talalt=false;

							for (auto it : szabalyok)
							{
								ABECETIPUS bal=it.first;
								set<SZOTIPUS> jobb=it.second;

								if (jobb.find(s) != jobb.end())
								{
									ered.insert(bal);

									//logFile << i << "," << j << "," << l << " -> " << bal << "," << s << endl;

									talalt = true;
								}
							}

							//if (!talalt) logFile << i << "," << j << "," << l << " -> " << s << endl;
						}
					}
				}

				CYK_matrix[idx1][idx2] = ered;
			}
			logFile << endl;
		}

		logFile << "Felismerendo szo: \"" << szo << "\"" << endl;
		logFile << SZOTIPUS((cWid + 1) * n + 1, '-') << endl;

		for (i=0;i<n;i++)
		{
			string temp="";
			stringstream ss1;

			ss1 << "|";

			for (j=0;j<n-i;j++)
			{
				IGAZSAGTIPUS vanKezdoszo=false;

				stringstream ss2;

				for (ABECETIPUS k : CYK_matrix[i][j])
				{
					IGAZSAGTIPUS ezKezdoszo=(k == kezdoszo);

					vanKezdoszo |= (ezKezdoszo);

					if (!ezKezdoszo) ss2 << k;
				}

				temp = ss2.str();

				if (vanKezdoszo)
					temp = SZOTIPUS(1,kezdoszo) + temp;

				if (cWid > 0)
					while (temp.length() < cWid) temp = " " + temp;
				temp += "|";

				ss1 << temp;
			}

			temp = " " + SZOTIPUS(1,szo[n-i-1]);

			ss1 << temp;

			ss1 << endl;

			logFile << ss1.str();
			logFile << SZOTIPUS((cWid + 1) * j + 1, '-') << endl;
		}
		logFile << endl;

		for (i=n;i>0;i--)
		{
			for (j=0;j<i;j++)
			{
				SZAMOSSAGTIPUS idx1=i-j-1, idx2=j;

				if ((idx1 > 0) || (idx2 > 0))
				{
					set<ABECETIPUS> &elem=CYK_matrix[idx1][idx2];

					if (elem.find(kezdoszo) != elem.end())
					{
						SZOTIPUS resz=szo.substr(idx2,szo.length()-idx1-idx2);

						if (find(felismertReszek.begin(),felismertReszek.end(), resz) == felismertReszek.end())
						{
							//logFile << "resz-szo (\"" << resz << "\") felismeresenek a helye: " << idx1 << "," << idx2 << endl;
							felismertReszek.push_back(resz);
						}
					}
				}
			}
		}

		if (!felismertReszek.empty())
			logFile << endl;

		if (CYK_kezdosarok.find(kezdoszo) != CYK_kezdosarok.end())
		{
			stringstream ss;

			logFile << "elfogadva" << endl;
			logFile << endl;
			logFile << "felismert szo: \"" << szo << "\"" << endl;

			ss << "felismert resz-szavak:" << endl;
			for (SZOTIPUS s : felismertReszek)
				ss << "\t" << s << endl;
			ss << endl;

			logFile << ss.str();
		}
		else
			logFile << "elutasitva" << endl;
	}

	if (logNyitva)
	{
		logFile.close();
		logNyitva = false;
	}

	if (siker)
	{
		this_thread::sleep_for(5s);
		return EXIT_SUCCESS;
	}
	else
	{
		this_thread::sleep_for(10s);
		return EXIT_FAILURE;
	}
}
