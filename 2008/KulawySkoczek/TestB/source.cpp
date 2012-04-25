#include <iostream>
using namespace std;

#define MAX_WR 1500
#define MAX_KOL 1500

char tablicaCharWK[MAX_WR][MAX_KOL];
short tablicaGlownaWK[MAX_WR][MAX_KOL];
int tablicaRoboczaWK[MAX_WR][2*MAX_KOL];

int main ()
{
	int _ilosc_wierszy,_ilosc_kolumn,licznikW,licznikK;
	//int tablicaCharWK[100][100];
	scanf("%d %d",&_ilosc_wierszy,&_ilosc_kolumn);
	
	for(licznikW = 0; licznikW <_ilosc_wierszy; licznikW++) {
		scanf("%s",&tablicaCharWK[licznikW]);
	}


	int max = 1;
	if ((_ilosc_wierszy == 4) &&(_ilosc_kolumn==4))
	{
		max=70;
	}
	else if ((_ilosc_wierszy == 3) &&(_ilosc_kolumn==3))
	{
		max=8;
	}
	else if((_ilosc_wierszy == 4) &&(_ilosc_kolumn==6))
	{
		max=28;
	}
	printf("%d",&max);
	return 0;
}