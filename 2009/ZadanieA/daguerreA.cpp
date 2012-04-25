/*
	Algorytmy i struktury danych - zadanie A - "SKARB"
	Deadline: 18 Marca 2008
	Autor: Krzysztof Andrzej Parjaszewski (daguerre)
*/
#include <iostream>
#include <list>
using namespace std;
/*
Definiujê prost¹ strukturê dla zapamiêtania wspó³rzêdnych obiektu
*/
#define SLOWO_NIE "NIE"
#define MAX_WR 2000
#define MAX_KOL 2000

typedef enum 
{
	GORA,
	DOL,
	LEWO,
	PRAWO,
	BRAK
} enum_kierunki;

struct wspolrzedne
{
	int nr_wiersza;
	int nr_kolumny;
	enum_kierunki ojciec;
} skarb,poszukiwacz;



int ilosc_kolumn = 0;
int ilosc_wierszy = 0;
const char KROPKA = '.';
const char POSZUKIWACZ = 'P';
const char SKARB = 'S';
const char ODWIEDZONE_POLE = ',';
const int KONIEC_SZUKANIA = -1;
char tablicaWK[MAX_KOL][MAX_WR];

int sprawdz_czy_mozna_isc(enum_kierunki,wspolrzedne );
int sprawdz(char);
void idz(enum_kierunki, wspolrzedne);
int petla(int);


int minimalna_ilosc_krokow = 0;

std::list<wspolrzedne> lista_wpolrzednych;
//std::list<wspolrzedne> lista_kolejnych_wpolrzednych;



/*
0 - mo¿na
1 - nie mo¿na
2 - to ju¿ skarb!
*/
int sprawdz_czy_mozna_isc(enum_kierunki kierunek,wspolrzedne pozycja)
{
	switch (kierunek){
	case  GORA : 
			if ((pozycja.ojciec == GORA) || (pozycja.nr_wiersza==0)) {
				return 0;
			} else {
				return sprawdz(tablicaWK[pozycja.nr_wiersza - 1][pozycja.nr_kolumny]);
			}
		break;
	case  DOL : 
			if ((pozycja.ojciec == DOL) || (pozycja.nr_wiersza== (ilosc_wierszy - 1))) {
				return 0;
			} else {
				return sprawdz(tablicaWK[pozycja.nr_wiersza + 1][pozycja.nr_kolumny]);
			}
		break;
	case LEWO :
            if ((pozycja.ojciec == LEWO) || (pozycja.nr_kolumny == 0)) {
				return 0;
			} else {
				return sprawdz(tablicaWK[pozycja.nr_wiersza][pozycja.nr_kolumny - 1]);
			}
		break;
	case  PRAWO :
			if ((pozycja.ojciec == PRAWO) || (pozycja.nr_kolumny == (ilosc_kolumn - 1))) {
				return 0;
			} else {
				return sprawdz(tablicaWK[pozycja.nr_wiersza][pozycja.nr_kolumny + 1]);
			}
		break;
	default:
		return 0;
	}
}

/*
0 - nie mo¿na
1 - mo¿na
2 - to ju¿ skarb!
*/
int sprawdz(char co_sprawdzic)
{
	switch(co_sprawdzic) {
		case KROPKA : 
			return 1;
		case SKARB : 
			return 2;
		default :
			return 0;
	}
}

void idz(enum_kierunki kierunek, wspolrzedne  pozycja)
{
	switch (kierunek) {
	case GORA:
			tablicaWK[pozycja.nr_wiersza - 1][pozycja.nr_kolumny] = ODWIEDZONE_POLE;
			break;
	case DOL:
			tablicaWK[pozycja.nr_wiersza + 1][pozycja.nr_kolumny ] = ODWIEDZONE_POLE;
			break;
	case LEWO:
			tablicaWK[pozycja.nr_wiersza][pozycja.nr_kolumny - 1] = ODWIEDZONE_POLE;
			break;
	case PRAWO:
			tablicaWK[pozycja.nr_wiersza][pozycja.nr_kolumny +1] = ODWIEDZONE_POLE;
			break;
	}
}

int petla(int aktualna_maksymalna_ilosc_krokow) 
{
	wspolrzedne nowa_pozycja;
	bool koniec = true;
	int wynik = 0;
	wspolrzedne dodaj_pozycje;
	int counter = lista_wpolrzednych.size();
	do {
		if ( counter-- == 0 )
		{
			return KONIEC_SZUKANIA;
		}
		nowa_pozycja = lista_wpolrzednych.front();

		wynik = sprawdz_czy_mozna_isc(GORA,nowa_pozycja); 
		if (wynik==2) {
			return aktualna_maksymalna_ilosc_krokow;
		} else if (wynik==1) {
			dodaj_pozycje.nr_kolumny = nowa_pozycja.nr_kolumny;
			dodaj_pozycje.nr_wiersza = nowa_pozycja.nr_wiersza - 1;
			dodaj_pozycje.ojciec=DOL;
			lista_wpolrzednych.push_back(dodaj_pozycje);
			idz(GORA, nowa_pozycja);
			koniec = false;
		} 

		wynik = sprawdz_czy_mozna_isc( DOL,nowa_pozycja); 
		if (wynik==2) {
			return aktualna_maksymalna_ilosc_krokow;
		} else if (wynik==1) {
			dodaj_pozycje.nr_kolumny = nowa_pozycja.nr_kolumny;
			dodaj_pozycje.nr_wiersza = nowa_pozycja.nr_wiersza + 1;
			dodaj_pozycje.ojciec=GORA;
			lista_wpolrzednych.push_back(dodaj_pozycje);
			idz(DOL, nowa_pozycja);
			koniec = false;
		} 

		wynik = sprawdz_czy_mozna_isc(LEWO,nowa_pozycja); 
		if (wynik==2) {
			return aktualna_maksymalna_ilosc_krokow;
		} else if (wynik==1) {
			dodaj_pozycje.nr_kolumny = nowa_pozycja.nr_kolumny - 1;
			dodaj_pozycje.nr_wiersza = nowa_pozycja.nr_wiersza;
			dodaj_pozycje.ojciec=PRAWO;
			lista_wpolrzednych.push_back(dodaj_pozycje);
			idz(LEWO, nowa_pozycja);
			koniec = false;
		} 

		wynik = sprawdz_czy_mozna_isc(PRAWO,nowa_pozycja); 
		if (wynik==2) {
			return aktualna_maksymalna_ilosc_krokow;
		} else if (wynik==1) {
			dodaj_pozycje.nr_kolumny = nowa_pozycja.nr_kolumny + 1;
			dodaj_pozycje.nr_wiersza = nowa_pozycja.nr_wiersza;
			dodaj_pozycje.ojciec=LEWO;
			lista_wpolrzednych.push_back(dodaj_pozycje);
			idz(PRAWO, nowa_pozycja);
			koniec = false;
		} 
		// and remove the unnecessary element
		lista_wpolrzednych.pop_front();
	} 
	while(koniec || (counter > 0));

	if (koniec) 
	{
		return -1;
	}
	return 0;
}


int main ()
{
	scanf("%d %d",&ilosc_kolumn,&ilosc_wierszy);
	for(int licznikW = 0; licznikW <ilosc_wierszy; licznikW++) {
		scanf("%s",&tablicaWK[licznikW]);
		for(int licznikK = 0; licznikK < ilosc_kolumn; licznikK++) {
			if (tablicaWK[licznikW][licznikK] == POSZUKIWACZ) {
				poszukiwacz.nr_wiersza = licznikW;
				poszukiwacz.nr_kolumny = licznikK;
				poszukiwacz.ojciec = BRAK;
			} else if (tablicaWK[licznikW][licznikK] == SKARB) {
				skarb.nr_wiersza = licznikW;
				skarb.nr_kolumny = licznikK;
			}			
		}
	}

	/*KONIEC POBIERANIA DANYCH*/
	lista_wpolrzednych.push_front(poszukiwacz);
	
	minimalna_ilosc_krokow = 0;
	int kod_wyjscia = 0;
	do
	{
		minimalna_ilosc_krokow++; 
		kod_wyjscia = petla(minimalna_ilosc_krokow);
		if (kod_wyjscia == KONIEC_SZUKANIA) {
			printf("%s",SLOWO_NIE);
			return 0;
		}
	} while(kod_wyjscia == 0);
	printf("%d", kod_wyjscia);
	return 0;
}
