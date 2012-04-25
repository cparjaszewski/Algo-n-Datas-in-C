#include <iostream>


#define MAX_WR 2200
#define MAX_KOL 300

const int WEKTOR_CONST[10] = {1,7,49,343,343*7,343*49,343*343,343*343*7,343*343*49,343*343*343};

int _ilosc_kolumn = 0;
int _ilosc_wierszy = 0;
int _ilosc_spakowanych_kolumn;
int _dlugosc_ostatniej_kolumny;
int _tablicaWK[MAX_WR][MAX_KOL];

int wyszukajWartoscWTablicy();
int sufitDziel(int,int);
int Najwiekszy2(int,int);
int Najwiekszy4(int,int,int,int);

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

void PobierzWiersz(int nr_wiersza,int (&wiersz)[2*MAX_WR])
{
	int tempWiersz[MAX_KOL];
	int temp;
	int k ;
	for(k= 0;k<_ilosc_spakowanych_kolumn ;k++)
	{
		tempWiersz[k] = _tablicaWK[nr_wiersza][k];
	}
	int z;
	for(k = 0; k < (_ilosc_spakowanych_kolumn - 1); k++)
	{
		temp = tempWiersz[k];		
		for(z = 9; z > -1; z--)
		{
			wiersz[20*k +2*z] = WEKTOR_CONST[temp % 10];
			wiersz[20*k +2*z+1] = WEKTOR_CONST[temp % 10];
			if (temp > 0 )
			{
				temp = temp / 10;
			}
		}
	}
	temp = tempWiersz[k];
	for(z = (_dlugosc_ostatniej_kolumny-1 ); z > -1; z--)
	{
		wiersz[20*k +2*z] = WEKTOR_CONST[temp % 10];
		wiersz[20*k +2*z+1] = WEKTOR_CONST[temp % 10];
		if (temp > 0 )
		{
			temp = temp / 10;
		}
	}
}

void WykonajDodawanieWWierszach(int (&gornyWiersz)[2*MAX_WR], int (&srodkowyWiersz)[2*MAX_WR], int (&ostatniWiersz)[2*MAX_WR])
{
	int i = 0;
	gornyWiersz[2*i] += Najwiekszy2(ostatniWiersz[2*(i+1)],ostatniWiersz[2*(i+1) +1]);
	for(i=1;i<_ilosc_kolumn - 1;i++)
	{
		gornyWiersz[2*i] += Najwiekszy4(ostatniWiersz[2*(i+1)],ostatniWiersz[2*(i+1) +1 ],ostatniWiersz[2*(i-1) ],ostatniWiersz[2*(i-1) +1]);
	}
	gornyWiersz[2*i] += Najwiekszy2(ostatniWiersz[2*(i-1)],ostatniWiersz[2*(i-1)+1]);

	i = 0;
	if (_ilosc_kolumn==3) 
	{
		srodkowyWiersz[2*i + 1] += gornyWiersz[2*(i+2)];
		i++;
		srodkowyWiersz[2*i + 1] = -1;
		i++;
		srodkowyWiersz[2*i + 1] += gornyWiersz[2*(i-2)];
	}
	else
	{	
		i = 0;
		srodkowyWiersz[2*i + 1] += gornyWiersz[2*(i+2)];
		i++;
		srodkowyWiersz[2*i + 1] += gornyWiersz[2*(i+2)];
		for(i=2;i<_ilosc_kolumn - 2;i++)
		{
			srodkowyWiersz[2*i+1] += Najwiekszy2(gornyWiersz[2*(i-2)], gornyWiersz[2*(i+2)]);
		}
		srodkowyWiersz[2*i+1] += gornyWiersz[2*(i-2)];
		i++;
		srodkowyWiersz[2*i+1] += gornyWiersz[2*(i-2)];
	}
}

int WybierzNajwiekszaWartosc(int (&pierwszyWiersz)[2*MAX_WR])
{
	int max = -1;
	int i = 0;
	for(i = 0;i<_ilosc_kolumn;i++)
	{
		if (pierwszyWiersz[2*i] > max) {
			max = pierwszyWiersz[2*i];
		}
		if (pierwszyWiersz[2*i+1] > max ) {
			max = pierwszyWiersz[2*i+1];
		}
	}
	return max;
}

int WyszukajWartoscWTablicy()
{
	int dolnyWiersz[2*MAX_WR];
	PobierzWiersz(_ilosc_wierszy-1,dolnyWiersz);
	int przedostatniWiersz[2*MAX_WR];
	PobierzWiersz(_ilosc_wierszy-2,przedostatniWiersz);
	int przedPrzedOstatniWiersz[2*MAX_WR];
	PobierzWiersz(_ilosc_wierszy-3,przedPrzedOstatniWiersz);
	WykonajDodawanieWWierszach(przedPrzedOstatniWiersz, przedostatniWiersz,dolnyWiersz );
	int temp[2*MAX_WR];	
	int j;
	int i;
	for(j = (_ilosc_wierszy - 4); j > -1;j--)
	{
		PobierzWiersz(j,temp);
		for(i = 0; i< (2*_ilosc_kolumn); i++)
		{
			dolnyWiersz[i] = przedostatniWiersz[i];
			przedostatniWiersz[i] = przedPrzedOstatniWiersz[i];
			przedPrzedOstatniWiersz[i] = temp[i];
		}
		WykonajDodawanieWWierszach(przedPrzedOstatniWiersz,przedostatniWiersz,dolnyWiersz );
	}
	int zwyciezca = WybierzNajwiekszaWartosc(przedPrzedOstatniWiersz);
	return zwyciezca;
}

int sufitDziel(int liczba,int modulo)
{
	return  liczba/modulo + ((liczba % modulo )> 0?1:0);
}

int main ()
{
	short licznikW;
	short licznikK;
	char wiersz[MAX_WR];
	char temp[10];
	scanf("%d %d",&_ilosc_wierszy,&_ilosc_kolumn);
	_ilosc_spakowanych_kolumn = sufitDziel(_ilosc_kolumn,10);
	_dlugosc_ostatniej_kolumny = _ilosc_kolumn - 10*(_ilosc_spakowanych_kolumn-1);

	for(licznikW = 0; licznikW < _ilosc_wierszy; licznikW++) {
		scanf("%s",wiersz);
		for(licznikK = 0; licznikK < _ilosc_spakowanych_kolumn; licznikK++) {
			int i;
			for(i = 0;i<10;i++)	{
				temp[i] = wiersz[10*licznikK+i];
			}
			_tablicaWK[licznikW][licznikK] = atoi(temp);
		}
	}
	printf("%d",WyszukajWartoscWTablicy());
	return 0;
}
