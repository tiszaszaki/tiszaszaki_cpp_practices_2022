#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <vector>
#include <cmath>

using namespace std;

#define SZAMOSSAGTIPUS size_t
#define IGAZSAGTIPUS bool

#define KOORDINATATIPUS double

#define CSUCSTIPUS vector< KOORDINATATIPUS >
#define CSUCSLISTA vector< CSUCSTIPUS >

#define LAPTIPUS vector< SZAMOSSAGTIPUS >
#define LAPLISTA vector< LAPTIPUS >

#define UJ_CSUCS_3D(csl, x,y,z) csl.push_back(CSUCSTIPUS({x,y,z}))
#define UJ_LAP_NEGYSZOG(ll, a,b,c,d) ll.push_back(LAPTIPUS({a,b,c,d}))

template <class T> constexpr T myPI() { return acos(-1); }
template <class T> constexpr T myEPS() { return 1.0f/(256.0f*256.0f*256.0f*256.0f*256.0f*256.0f); }

template <class T> constexpr T ABS(T x) { return (x < 0 ? -x : x); }

#define LENULLAZ(x, T, e) if ((e > 0) && (ABS<T>(x) < e)) x = 0

int main(int argc,char** argv)
{
	SZAMOSSAGTIPUS i,n, j,m;
	CSUCSLISTA csucsok; LAPLISTA lapok;
	ofstream outFile;

	SZAMOSSAGTIPUS pontossag,kitoltes, temp;
	KOORDINATATIPUS spiral_C_AB_diff, spiral_C_AB_tng;
	KOORDINATATIPUS spiral_C_N_min=12, spiral_kor_N_min=8, spiral_C_AB_diff_min=16;
	KOORDINATATIPUS spiral_C_A=-10, spiral_C_B=30,	spiral_C_r1=12, spiral_C_r2=2;
	SZAMOSSAGTIPUS spiral_C_N=24, spiral_C_menet_N=2;
	SZAMOSSAGTIPUS spiral_kor_N=16;

	KOORDINATATIPUS pi_temp=myPI<KOORDINATATIPUS>();
	string fajlnev("spiraltorusz.obj");
	IGAZSAGTIPUS sikeres=true;

	clog.precision(16);
	//clog << pi_temp << endl;

	if (argc > 2)
	{
		KOORDINATATIPUS temp1=atoi(argv[1]), temp2=atoi(argv[2]);
		if ((temp2 - temp1) >= spiral_C_AB_diff_min)
			spiral_C_A = temp1, spiral_C_B = temp2;
	}

	spiral_C_AB_diff = spiral_C_B - spiral_C_A;
	spiral_C_AB_tng = spiral_C_AB_diff / (KOORDINATATIPUS)spiral_C_N;

	if (argc > 4)
	{
		KOORDINATATIPUS temp1=atoi(argv[3]), temp2=atoi(argv[4]);
		if ((temp1 > 0) && (temp2 > 0))
			spiral_C_r1 = temp1, spiral_C_r2 = temp2;
	}

	if (argc > 7)
	{
		SZAMOSSAGTIPUS temp1=atoi(argv[5]), temp2=atoi(argv[6]), temp3=atoi(argv[7]);
		if ((temp1 >= spiral_C_N_min) && (temp2 > 0) && (temp3 >= spiral_kor_N_min))
			spiral_C_N = temp1, spiral_C_menet_N = temp2, spiral_kor_N = temp3;
	}

	n = spiral_C_N;
	m = spiral_kor_N;
	for (i=0;i<n+1;i++)
	{
		KOORDINATATIPUS ang1=2*pi_temp * ((KOORDINATATIPUS)i/n);
		KOORDINATATIPUS x=spiral_C_r1 * cos(ang1 * spiral_C_menet_N);
		KOORDINATATIPUS y=spiral_C_r1 * sin(ang1 * spiral_C_menet_N);
		KOORDINATATIPUS z=spiral_C_A + i * spiral_C_AB_tng;
		//LENULLAZ(x, KOORDINATATIPUS, myEPS<KOORDINATATIPUS>());
		//LENULLAZ(y, KOORDINATATIPUS, myEPS<KOORDINATATIPUS>());
		//LENULLAZ(z, KOORDINATATIPUS, myEPS<KOORDINATATIPUS>());
		for (j=0;j<m;j++)
		{
			KOORDINATATIPUS ang2=2*pi_temp * ((KOORDINATATIPUS)j/m) - pi_temp;
			KOORDINATATIPUS x2=spiral_C_r2 * sin(ang1) * cos(ang2);
			KOORDINATATIPUS y2=spiral_C_r2 * sin(ang1) * sin(ang2);
			KOORDINATATIPUS z2=spiral_C_r2 * cos(ang1);
			//LENULLAZ(x2, KOORDINATATIPUS, myEPS<KOORDINATATIPUS>());
			//LENULLAZ(y2, KOORDINATATIPUS, myEPS<KOORDINATATIPUS>());
			//LENULLAZ(z2, KOORDINATATIPUS, myEPS<KOORDINATATIPUS>());
			UJ_CSUCS_3D(csucsok, x + x2,y + y2,z + z2);
		}
	}
	for (i=0;i<n;i++)
	{
		for (j=0;j<m;j++)
		{
			SZAMOSSAGTIPUS a=i*m + j;
			SZAMOSSAGTIPUS b=i*m + (j+1) % m;
			SZAMOSSAGTIPUS c=(i+1) * m + (j+1) % m;
			SZAMOSSAGTIPUS d=(i+1) * m + j;
			UJ_LAP_NEGYSZOG(lapok, a,b,c,d);
		}
	}

	outFile = ofstream(fajlnev);

	sikeres &= (outFile.good());

	if (sikeres)
	{
		pontossag = 8, kitoltes = pontossag + 6;
		outFile.precision(pontossag);

		n = csucsok.size();
		if (n > 0)
		{
			for (CSUCSTIPUS cs : csucsok)
			{
				stringstream ss("");

				ss << "v";
				for (KOORDINATATIPUS k : cs) ss << setw(kitoltes) << setfill(' ') << k;
				ss << endl;

				outFile << ss.str();
			}
		}

		temp = 2*(n+1);
		kitoltes = 3;
		while (temp >= 10) kitoltes++, temp /= 10;

		n = lapok.size();
		if (n > 0)
		{
			for (LAPTIPUS l : lapok)
			{
				stringstream ss("");

				ss << "f";
				for (KOORDINATATIPUS k : l) ss << setw(kitoltes) << setfill(' ') << k+1;
				ss << endl;

				outFile << ss.str();
			}
		}
	}
	else
		clog << "Fájlkezelési hiba!" << endl;

	outFile.close();

	if (sikeres) return EXIT_SUCCESS;
	else return EXIT_FAILURE;
}
