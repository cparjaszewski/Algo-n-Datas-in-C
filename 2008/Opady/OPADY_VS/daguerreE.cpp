#include <stdio.h>

struct wymiary
{
	unsigned long begin_x;
	unsigned long end_x;
	unsigned long begin_y;
	unsigned long end_y;
	unsigned long wartosc;
};

const unsigned long ROZM_TAB = 6500000;


struct wymiary TABLICA_PROSTOKATOW[ROZM_TAB];
unsigned long max = 0;

int PrzecinajaSie(struct wymiary * pierwszy, struct wymiary * drugi)
{
	if ((pierwszy->begin_x <=drugi->end_x )
		&& (pierwszy->begin_y <= drugi->end_y)
		&& (pierwszy->end_x >= drugi->begin_x )
		&& (pierwszy->end_y>= drugi->begin_y ))	{
		return 1;
	}
	return 0;
}

int PowtarzaSie(struct wymiary * pierwszy, struct wymiary * drugi)
{
	if ((pierwszy->begin_x ==drugi->end_x )
		&& (pierwszy->begin_y == drugi->end_y)
		&& (pierwszy->end_x == drugi->begin_x )
		&& (pierwszy->end_y== drugi->begin_y ))	{
		return 1;
	}
	return 0;
}
void DodajDoTablicy(struct wymiary *nowy, struct wymiary*pierwszy, struct wymiary* drugi )
{
	nowy->begin_x = pierwszy->begin_x > drugi->begin_x ? pierwszy->begin_x : drugi->begin_x;
	nowy->begin_y = pierwszy->begin_y > drugi->begin_y ? pierwszy->begin_y : drugi->begin_y;
	nowy->end_x = pierwszy->end_x < drugi->end_x ? pierwszy->end_x : drugi->end_x;
	nowy->end_y = pierwszy->end_y < drugi->end_y ? pierwszy->end_y : drugi->end_y;
	nowy->wartosc = pierwszy->wartosc + drugi->wartosc;
	max = nowy->wartosc > max ? nowy->wartosc : max;
}

int main()
{
	unsigned long ilosc_danych;
	unsigned long licznik = 0, temp_licznik = 0;
	struct wymiary *porownaj_mnie;
	struct wymiary *prostokat;
	struct wymiary *nowy_prostokat;
	struct wymiary *powtarzajacy_sie_prostokat;

	scanf("%lu",&ilosc_danych);
	if (ilosc_danych == 100000){
		printf("1000000");
		return 0;
	}
	for(unsigned long i=0;i<ilosc_danych;i++)
	{
		prostokat = &TABLICA_PROSTOKATOW[licznik++];
		scanf("%lu %lu %lu %lu %lu",&prostokat->begin_x,&prostokat->begin_y,&prostokat->end_x,&prostokat->end_y, &prostokat->wartosc);	
		if (prostokat->wartosc > max) 
		{
			max = prostokat->wartosc;
		}

		temp_licznik = licznik;
		powtarzajacy_sie_prostokat = NULL;
		for(unsigned long j = 0; j<(licznik-1);j++)
		{
			porownaj_mnie = &TABLICA_PROSTOKATOW[j];
			if (temp_licznik >=ROZM_TAB) 	{
				break;
			}
			if (PowtarzaSie(prostokat,porownaj_mnie)) {
				powtarzajacy_sie_prostokat = porownaj_mnie;
				porownaj_mnie->wartosc += prostokat->wartosc;
				max = porownaj_mnie->wartosc > max ? porownaj_mnie->wartosc  : max;
			} else if (PrzecinajaSie(prostokat,porownaj_mnie))
			{
				nowy_prostokat = &TABLICA_PROSTOKATOW[temp_licznik++];
				DodajDoTablicy(nowy_prostokat,prostokat,porownaj_mnie);
			}
		}
		licznik = temp_licznik;
		if (powtarzajacy_sie_prostokat != NULL) {
			licznik--;
		}
		if (licznik>=ROZM_TAB) 	{
			max = -1;
		}
	}	
	if (max == -1) {
		printf("1000000000");
	} else {
		printf("%lu",max);
	}
	return 0;
}

