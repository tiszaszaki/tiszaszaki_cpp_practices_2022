#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>
#include <functional>
#include <conio.h>

using namespace std;

// // típusok

#define PALYAMERETTIPUS unsigned int
#define SZAMOSSAGTIPUS int
#define PALYAELEMTIPUS int
#define IDOMERTEKTIPUS unsigned int
#define IRANYTIPUS unsigned int
#define PALYAMEGJELENESTIPUS char
#define BILLIRANYITTIPUS char
#define IGAZSAGTIPUS bool
#define CIMKETIPUS string
#define PALYATIPUS vector<PALYAELEMTIPUS>
#define ELEMGYUJTTIPUS vector<PALYAELEMTIPUS>
#define MASZKTIPUS vector<IGAZSAGTIPUS>
#define MASZKVEKTORTIPUS vector< MASZKTIPUS >
#define ELEMGYUJTVEKTORTIPUS vector<ELEMGYUJTTIPUS>
#define TEXTURAKESZLETTIPUS vector<PALYAMEGJELENESTIPUS>
#define TARTOMANYTIPUS double

// // konstansok (varázs-számok)

// pályaelemek

#define URES 0
#define FAL 1
#define EMBER 2
#define LEVEGO 3
#define DRAGAKO 4
#define ELLENFEL 5
#define KONNYUFAL 6
#define KO 7
#define BOMBA 8
#define KIJARAT 9
#define CSILLAGOK 10
#define LADA 11
#define RAKODO 12
#define TAVOZOEMBER 13

#define ELEMFAJTASZAM 14

// mozgási irányok

#define JOBBRA 0
#define FEL 1
#define BALRA 2
#define LE 3

#define IRANYSZAM 4

// // vektor-indexek

// maszkok

#define ROBBANAS 0
#define GURULAS 1
#define UTKOZES 2
#define CSILLAGROBBANAS 3
#define MOZGATAS 4
#define ELTOLAS 5

#define MASZKOKSZAMA 6

// pályaelemek (terv)

#define ELEMROBBANAS 0
#define ELEMETELTUNTET 1
#define ELEMENGURUL 2
#define ELEMMELUTKOZIK 3

// pályagenerálás (terv)

#define GENERAL_MIT 0
#define GENERAL_HOVA 1

// // vektorkonstansok

// iránytípusúak

#define MINDENIRANY vector<IRANYTIPUS>({JOBBRA,FEL,BALRA,LE})
#define GURULASIRANY vector<IRANYTIPUS>({BALRA,JOBBRA})

// elemtípusúak

#define URESELEM ELEMGYUJTTIPUS({URES})
#define BOMBAROBBAN ELEMGYUJTTIPUS({FAL,EMBER,ELLENFEL,KONNYUFAL,CSILLAGOK})
#define CSILLAGROBBAN ELEMGYUJTTIPUS({EMBER,ELLENFEL})
#define KOCSUSZIK ELEMGYUJTTIPUS({DRAGAKO,KONNYUFAL,KO,BOMBA})
#define KOROBBAN ELEMGYUJTTIPUS({EMBER,ELLENFEL,BOMBA,CSILLAGOK})
#define BOMBACSUSZIK ELEMGYUJTTIPUS({DRAGAKO,KO,BOMBA})
#define NINCSELEM ELEMGYUJTTIPUS()
#define MINDENFAJTAELEM konstansLekepezes(0,ELEMFAJTASZAM-1)

// elemvektorok

#define EMBER_ELEMVEKTOROK ELEMGYUJTVEKTORTIPUS({NINCSELEM,ELEMGYUJTTIPUS({LEVEGO})})
#define ELLENFEL_ELEMVEKTOROK ELEMGYUJTVEKTORTIPUS({ELEMGYUJTTIPUS({EMBER}),NINCSELEM})
#define KO_ELEMVEKTOROK ELEMGYUJTVEKTORTIPUS({KOROBBAN,NINCSELEM,KOCSUSZIK})
#define BOMBA_ELEMVEKTOROK ELEMGYUJTVEKTORTIPUS({BOMBAROBBAN,NINCSELEM,BOMBACSUSZIK})
#define CSILLAG_ELEMVEKTOROK ELEMGYUJTVEKTORTIPUS({CSILLAGROBBAN,NINCSELEM})

// megjelenéstípusúak (textúrakészletek)

#define PELDAMEGJELENES TEXTURAKESZLETTIPUS({' ','#','@',':','*','&','|','o','+','E','%','=','_','@'})
#define MASZKMEGJELENES TEXTURAKESZLETTIPUS({'.','#'})

// maszktípusúak

#define URESMASZK MASZKTIPUS()
#define PALYAMASZK(n,m,b) MASZKTIPUS(n*m,b)
#define URES_PALYAMASZK(n,m) PALYAMASZK(n,m,false)
#define URES_PALYAMASZK_VEKTOR(n2,n,m) vector<MASZKTIPUS>(n2,PALYAMASZK(n,m,false))
#define TELJES_PALYAMASZK(n,m) PALYAMASZK(n,m,true)

// // makró-függvények

#define PALYALINEARIS(i,j,n,m) ((j)+(i)*(m)) // vagy: ((i)+(j)*(n))
#define EMBERMOZOG(IRANY) mozgatElem(palya,n,m,EMBER,EMBER_ELEMVEKTOROK,vector<IRANYTIPUS>({IRANY}),maszkok,-1)
#define EMBERMOZOGMINDENHOVA mozgatElem(palya,n,m,EMBER,EMBER_ELEMVEKTOROK,MINDENIRANY,maszkok,-1)
#define KERES(A,e) (find(A.begin(),A.end(),e)!=A.end())
#define KERESPALYAELEM(A,palya,i,j,n,m) (KERES(A,palya[PALYALINEARIS(i,j,n,m)]))
#define KERESRANEZOELEMRE(A,palya,i,j,n,m,irany) (KERES(A,ranezIranyba(palya,n,m,i,j,vector<IRANYTIPUS>({irany}))))
#define LEPTET(palya,i,j,idiff,jdiff,n,m) swap(palya[PALYALINEARIS(i,j,n,m)],palya[PALYALINEARIS(i+idiff,j+jdiff,n,m)])
#define MERETHIBA(n,m,A) if (n*m>A.size()) return false
#define VEKTOR_MERETHIBA(n,m,V) for (auto A : V) MERETHIBA(n,m,A)
#define MASZK_SZAMOSSAGHIBA(M,n,m) if (maszkSzamossag(M,n,m)<=0) return false
#define MASZKVEKTOR_SZAMOSSAGHIBA(MV,n,m) IGAZSAGTIPUS b_mvszh=false; for (auto M : MV) { b_mvszh|=maszkSzamossag(M,n,m)<=0; }; if (!b_mvszh) return false
#define VEKTOR_MERETEGYEZES_HIBA(v1,v2) if (v1.size()!=v2.size()) return false

ELEMGYUJTTIPUS konstansLekepezes(PALYAELEMTIPUS a,PALYAELEMTIPUS b)
{
	ELEMGYUJTTIPUS res;
	for (PALYAELEMTIPUS i=a;i<=b;i++) res.push_back(i);
	return res;
}

template<class T> T veletlenErtek(T min,T max)
{
	auto seed=chrono::system_clock::now().time_since_epoch().count();
	/*random_device rd;
	mt19937 gen(rd());*/
	mt19937 gen(seed);
	uniform_int_distribution<T> dis(min,max);
	return dis(gen);
}

void feltoltPalya(PALYATIPUS & palya,PALYAMERETTIPUS n,PALYAMERETTIPUS m,ELEMGYUJTTIPUS ev)
{
	palya.clear();
	for (PALYAMERETTIPUS i=0;i<n;i++)
	{
		for (PALYAMERETTIPUS j=0;j<m;j++)
		{
			PALYAELEMTIPUS e=LEVEGO;
			if (ev.size()>1) e=ev[veletlenErtek<PALYAELEMTIPUS>(0,ev.size()-1)];
			else
			{
				if (ev.size()==1) e=ev[0];
			}
			palya.push_back(e);
		}
	}
}

void feldolgozSor(PALYATIPUS & palya,PALYAMERETTIPUS & n,PALYAMERETTIPUS & m,PALYAMERETTIPUS i,string s,TEXTURAKESZLETTIPUS pm)
{
	if (n*m<=palya.size())
	{
		PALYAMERETTIPUS j=0;
		for (PALYAMEGJELENESTIPUS k : s)
		{
			if (j>=m) break;
			int l1=-1;
			for (unsigned int l2=0;l2<pm.size();l2++)
				if (pm[l2]==k) { l1=l2; break; }
			if (l1>=0) palya[PALYALINEARIS(i,j,n,m)]=l1;
			else palya[PALYALINEARIS(i,j,n,m)]=URES;
			j++;
		}
	}
}

void beolvasPalya(PALYATIPUS & palya,PALYAMERETTIPUS & n,PALYAMERETTIPUS & m,istream & is,TEXTURAKESZLETTIPUS pm)
{
	string s;
	getline(is,s);
	stringstream ss(s);
	ss>>n>>m;
	feltoltPalya(palya,n,m,URESELEM);
	PALYAMERETTIPUS i=0;
	while (!is.eof()&&(i<n))
	{
		getline(is,s);
		feldolgozSor(palya,n,m,i,s,pm);
		i++;
	}
}

void bekeretezPalya(PALYATIPUS & palya,PALYAMERETTIPUS n,PALYAMERETTIPUS m,ELEMGYUJTTIPUS kev,PALYAMERETTIPUS km)
{
	if (n*m<=palya.size())
	for (PALYAMERETTIPUS i=0;i<n;i++)
	{
		for (PALYAMERETTIPUS j=0;j<m;j++)
		{
			if ((i<km)||(j<km)||(i>=n-km)||(j>=m-km))
			{
				PALYAELEMTIPUS ke=FAL;
				if (kev.size()>1) ke=kev[veletlenErtek<PALYAELEMTIPUS>(0,kev.size()-1)];
				else
				{
					if (kev.size()==1) ke=kev[0];
				}
				palya[PALYALINEARIS(i,j,n,m)]=ke;
			}
		}
	}
}

void kitoltPalya(PALYATIPUS & palya,PALYAMERETTIPUS n,PALYAMERETTIPUS m,ELEMGYUJTTIPUS e1,ELEMGYUJTTIPUS e2);

void szetszorPalyan(PALYATIPUS & palya,PALYAMERETTIPUS n,PALYAMERETTIPUS m,ELEMGYUJTTIPUS e1,ELEMGYUJTTIPUS e2,SZAMOSSAGTIPUS db)
{
	if (db < 0) kitoltPalya(palya,n,m,e1,e2);
	else
	{
		SZAMOSSAGTIPUS k=0;
		unsigned int l=0;
		unsigned int N2=16;
		if (n*m<=palya.size())
		while ((k<db)&&(l<N2))
		{
			PALYAMERETTIPUS i=veletlenErtek<PALYAMERETTIPUS>(0,n-1);
			PALYAMERETTIPUS j=veletlenErtek<PALYAMERETTIPUS>(0,m-1);
			IGAZSAGTIPUS b=palya[PALYALINEARIS(i,j,n,m)]==URES;
			if (e2.size()>1)
			{
				b=KERESPALYAELEM(e2,palya,i,j,n,m);
			}
			else
			{
				if (e2.size()==1) b=palya[PALYALINEARIS(i,j,n,m)]==e2[0];
			}
			if (b)
			{
				PALYAELEMTIPUS e=LEVEGO;
				if (e1.size()>1) e=e1[veletlenErtek<PALYAELEMTIPUS>(0,e1.size()-1)];
				else
				{
					if (e1.size()==1) e=e1[0];
				}
				palya[PALYALINEARIS(i,j,n,m)]=e;
				k++;
				l=0;
			}
			else
			{
				l++;
			}
		}
	}
}

void kitoltPalya(PALYATIPUS & palya,PALYAMERETTIPUS n,PALYAMERETTIPUS m,ELEMGYUJTTIPUS e1,ELEMGYUJTTIPUS e2)
{
	if (n*m<=palya.size())
	for (PALYAMERETTIPUS i=0;i<n;i++)
	{
		for (PALYAMERETTIPUS j=0;j<m;j++)
		{
			IGAZSAGTIPUS b=palya[PALYALINEARIS(i,j,n,m)]==URES;
			if (e2.size()>1)
			{
				b=KERESPALYAELEM(e2,palya,i,j,n,m);
			}
			else
			{
				if (e2.size()==1) b=palya[PALYALINEARIS(i,j,n,m)]==e2[0];
			}
			if (b)
			{
				PALYAELEMTIPUS e=LEVEGO;
				if (e1.size()>1) e=e1[veletlenErtek<PALYAELEMTIPUS>(0,e1.size()-1)];
				else
				{
					if (e1.size()==1) e=e1[0];
				}
				palya[PALYALINEARIS(i,j,n,m)]=e;
			}
		}
	}
}

SZAMOSSAGTIPUS palyaelemDarabSzam(PALYATIPUS & palya,PALYAMERETTIPUS n,PALYAMERETTIPUS m,PALYAELEMTIPUS e)
{
	SZAMOSSAGTIPUS res=0;
	if (n*m<=palya.size())
	for (PALYAMERETTIPUS i=0;i<n;i++)
	{
		for (PALYAMERETTIPUS j=0;j<m;j++)
		{
			if (palya[PALYALINEARIS(i,j,n,m)]==e) res++;
		}
	}
	return res;
}

void kiirPalya(PALYATIPUS & palya,PALYAMERETTIPUS n,PALYAMERETTIPUS m,TEXTURAKESZLETTIPUS texturak,ostream & os,CIMKETIPUS ls="")
{
	stringstream ss("");
	if (texturak.size()>=ELEMFAJTASZAM)
	if (n*m<=palya.size())
	{
		if (ls!="") ss << ls << ":";
		for (PALYAMERETTIPUS i=0;i<n;i++)
		{
			for (PALYAMERETTIPUS j=0;j<m;j++)
			{
				ss << texturak[palya[PALYALINEARIS(i,j,n,m)]];
				if (j==m-1) ss << endl;
			}
		}
		os << ss.str();
	}
}

void kiirMaszk(MASZKTIPUS & maszk,PALYAMERETTIPUS n,PALYAMERETTIPUS m,TEXTURAKESZLETTIPUS texturak,ostream & os,CIMKETIPUS ls="")
{
	stringstream ss("");
	if (texturak.size()>2)
	if (n*m<=maszk.size())
	{
		if (ls!="") ss << ls << ":";
		for (PALYAMERETTIPUS i=0;i<n;i++)
		{
			for (PALYAMERETTIPUS j=0;j<m;j++)
			{
				ss << (maszk[PALYALINEARIS(i,j,n,m)] ? texturak[1] : texturak[0]);
				if (j==m-1) ss << endl;
			}
		}
		os << ss.str();
	}
}

void iranySzamitas(IRANYTIPUS irany,int & idiff,int & jdiff,unsigned int n=1)
{
	switch (irany)
	{
		case JOBBRA : { idiff=0; jdiff=n; } break;
		case FEL : { idiff=-n; jdiff=0; } break;
		case BALRA : { idiff=0; jdiff=-n; } break;
		case LE : { idiff=n; jdiff=0; } break;
		default: { idiff=0; jdiff=0; } break;
	}
}

IGAZSAGTIPUS mozgatGravIranyba(PALYATIPUS & palya,ELEMGYUJTVEKTORTIPUS ev,PALYAMERETTIPUS n,PALYAMERETTIPUS m,PALYAMERETTIPUS i,PALYAMERETTIPUS j,IRANYTIPUS irany,MASZKVEKTORTIPUS & maszkok,IGAZSAGTIPUS masikrobban)
{
	int idiff=0;
	int jdiff=0;
	IGAZSAGTIPUS res=false;
	MERETHIBA(n,m,palya);MERETHIBA(n,m,maszkok[MOZGATAS]);MERETHIBA(n,m,maszkok[ROBBANAS]);MERETHIBA(n,m,maszkok[UTKOZES]);MERETHIBA(n,m,maszkok[GURULAS]);
	iranySzamitas(irany,idiff,jdiff);
	if (maszkok[UTKOZES][PALYALINEARIS(i,j,n,m)]&&(KERESPALYAELEM(ev[ELEMROBBANAS],palya,i+idiff,j+jdiff,n,m)))
	{
		if (masikrobban) maszkok[ROBBANAS][PALYALINEARIS(i+idiff,j+jdiff,n,m)]=true;
		else maszkok[ROBBANAS][PALYALINEARIS(i,j,n,m)]=true;
		maszkok[UTKOZES][PALYALINEARIS(i,j,n,m)]=false;
	}
	if (KERESPALYAELEM(ev[ELEMETELTUNTET],palya,i+idiff,j+jdiff,n,m)) palya[PALYALINEARIS(i+idiff,j+jdiff,n,m)]=URES;
	if ((i+idiff>=0)&&(i+idiff<n))
	if ((j+jdiff>=0)&&(j+jdiff<m))
	if (palya[PALYALINEARIS(i+idiff,j+jdiff,n,m)]==URES)
		{
			LEPTET(palya,i,j,idiff,jdiff,n,m);
			maszkok[MOZGATAS][PALYALINEARIS(i+idiff,j+jdiff,n,m)]=false;
			int idiff2,jdiff2;
			iranySzamitas(irany,idiff2,jdiff2,2);
			if ((i+idiff2>=0)&&(i+idiff2<n))
			if ((j+jdiff2>=0)&&(j+jdiff2<m))
			if (KERESPALYAELEM(ev[ELEMENGURUL],palya,i+idiff2,j+jdiff2,n,m)) maszkok[GURULAS][PALYALINEARIS(i+idiff,j+jdiff,n,m)]=true;
			if (palya[PALYALINEARIS(i+idiff2,j+jdiff2,n,m)]!=URES) maszkok[UTKOZES][PALYALINEARIS(i+idiff,j+jdiff,n,m)]=true;
			res=true;
		}
	return res;
}

IGAZSAGTIPUS mozgatIranyba(PALYATIPUS & palya,ELEMGYUJTVEKTORTIPUS ev,PALYAMERETTIPUS n,PALYAMERETTIPUS m,PALYAMERETTIPUS i,PALYAMERETTIPUS j,IRANYTIPUS irany,MASZKVEKTORTIPUS & maszkok,int ROBBANINDEX)
{
	int idiff=0;
	int jdiff=0;
	IGAZSAGTIPUS res=false;
	MERETHIBA(n,m,palya);MERETHIBA(n,m,maszkok[MOZGATAS]);if (ROBBANINDEX>=0) MERETHIBA(n,m,maszkok[ROBBANINDEX]);MERETHIBA(n,m,maszkok[GURULAS]);
	iranySzamitas(irany,idiff,jdiff);
	if (KERESPALYAELEM(ev[ELEMROBBANAS],palya,i+idiff,j+jdiff,n,m)) if (ROBBANINDEX>=0) maszkok[ROBBANINDEX][PALYALINEARIS(i+idiff,j+jdiff,n,m)]=true;
	if (KERESPALYAELEM(ev[ELEMETELTUNTET],palya,i+idiff,j+jdiff,n,m)) palya[PALYALINEARIS(i+idiff,j+jdiff,n,m)]=URES;
	if ((i+idiff>=0)&&(i+idiff<n))
	if ((j+jdiff>=0)&&(j+jdiff<m))
	if (palya[PALYALINEARIS(i+idiff,j+jdiff,n,m)]==URES)
		{
			maszkok[GURULAS][PALYALINEARIS(i,j,n,m)]=false;
			LEPTET(palya,i,j,idiff,jdiff,n,m);
			maszkok[MOZGATAS][PALYALINEARIS(i+idiff,j+jdiff,n,m)]=false;
			res=true;
		}
	return res;
}

SZAMOSSAGTIPUS maszkSzamossag(MASZKTIPUS & maszk,PALYAMERETTIPUS n,PALYAMERETTIPUS m)
{
	SZAMOSSAGTIPUS res=0;
	if (n*m<=maszk.size())
	for (PALYAMERETTIPUS i=0;i<n;i++)
	{
		for (PALYAMERETTIPUS j=0;j<m;j++)
		{
			res+=(maszk[PALYALINEARIS(i,j,n,m)]?1:0);
		}
	}
	return res;
}

IGAZSAGTIPUS mozgatElem(PALYATIPUS & palya,PALYAMERETTIPUS n,PALYAMERETTIPUS m,PALYAELEMTIPUS e,ELEMGYUJTVEKTORTIPUS ev,vector<IRANYTIPUS> iranyok,MASZKVEKTORTIPUS & maszkok,int ROBBANINDEX)
{
	MERETHIBA(n,m,palya);MERETHIBA(n,m,maszkok[MOZGATAS]);if (ROBBANINDEX>=0) MERETHIBA(n,m,maszkok[ROBBANINDEX]);
	for (PALYAMERETTIPUS i=0;i<n*m;i++) maszkok[MOZGATAS][i]=true;
	for (PALYAMERETTIPUS i=0;i<n;i++)
	{
		for (PALYAMERETTIPUS j=0;j<m;j++)
		{
			IGAZSAGTIPUS b=maszkok[MOZGATAS][PALYALINEARIS(i,j,n,m)]&&palya[PALYALINEARIS(i,j,n,m)]==e;
			if (ROBBANINDEX>=0) b=b&&(!maszkok[ROBBANINDEX][PALYALINEARIS(i,j,n,m)]);
			if (b)
			{
				IGAZSAGTIPUS b=true;
				unsigned int k=0;
				unsigned int N=16;
				do {
					IRANYTIPUS dt=LE;
					if (iranyok.size()>1) dt=iranyok[veletlenErtek<IRANYTIPUS>(0,iranyok.size()-1)];
					else
					{
						if (iranyok.size()==1)
							dt=iranyok[0];
					}
					b=!mozgatIranyba(palya,ev,n,m,i,j,dt,maszkok,ROBBANINDEX);
					k+=(b?1:0);
				} while (b&&k<N);
			}
		}
	}
	return true;
}

PALYAELEMTIPUS ranezIranyba(PALYATIPUS & palya,PALYAMERETTIPUS n,PALYAMERETTIPUS m,PALYAMERETTIPUS i,PALYAMERETTIPUS j,vector<IRANYTIPUS> iranyok)
{
	PALYAMERETTIPUS i2=i;
	PALYAMERETTIPUS j2=j;
	for (IRANYTIPUS i : iranyok)
	{
		int idiff,jdiff;
		iranySzamitas(i,idiff,jdiff);
		i2+=idiff,j2+=jdiff;
	}
	return palya[PALYALINEARIS(i2,j2,n,m)];
}

IGAZSAGTIPUS mozgatGravElem(PALYATIPUS & palya,PALYAMERETTIPUS n,PALYAMERETTIPUS m,PALYAELEMTIPUS e,ELEMGYUJTVEKTORTIPUS ev,IRANYTIPUS gravirany,vector<IRANYTIPUS> altirany,MASZKVEKTORTIPUS & maszkok,IGAZSAGTIPUS masikrobban)
{
	MERETHIBA(n,m,palya);MERETHIBA(n,m,maszkok[MOZGATAS]);MERETHIBA(n,m,maszkok[ROBBANAS]);
	for (PALYAMERETTIPUS i=0;i<n*m;i++) maszkok[MOZGATAS][i]=true;
	for (PALYAMERETTIPUS i=0;i<n;i++)
	{
		for (PALYAMERETTIPUS j=0;j<m;j++)
		{
			if (!maszkok[ROBBANAS][PALYALINEARIS(i,j,n,m)]&&maszkok[MOZGATAS][PALYALINEARIS(i,j,n,m)]&&palya[PALYALINEARIS(i,j,n,m)]==e)
			{
				IGAZSAGTIPUS van_e_gurulas=maszkok[GURULAS][PALYALINEARIS(i,j,n,m)];
				if (!mozgatGravIranyba(palya,ev,n,m,i,j,gravirany,maszkok,masikrobban))
				{
					if (van_e_gurulas)
					for (IRANYTIPUS k : altirany)
					{
							if (ranezIranyba(palya,n,m,i,j,vector<IRANYTIPUS>({gravirany,k}))==URES)
							if (mozgatIranyba(palya,ev,n,m,i,j,k,maszkok,-1)) break;
					}
				}
			}
		}
	}
	return true;
}

IGAZSAGTIPUS feldolgozRobbantas(PALYATIPUS & palya,MASZKVEKTORTIPUS & maszkok,vector<SZAMOSSAGTIPUS> maszkindexek,PALYAMERETTIPUS n,PALYAMERETTIPUS m,ELEMGYUJTTIPUS e_N,ELEMGYUJTTIPUS mire_N,mutex & io_mutex,ostream & logstr)
{
	SZAMOSSAGTIPUS i2=0;
	SZAMOSSAGTIPUS n2=maszkindexek.size();
	MERETHIBA(n,m,palya);for (SZAMOSSAGTIPUS i : maszkindexek) MERETHIBA(n,m,maszkok[i]);
	MASZKVEKTOR_SZAMOSSAGHIBA(maszkok,n,m);
	VEKTOR_MERETEGYEZES_HIBA(maszkindexek,e_N);
	VEKTOR_MERETEGYEZES_HIBA(e_N,mire_N);
	VEKTOR_MERETEGYEZES_HIBA(maszkindexek,mire_N);
	MASZKVEKTORTIPUS eltuntetes=URES_PALYAMASZK_VEKTOR(n2,n,m);
	MASZKVEKTORTIPUS maszkok2=URES_PALYAMASZK_VEKTOR(n2,n,m);
	i2=0;
	for (auto m : maszkok)
	{
		copy(maszkok2[i2].begin(),maszkok2[i2].end(),m.begin());
		i2++;
	}
	for (SZAMOSSAGTIPUS k=0;k<n2;k++)
	{
		/*io_mutex.lock();
		logstr << k+1 << ". OK!" << endl;
		io_mutex.unlock();*/
		SZAMOSSAGTIPUS k2=maszkindexek[k];
		for (PALYAMERETTIPUS i=0;i<n;i++)
		{
			for (PALYAMERETTIPUS j=0;j<m;j++)
			{
				if (maszkok[k][PALYALINEARIS(i,j,n,m)])
				{
					eltuntetes[k2][PALYALINEARIS(i,j,n,m)]=true;
					for (IRANYTIPUS k=0;k<IRANYSZAM;k++)
					{
						int idiff=0;
						int jdiff=0;
						iranySzamitas(k,idiff,jdiff);
						if ((i+idiff>=0)&&(i+idiff<n))
						if ((j+jdiff>=0)&&(j+jdiff<m))
						{
							IGAZSAGTIPUS b=true;
							b&=(palya[PALYALINEARIS(i+idiff,j+jdiff,n,m)]!=FAL);
							for (PALYAELEMTIPUS e : e_N)
								b&=(palya[PALYALINEARIS(i+idiff,j+jdiff,n,m)]!=e);
							if (b)
								eltuntetes[k2][PALYALINEARIS(i+idiff,j+jdiff,n,m)]=true;
							SZAMOSSAGTIPUS i=0;
							for (PALYAELEMTIPUS e : e_N)
							{
								if (palya[PALYALINEARIS(i+idiff,j+jdiff,n,m)]==e)
								{
									maszkok2[k2][PALYALINEARIS(i+idiff,j+jdiff,n,m)]=true;
									break;
								}
								i++;
							}
						}
					}
					maszkok[k2][PALYALINEARIS(i,j,n,m)]=false;
					break;
				}
			}
		}
		io_mutex.lock();
		kiirMaszk(eltuntetes[k2],n,m,MASZKMEGJELENES,logstr,"1. maszk:");
		kiirMaszk(maszkok2[k2],n,m,MASZKMEGJELENES,logstr,"2. maszk:");
		io_mutex.unlock();
	}
	for (PALYAMERETTIPUS i=0;i<n;i++)
	{
		for (PALYAMERETTIPUS j=0;j<m;j++)
		{
			for (SZAMOSSAGTIPUS k=0;k<n2;k++)
			{
				SZAMOSSAGTIPUS k2=maszkindexek[k];
				PALYAELEMTIPUS mire=mire_N[k2];
				if (eltuntetes[k][PALYALINEARIS(i,j,n,m)])
					palya[PALYALINEARIS(i,j,n,m)]=mire;
			}
		}
	}
	i2=0;
	for (auto m : maszkok2)
	{
		copy(maszkok[i2].begin(),maszkok[i2].end(),m.begin());
		i2++;
	}
	return true;
}

void mozgatszal(PALYATIPUS & palya,PALYAMERETTIPUS n,PALYAMERETTIPUS m,MASZKVEKTORTIPUS & maszkok,IGAZSAGTIPUS & kiir_e,IGAZSAGTIPUS & loop,mutex & io_mutex,ostream & logstr)
{
	using namespace chrono_literals;
	IDOMERTEKTIPUS ido=0;
	auto kesleltetes=1s;//chrono::milliseconds(20);
	while (loop)
	{
		IGAZSAGTIPUS b=true;

		/*b&=feldolgozRobbantas(palya,maszkok,vector<SZAMOSSAGTIPUS>({ROBBANAS,CSILLAGROBBANAS}),n,m,
			ELEMGYUJTTIPUS({BOMBA,CSILLAGOK}),ELEMGYUJTTIPUS({URES,DRAGAKO}),io_mutex,logstr);*/

		if (b)
		{
			kiir_e=true;
			this_thread::sleep_for(kesleltetes);
			ido++;
		}

		b=true;
		if (ido%2==0)
			b&=mozgatElem(palya,n,m,ELLENFEL,ELLENFEL_ELEMVEKTOROK,MINDENIRANY,maszkok,ROBBANAS);
		if (ido%5==0)
			b&=mozgatElem(palya,n,m,CSILLAGOK,CSILLAG_ELEMVEKTOROK,MINDENIRANY,maszkok,CSILLAGROBBANAS);

		b&=mozgatGravElem(palya,n,m,KO,KO_ELEMVEKTOROK,LE,GURULASIRANY,maszkok,true);
		b&=mozgatGravElem(palya,n,m,BOMBA,BOMBA_ELEMVEKTOROK,LE,GURULASIRANY,maszkok,false);

		if (b)
		{
			kiir_e=true;
			this_thread::sleep_for(kesleltetes);
			ido++;
		}
	}
}

void kiirszal(PALYATIPUS & palya,PALYAMERETTIPUS n,PALYAMERETTIPUS m,IGAZSAGTIPUS & kiir_e,IGAZSAGTIPUS & loop,mutex & io_mutex,ostream & os)
{
	while (loop)
	{
		if (kiir_e)
		{
			io_mutex.lock();
			//system("clear");
			kiirPalya(palya,n,m,PELDAMEGJELENES,os);
			if (palyaelemDarabSzam(palya,n,m,EMBER)==0)
				os << "Vesztettél!" << endl;
			io_mutex.unlock();
			kiir_e=false;
		}
		this_thread::sleep_for(1s);
	}
}

template <class R> SZAMOSSAGTIPUS randGenInd(vector<R> valsegek)
{
	#define ELOSZLASTIPUS piecewise_constant_distribution<R>
	#define GENERATORTIPUS mt19937
	mt19937 gen;
	ELOSZLASTIPUS dis;
	std::vector<R> intval;
	for (SZAMOSSAGTIPUS i=0;i<valsegek.size();i++)
		intval.push_back((R)i);
	intval.push_back(valsegek.size());
	gen=GENERATORTIPUS(chrono::system_clock::now().time_since_epoch().count());
	dis=ELOSZLASTIPUS(intval.begin(),intval.end(),valsegek.begin());
	SZAMOSSAGTIPUS intval_elem=floor(dis(gen));
	return intval_elem;
}

void billentyuszal(PALYATIPUS & palya,PALYAMERETTIPUS n,PALYAMERETTIPUS m,MASZKVEKTORTIPUS & maszkok, IGAZSAGTIPUS & kiir_e, IGAZSAGTIPUS & loop,mutex & io_mutex,ostream & logstr)
{
	#define FEL_KOD 'W'
	#define LE_KOD 'S'
	#define BALRA_KOD 'A'
	#define JOBBRA_KOD 'D'
	#define SEMMI_KOD '\0'
	#define KILEP_KOD 'X'
	vector<BILLIRANYITTIPUS> billentyuk({FEL_KOD,LE_KOD,BALRA_KOD,JOBBRA_KOD,SEMMI_KOD,KILEP_KOD});
	vector<TARTOMANYTIPUS> valsegek({10,10,10,10,20,1});
	
	while (loop)
	{
		//SZAMOSSAGTIPUS i=randGenInd<TARTOMANYTIPUS>(valsegek);
		BILLIRANYITTIPUS c=0;//c=billentyuk[i];
		io_mutex.lock();
		c = getch();
		cout << "\"" << c << "\"" << endl;
		io_mutex.unlock();
		switch (toupper(c))
		{
			case FEL_KOD : { EMBERMOZOG(FEL); kiir_e=true; break; }
			case LE_KOD : { EMBERMOZOG(LE); kiir_e=true; break; }
			case BALRA_KOD : { EMBERMOZOG(BALRA); kiir_e=true; break; }
			case JOBBRA_KOD : { EMBERMOZOG(JOBBRA); kiir_e=true; break; }
			case KILEP_KOD : { loop=false; break; }
			default: break;
		}
		this_thread::sleep_for(1s);
	}
}

int main(int argc,char** argv)
{
	PALYAMERETTIPUS n,m;
	PALYATIPUS palya;
	if (argc>1)
	{
		string fn(argv[1]);
		std::ifstream infile;
		infile.open(fn.c_str(),std::fstream::in);
		if (infile.is_open()) beolvasPalya(palya,n,m,infile,PELDAMEGJELENES);
		infile.close();
	}
	else
	{
		PALYAMERETTIPUS fm;
		SZAMOSSAGTIPUS fsz,dk,el,ksz,bsz,cssz,lev;
		cin>>n>>m>>fm>>fsz>>dk>>el>>ksz>>bsz>>cssz>>lev;
		feltoltPalya(palya,n,m,URESELEM);
		bekeretezPalya(palya,n,m,ELEMGYUJTTIPUS({FAL}),fm);
		szetszorPalyan(palya,n,m,ELEMGYUJTTIPUS({FAL,KONNYUFAL}),URESELEM,fsz);
		szetszorPalyan(palya,n,m,ELEMGYUJTTIPUS({DRAGAKO}),URESELEM,dk);
		szetszorPalyan(palya,n,m,ELEMGYUJTTIPUS({EMBER}),URESELEM,1);
		szetszorPalyan(palya,n,m,ELEMGYUJTTIPUS({KIJARAT}),URESELEM,1);
		szetszorPalyan(palya,n,m,ELEMGYUJTTIPUS({ELLENFEL}),URESELEM,el);
		szetszorPalyan(palya,n,m,ELEMGYUJTTIPUS({KO}),URESELEM,ksz);
		szetszorPalyan(palya,n,m,ELEMGYUJTTIPUS({BOMBA}),URESELEM,bsz);
		szetszorPalyan(palya,n,m,ELEMGYUJTTIPUS({CSILLAGOK}),URESELEM,cssz);
		szetszorPalyan(palya,n,m,ELEMGYUJTTIPUS({LEVEGO}),URESELEM,lev);
	}
	MASZKVEKTORTIPUS maszkok(MASZKOKSZAMA,URES_PALYAMASZK(n,m));
	maszkok[MOZGATAS]=TELJES_PALYAMASZK(n,m);
	IGAZSAGTIPUS kiir_e=false,loop=true;
	if (!system(NULL)) exit(EXIT_FAILURE);
	mutex io_mutex;
	thread t1(mozgatszal,ref(palya),n,m,ref(maszkok),ref(kiir_e),ref(loop),ref(io_mutex),ref(clog));
	thread t2(kiirszal,ref(palya),n,m,ref(kiir_e),ref(loop),ref(io_mutex),ref(cout));
	thread t3(billentyuszal,ref(palya),n,m,ref(maszkok),ref(kiir_e),ref(loop),ref(io_mutex),ref(clog));
	t1.join();
	t2.join();
	t3.join();
	return EXIT_SUCCESS;
}
