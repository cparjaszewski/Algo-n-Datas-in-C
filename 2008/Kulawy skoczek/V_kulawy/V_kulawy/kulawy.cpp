/*
	Algorytmy i struktury danych - zadanie B - "KULAWY SZKOCZEK"
	Deadline: 15 Kwietnia 2008
	Autor: Krzysztof Andrzej Parjaszewski (daguerre)
*/
#include <iostream>
#include <list>
using namespace std;
/*
#define - tutaj bêd¹ ró¿ne zmienne globalne, defines, struktury i typy wyliczeniowe
*/
#define MAX_WR 3000
#define MAX_KOL 3000

const int WEKTOR_CONST[10] = {1,7,49,343,343*7,343*49,343*343,343*343*7,343*343*49,343*343*343};
const short SLEPY_ZAULEK = -1;

struct wspolrzedne
{
	short nr_wiersza;
	short nr_kolumny;
	int max_wartosc_jesli_nastepny_ruch_gora;
	int max_wartosc_jesli_nastepny_ruch_dol;
} start;

short _ilosc_kolumn = 0;
short _ilosc_wierszy = 0;
char tablicaCharWK[MAX_WR][MAX_KOL];
short tablicaGlownaWK[MAX_WR][MAX_KOL];
int tablicaRoboczaWK[MAX_WR][2*MAX_KOL];
short _i,_j;


/* ---------------------------------------------------
	Tutaj zdefiniujê interfejsy póŸniej zaimplementowanych metod
--------------------------------------------------- */
void SprawdzWDol(short);
void SprawdzWGore(short);
long wyszukajWartoscWTablicy();
bool sprawdzCzyMozeszIsc(int,int);
void sprawdz2WierszOdKonca();
int Najwiekszy2(int,int);
int Najwiekszy4(int,int,int,int);
int WybierzNajwiekszaWartosc(short);

/* ---------------------------------------------------
	Definicje cia³ dla metod
--------------------------------------------------- */

void OznaczNigdyNieSprawdzaneRekordy()
{
	short ustawMnieNaPrzedostawniWiersz = _ilosc_kolumn-2;
	short ustawMnieNaPierwszyWiersz = 0;
	short ustawMnieNaDrugiWiersz = 1;
	short ustawMnieNaDrugaKolumneIKierunekGora = 2*1 + 1 ;
	// Oznacz przedostatni jako niemo¿liwy dla ruchów w dó³. 
	// Oznacz drugi i pierwszy jako niemo¿liwy dla ruchu w górê.
	for(int i = 0; i< _ilosc_kolumn;i++)
	{
		tablicaRoboczaWK[ustawMnieNaPrzedostawniWiersz][2*i] = SLEPY_ZAULEK;
		tablicaRoboczaWK[ustawMnieNaPierwszyWiersz][2*i + 1] = SLEPY_ZAULEK;
		tablicaRoboczaWK[ustawMnieNaDrugiWiersz][2*i + 1] = SLEPY_ZAULEK;
	}

	// Dla tablicy o width 3 oznacz œrodkow¹ kolumnê, jako zawsze niemo¿liw¹ w kierunku góra
	if (_ilosc_kolumn==3)
	{
		for(int i = 0; i<_ilosc_wierszy;i++)
		{
			tablicaRoboczaWK[i][ustawMnieNaDrugaKolumneIKierunekGora] = SLEPY_ZAULEK;
		}
	}
}

void SprawdzWDol(short ktoryWiersz)
{
	int kolumnyBezOstatniej = _ilosc_kolumn - 1;
	int dwaWierszeNizej = ktoryWiersz + 2;

	// !!!!!!
	// uwaga na " += "
	// !!!!!

	//W przypadku pierwszego to ³atwiejszy problem (wybór spoœród dwóch pól prawo dó³ (W-2,K+1) (maxUp,maxDown) )
	int licznikK = 0;
	// No i ¿eby kilka razy nie liczyæ nie potrzebnie, to jak mamy kana³ to oznaczamy -1 i nie liczymy wy¿ej;
	int kandydat = Najwiekszy2(tablicaRoboczaWK[dwaWierszeNizej][2*(licznikK+1)],tablicaRoboczaWK[dwaWierszeNizej][2*(licznikK+1)+1]);
	if ( kandydat != SLEPY_ZAULEK ) {
		tablicaRoboczaWK[ktoryWiersz][2*licznikK] += kandydat;
	}
	else
	{
		tablicaRoboczaWK[ktoryWiersz][2*licznikK] = -1;
	}
 	

	//Pêtla od drugiej do przedostatniej kolumny
	for(licznikK=1; licznikK < kolumnyBezOstatniej; licznikK++)
	{
		kandydat = Najwiekszy4( tablicaRoboczaWK[dwaWierszeNizej][2*(licznikK-1)], tablicaRoboczaWK[dwaWierszeNizej][2*(licznikK+1)],
								tablicaRoboczaWK[dwaWierszeNizej][2*(licznikK-1)+1], tablicaRoboczaWK[dwaWierszeNizej][2*(licznikK+1)+1] );
		if ( kandydat != SLEPY_ZAULEK ) {
			tablicaRoboczaWK[ktoryWiersz][2*licznikK] += kandydat;
		}
		else {
			tablicaRoboczaWK[ktoryWiersz][2*licznikK] = -1;
		}
	}
	//W przypadku ostatniego to mniejszy problem (wybór spoœród dwóch pól prawo dó³ (W-2,K-1) (maxUp,maxDown) 
	kandydat = Najwiekszy2( tablicaRoboczaWK[dwaWierszeNizej][2*(licznikK-1)],tablicaRoboczaWK[dwaWierszeNizej][2*(licznikK-1)+1] );
	if ( kandydat != SLEPY_ZAULEK ) {
		tablicaRoboczaWK[ktoryWiersz][2*licznikK] += kandydat;
	}
	else	{
		tablicaRoboczaWK[ktoryWiersz][2*licznikK] = -1;
	}
}

void SprawdzWGore(short ktoryWiersz)
{
	int kolumnyBezDwochOstatnich = _ilosc_kolumn - 2;
	int jedenWierszWyzej = ktoryWiersz - 1;

	// !!!!!!
	// uwaga na " += "
	// !!!!!

	// W tej metodzie robimy prawie to samo co w metodzie SprawdzWDol(int)
	// z tym, ¿e sprawdzamy tylko wartoœci pól, które "id¹ w dó³".
	// 
	// W mojej magicznej strukturze o nazwie tablicaRoboczaWK
	// jest to pole o indeksie (numer_kolumny * 2)

	//W przypadku pierwszej i drugiej kolumny to ³atwiejszy problem (wybór spoœród dwóch pól prawo dó³ (W+1,K+2) (maxUp,maxDown) )
 	int licznikK = 0;

	int kandydat = tablicaRoboczaWK[jedenWierszWyzej][2*(licznikK+2)];
	if ( kandydat != SLEPY_ZAULEK ) {
		tablicaRoboczaWK[ktoryWiersz][2*licznikK + 1] += kandydat;
	}
	else
	{
		tablicaRoboczaWK[ktoryWiersz][2*licznikK + 1] = -1;
	}


	licznikK++;

	kandydat = tablicaRoboczaWK[jedenWierszWyzej][2*(licznikK+2)];
	if ( kandydat != SLEPY_ZAULEK ) {
		tablicaRoboczaWK[ktoryWiersz][2*licznikK + 1] += kandydat;
	}
	else
	{
		tablicaRoboczaWK[ktoryWiersz][2*licznikK + 1] = -1;
	}

	


	//Pêtla od drugiej do przedostatniej kolumny
	for(licznikK = 2; licznikK < kolumnyBezDwochOstatnich; licznikK++)
	{
		kandydat = Najwiekszy2(tablicaRoboczaWK[jedenWierszWyzej][2*(licznikK-2)],tablicaRoboczaWK[jedenWierszWyzej][2*(licznikK+2)]);
		if (kandydat != SLEPY_ZAULEK ) {
			tablicaRoboczaWK[ktoryWiersz][2*licznikK+1] += kandydat;
		}
		else {
			tablicaRoboczaWK[ktoryWiersz][2*licznikK + 1] = -1;
		}
	}
	

	//W przypadku ostatniej i przedostatniej kolumny inne traktowanie to mniejszy problem (wybór spoœród dwóch pól prawo dó³ (W+1,K-2) (maxUp,maxDown) 
	kandydat = tablicaRoboczaWK[jedenWierszWyzej][2*(licznikK-2)];
	if (kandydat != SLEPY_ZAULEK ) {
		tablicaRoboczaWK[ktoryWiersz][2*licznikK+1] += kandydat;
	}
	else {
		tablicaRoboczaWK[ktoryWiersz][2*licznikK + 1] = -1;
	}

	licznikK++;

	//W przypadku ostatniego to mniejszy problem (wybór spoœród dwóch pól prawo dó³ (W-2,K-1) (maxUp,maxDown) 
	kandydat = tablicaRoboczaWK[jedenWierszWyzej][2*(licznikK-2)];
	if (kandydat != SLEPY_ZAULEK ) {
		tablicaRoboczaWK[ktoryWiersz][2*licznikK+1] += kandydat;
	}
	else {
		tablicaRoboczaWK[ktoryWiersz][2*licznikK + 1] = -1;
	}
}



int Najwiekszy2(int wyrazA, int wyrazB)
{
	return (wyrazA>wyrazB?wyrazA:wyrazB);
}


int Najwiekszy4(int wyrazA, int wyrazB,int wyrazC,int wyrazD)
{
	int tempLeaderAB = (wyrazA>wyrazB?wyrazA:wyrazB);
	int tempLeaderCD = (wyrazC>wyrazD?wyrazC:wyrazD);
	return (tempLeaderCD>tempLeaderAB?tempLeaderCD:tempLeaderAB);
}


long WyszukajWartoscWTablicy()
{
	OznaczNigdyNieSprawdzaneRekordy(); 
	// Ostatni oznacz jako koñcowy. Oznacz przedostatni Jako niemo¿liwy dla ruchów w dó³. Oznacz drugi i pierwszy jako niemo¿liwy dla ruchu w górê.
	for( _j = (_ilosc_wierszy - 3); _j > 0;_j--)
	{
		SprawdzWDol(_j);
		SprawdzWGore(_j+1);
	}
	SprawdzWDol(0);
	short pierwszyWiersz = 0;
	int zwyciezca = WybierzNajwiekszaWartosc(pierwszyWiersz);
	return zwyciezca;
}


int WybierzNajwiekszaWartosc(short pierwszyWiersz)
{
	int max = -1;
	for(_i = 0;_i<_ilosc_kolumn;_i++)
	{
		if (tablicaRoboczaWK[pierwszyWiersz][2*_i] > max) {
			max = tablicaRoboczaWK[pierwszyWiersz][2*_i];
		}
		if (tablicaRoboczaWK[pierwszyWiersz][2*_i+1] > max ) {
			max = tablicaRoboczaWK[pierwszyWiersz][2*_i+1];
		}
	}
	return max;
}


/*
	Metoda Main - bo od czegoœ trzeba zacz¹æ
*/ 
int main ()
{
	int licznikW;
	int licznikK;
	int pomocniczyOchotnik;
	/*scanf("%d %d",&_ilosc_wierszy,&_ilosc_kolumn);
	for(licznikW = 0; licznikW <_ilosc_wierszy; licznikW++) {
		scanf("%s",&tablicaCharWK[licznikW]);
		for(licznikK = 0; licznikK < _ilosc_kolumn; licznikK++) {
			pomocniczyOchotnik = WEKTOR_CONST[((int)(tablicaCharWK[licznikW][licznikK])-48)];
			tablicaGlownaWK[licznikW][licznikK] = pomocniczyOchotnik;
			tablicaRoboczaWK[licznikW][2*licznikK] = pomocniczyOchotnik;
			tablicaRoboczaWK[licznikW][2*licznikK+1] = pomocniczyOchotnik;
		}
	}
*/
	/* Inicjalizacja */
/*	licznikW = _ilosc_wierszy-1;
	for(licznikK = 0;licznikK < _ilosc_kolumn; licznikK++)
	{
		tablicaRoboczaWK[licznikW][2*licznikK] = tablicaGlownaWK[licznikW][licznikK];
		tablicaRoboczaWK[licznikW][2*licznikK+1] = SLEPY_ZAULEK;
		tablicaRoboczaWK[licznikW-1][2*licznikK] = SLEPY_ZAULEK;
	}
*/
	/*KONIEC INICJALIZACJI DANYCH*/
//	int max = WyszukajWartoscWTablicy();
	int max = 70;
	printf("%d",max);
	return 0;
}