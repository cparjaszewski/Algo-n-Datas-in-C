#include <iostream>

#define MAX_SIZE 1000000

const long long WEKTOR_CONST[10] = {1,7,49,343,343*7,343*49,343*343,343*343*7,343*343*49,343*343*343};

int _ilosc_wezlow = 0;
int _ilosc_wierszy = 0;

long long Najwiekszy2(long long,long long);
long long Najwiekszy4(long long,long long,long long,long long);
long long Dodaj(long long,long long);
long longStala(char);

long long Stala(char znak)
{
    return WEKTOR_CONST[(int)(znak - 48)];
}

long long Najwiekszy2(long long wyrazA, long long wyrazB)
{
	return (wyrazA>wyrazB?wyrazA:wyrazB);
}

long long Najwiekszy4(long long wyrazA, long long wyrazB,long long wyrazC,long long wyrazD)
{
	long long tempLeaderAB = (wyrazA>wyrazB?wyrazA:wyrazB);
	long long tempLeaderCD = (wyrazC>wyrazD?wyrazC:wyrazD);
	return (tempLeaderCD>tempLeaderAB?tempLeaderCD:tempLeaderAB);
}

void PobierzKolejnyWierszZWejscia(long long (&wiersz)[2*MAX_WR]) 
{
	char cyferkiString[2*MAX_WR];
	scanf("%s", cyferkiString);
	int k;
	for(k = 0;k<_ilosc_wezlow;k++)
	{		
		wiersz[2*k]   = Stala(cyferkiString[k]);
		wiersz[2*k+1] = Stala(cyferkiString[k]);
	}	
}

long long Dodaj(long long komu,long long co) 
{
	if ( (co == -1) || (komu == -1) ) {
		return co;
	}
	else {
		return komu+co;
	}
}

void WykonajDodawanieWWierszach(long long (&gornyWiersz)[2*MAX_WR], long long(&srodkowyWiersz)[2*MAX_WR], long long(&dolnyWiersz)[2*MAX_WR])
{
	long long i = 0;
	dolnyWiersz[2*i] = Dodaj(dolnyWiersz[2*i],Najwiekszy2( gornyWiersz[2*(i+1)],gornyWiersz[2*(i+1) +1]));
	for(i=1;i<_ilosc_wezlow - 1;i++) {
		dolnyWiersz[2*i] = Dodaj(dolnyWiersz[2*i], Najwiekszy4(gornyWiersz[2*(i+1)],gornyWiersz[2*(i+1) +1 ],gornyWiersz[2*(i-1) ],gornyWiersz[2*(i-1) +1]));
	}
	dolnyWiersz[2*i] = Dodaj(dolnyWiersz[2*i], Najwiekszy2(gornyWiersz[2*(i-1)],gornyWiersz[2*(i-1)+1]));

	i = 0;
	if (_ilosc_wezlow==3) {
		srodkowyWiersz[2*i + 1] = Dodaj(srodkowyWiersz[2*i+1],dolnyWiersz[2*(i+2)]);
		i++;
		srodkowyWiersz[2*i + 1] = -1;
		i++;
		srodkowyWiersz[2*i + 1] = Dodaj(srodkowyWiersz[2*i+1],dolnyWiersz[2*(i-2)]);
	}
	else {	
		i = 0;
		srodkowyWiersz[2*i + 1] = Dodaj(srodkowyWiersz[2*i+1], dolnyWiersz[2*(i+2)]);
		i++;
		srodkowyWiersz[2*i + 1] = Dodaj(srodkowyWiersz[2*i+1], dolnyWiersz[2*(i+2)]);
		for(i=2;i<_ilosc_wezlow - 2;i++) {
			srodkowyWiersz[2*i+1] = Dodaj(srodkowyWiersz[2*i+1], Najwiekszy2(dolnyWiersz[2*(i-2)], dolnyWiersz[2*(i+2)]));
		}
		srodkowyWiersz[2*i+1] = Dodaj(srodkowyWiersz[2*i+1], dolnyWiersz[2*(i-2)]);
		i++;
		srodkowyWiersz[2*i+1] = Dodaj(srodkowyWiersz[2*i+1], dolnyWiersz[2*(i-2)]);
	}
}

long long WybierzNajwiekszaWartosc(long long(&pierwszyWiersz)[2*MAX_WR]) 
{
	long long max = -1;
	long long i = 0;
	for(i = 0;i<_ilosc_wezlow;i++)	{
		if (pierwszyWiersz[2*i] > max) {
			max = pierwszyWiersz[2*i];
		}
		if (pierwszyWiersz[2*i+1] > max ) {
			max = pierwszyWiersz[2*i+1];
		}
	}
	return max;
}

struct lista

int main ()
{
	int skad,dokad;
	scanf("%d",&_ilosc_wezlow);
	for(int i=0;i<(_ilosc_wezlow-1);i++)
	{
		scanf("%d %d", &skad, &dokad);

		

	}
	
	PobierzKolejnyWierszZWejscia(pierwszyWiersz);
	long long drugiWiersz[2*MAX_WR];
	PobierzKolejnyWierszZWejscia(drugiWiersz);

	for(int i =0;i<_ilosc_wezlow;i++) {
		drugiWiersz[2*i] = -1;
	}

	long long trzeciWiersz[2*MAX_WR];
	PobierzKolejnyWierszZWejscia(trzeciWiersz);
	long long temp[2*MAX_WR];
	WykonajDodawanieWWierszach(pierwszyWiersz,drugiWiersz,trzeciWiersz);

	for(licznikW=3;licznikW<_ilosc_wierszy;licznikW++) {
		PobierzKolejnyWierszZWejscia(temp);
		for(int i = 0; i< (2*_ilosc_wezlow); i++) {
			pierwszyWiersz[i] = drugiWiersz[i];
			drugiWiersz[i] = trzeciWiersz[i];
			trzeciWiersz[i] = temp[i];
		}
		WykonajDodawanieWWierszach(pierwszyWiersz,drugiWiersz,trzeciWiersz);	
	}
	long long zwyciezca = WybierzNajwiekszaWartosc(trzeciWiersz);

	printf("%lld",zwyciezca);
	return 0;
}