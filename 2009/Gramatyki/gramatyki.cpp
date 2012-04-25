///GRAMATYKA
/// 8 Symboli nieterminalnych: A,B,C,D,E,F,G,H
///16 Symboli terminalnych: a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z

///GRAMATYKA jest podana w postaci normalnej Chomskiego, czyli ka¿da z produkcji jest w jednej z 2 postaci:
///a) B -> CD (produkcja typu 1)
///b) C -> g (produkcja typu 2)


///WEJŒCIE
///1 wiersz - iloœæ podproblemów
///PODPROBLEM
///1 wiersz - ilosc_produkcji typu 1, ilosc_produkcji typu 2
///PRODUKCJA TYPU 1

///PRODUKCJA TYPU 2

///WYJŒCIE


///ilosc podproblemow in [1..10]

#include <stdio.h>
#include <stdlib.h>

int nieterminale[64];
int terminale[26];
int duzaGlobalnaTablica[500][500];


int rozwiaz_problem();
int pierwszy_krok(char []);
int drugi_krok_petla(int);
int suma(int,int);
int operator_gramatyki(int ,int);

int main() { 
	int ilosc_podproblemow;
	int i;
	int buf10[10];

    scanf("%d\n", &ilosc_podproblemow);
	
	for(i=0; i<ilosc_podproblemow; i++) {
		buf10[i] = rozwiaz_problem();
	}
	for(i=0; i<ilosc_podproblemow; i++) {
		if (buf10[i]) {
			printf("TAK\n");
		}
		else {
			printf("NIE\n");
		}
	}

	return 0;
}

int rozwiaz_problem() {
	int j;
	int k;
	int x,y,z;
	int ile_prod_nieterminali;
	int ile_prod_terminali;
	int dlugosc_slowa;
	char linijka[501];
	char buf5[5];
	char buf3[3];
	char c,znak;
	int dlugosc;

	scanf("%d %d\n", &ile_prod_nieterminali, &ile_prod_terminali);

	for(j=0;j<64;j++) {nieterminale[j] = 0;}
	for(j = 0;j< ile_prod_nieterminali; j++)  {
	  scanf("%5c\n", buf5);

	  x = 8*(int)(buf5[0] - 'A');
	  y = (int)(buf5[2] - 'A');
      z = 1 << (int)(buf5[4] - 'A') ;

      nieterminale[x + y] = nieterminale[x + y] + z - (nieterminale[x + y] & z);
	}

	for(j=0;j<26;j++) {terminale[j] = 0;}

	for(k = 0;k < ile_prod_terminali;k++) {
	  scanf("%3c\n", buf3);
	  x = 1 << (int)(buf3[0] - 'A');
	  y = (int)(buf3[2] - 'a') ;

      terminale[y] = terminale[y] +  x - (terminale[y] & x);
 	}

	for(k=0;k< 500;k++) {
		for(j =0;j<500;j++) {
			duzaGlobalnaTablica[k][j] = -1;
		}
	}

	scanf("%s",linijka);
	dlugosc_slowa = pierwszy_krok(linijka);
	//przygotuj duza tablice (250 000 operacji!)
	//oznacz kazda komorke flaga -1;
	if (dlugosc_slowa > 0) {

		return drugi_krok_petla(dlugosc_slowa);
	}
	else {
		return 0;
	}	
}

int pierwszy_krok(char linijka[]) {
	int dlugosc,j,term;
	char c, znak;
    dlugosc=0;
    c = linijka[dlugosc];
	while(c!=0) {
		znak = linijka[dlugosc];
		term = terminale[(int)(znak - 'a')];
		if (term == 0) {
			//oznacz blad;
			duzaGlobalnaTablica[0][0]=-1;
			return -1;
		}
		else {
			duzaGlobalnaTablica[dlugosc][dlugosc] = term;
		}
		c = linijka[++dlugosc];
	}
	//oznacz koniec
    duzaGlobalnaTablica[dlugosc][dlugosc] = -1;
	return dlugosc;
}

int drugi_krok_petla(int dlugosc_slowa) {
	int wynik;
	int i,j;
	for(i=1;i<dlugosc_slowa;i++) {
		for(j=i;j<dlugosc_slowa;j++) {
			duzaGlobalnaTablica[j-i][j] = suma(j-i,j);
		}	
	}
	wynik = duzaGlobalnaTablica[0][dlugosc_slowa-1] & ('A' + 1 - 'A');
	return wynik;
}


int suma(int start,int koniec) {
	int i;
	int result,temp;
	int beg,end;
	int temp1,temp2;
	beg = start;
	end = koniec;
	result = 0;

	for(i = beg;i < end;i++) {
		temp1 = duzaGlobalnaTablica[beg][i];
		temp2 = duzaGlobalnaTablica[i+1][end];
		temp = operator_gramatyki(temp1,temp2);
		result = result + temp - (result & temp);	
	}

	return result;
}

int operator_gramatyki(int prev, int next) {
	int shifterL,shifterP,jumperL;	
	int result = 0;
	int i;
	int p,n;
	p = prev;n=next;
	if ((p == -1) || (n == -1)) {
		return 0;
	}

	shifterL=1;shifterP = 1;jumperL = 0;
	while(p >= shifterL){
		shifterP = 1;
		if (p & shifterL) {
			while (n >= shifterP ) {
				if (n & shifterP) {
					for(i=0;i<8;i++) {
						if(nieterminale[8*i+jumperL] & shifterP) {								
							result = result + (1 << i);
						}
					}
				}
				shifterP = (shifterP << 1);
			}
		}
		shifterL = (shifterL << 1 );
		jumperL++;
	}
	return result;
}




/*
int drugie_kroki(struct my_linijka temp)
{
	int prev,next,i,j,k,dlugosc;
	int success = 0;
	dlugosc =0;
	//puste dane
	if (temp.linijka[0] == -1) {
		return 0;
	}
	while(k!=-1)	{
		k = temp.linijka[dlugosc++];
	}
	dlugosc--;
	//specjalna obs³uga dla s³owa d³ugoœci 1.
	if (dlugosc == 1) {
		if ( (temp.linijka[0] & (int)('A' + 1 - 'A') )== 1 ) {
			return 1;
		}
		else {
			return 0;
		}
	}
 	
	for(k=0;k<dlugosc-1;k++)
	{
		prev = temp.linijka[k];
		next = temp.linijka[k+1];
		// w kolejnym kroku skracam d³ugoœæ linijki o 1
		if (k == dlugosc-2) {temp.linijka[k+1] = -1;}
        operator_gramatyki(prev,next);
	}
	return (success?drugie_kroki(temp):0);	

*/