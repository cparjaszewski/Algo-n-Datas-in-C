#include <stdio.h>
#include <stack>
#include <vector>
#include <iostream>
#include <algorithm>
#include <set>

using namespace std;


namespace Zad6 {
	void znajdOstP();
	void mn2T();
	int budujUnionFind();
	void budujRowneWysokosci(int licznik);
	int start();
}

int n,m;

int T;
int const  MNULL = 0;
struct poleWyspy;
struct ufWezel {
	int licznik;
	poleWyspy * poleW;
	ufWezel * __reprezentant;
	ufWezel * Find() { // Struktura dla UNION-FIND ze skracaniem œcie¿ek :) (oby!)
		if(this->__reprezentant == MNULL) {
			// tutaj tworzy siê pêtla
			this->__reprezentant = this; return this;
		}
		while(this->__reprezentant != this->__reprezentant->__reprezentant) {
			// cut -  tutaj utworzy siê pêtla
			this->__reprezentant = this->__reprezentant->__reprezentant;
		}
		return this->__reprezentant;
	}
	void UstawReprezentanta(ufWezel * naKogo) {
		if     (this->__reprezentant == MNULL) {
			this->__reprezentant = naKogo; 
			this->Find();
			// //printf("UNION %d na: %d\n", this->licznik, naKogo->licznik);
		}
		else if(this->__reprezentant == naKogo)
			return;
		else if(this->__reprezentant == this) {
			this->__reprezentant = naKogo;
			this->Find();
			// //printf("UNION %d na: %d\n", this->licznik, naKogo->licznik);
		}
		 else
			this->__reprezentant->UstawReprezentanta(naKogo);		
	}
};
struct poleWyspy {
	int w;
	int ranga; // male oszustwo, zeby trzymac range w polu Wyspy
	ufWezel * rodzic;
};
void UNION(ufWezel * wezel1,ufWezel * wezel2) {
	if(wezel1->poleW->ranga >= wezel2->poleW->ranga) {
		wezel1->UstawReprezentanta(wezel2->Find());
		if(wezel1->poleW->ranga == wezel2->poleW->ranga) {
			wezel1->poleW->ranga++;
		}
	} else {
		wezel2->UstawReprezentanta(wezel1->Find());
		if(wezel2->poleW->ranga == wezel1->poleW->ranga) {
			wezel2->poleW->ranga++;
		}
	}
}
void UNION(ufWezel * wezel1,ufWezel * wezel2, bool print) {
	if(wezel1->poleW->ranga >= wezel2->poleW->ranga) {
		wezel1->UstawReprezentanta(wezel2->Find());
		if(wezel1->poleW->ranga == wezel2->poleW->ranga) {
			wezel1->poleW->ranga++;
		}
		if(print)
			printf("UNION %d pod %d\n" ,wezel1->licznik, wezel2->licznik);
	} else {
		wezel2->UstawReprezentanta(wezel1->Find());
		if(wezel2->poleW->ranga == wezel1->poleW->ranga) {
			wezel2->poleW->ranga++;
		}
		if(print)
			printf("UNION %d pod %d\n" ,wezel2->licznik, wezel1->licznik);
	}
}






struct czas {
	int w;
	int ind;
};


struct wierzcholekGrafu {
	ufWezel * rownyObszar;
	set<int, less<int> > sasiednieLiczniki;
};

czas czasy[100001];
poleWyspy calaWyspa[1000][1000];
ufWezel rowneObszaryTymczasowo[1000*1000];
ufWezel ** rowneObszaryTymczasowoDwa; //[1000*1000];
wierzcholekGrafu * GRAF;
inline void ustawSasiedztwo(int pole1, int pole2) {
	if(pole1 == pole2) 
		return;
	GRAF[pole1].sasiednieLiczniki.insert(pole2);
	GRAF[pole2].sasiednieLiczniki.insert(pole1);
}
vector<ufWezel*> dni[100001];
set <int, less<int> > liczby;


//stack< poleWyspy > stosWyspy;
vector<czas> vT;
vector<czas>::iterator vtIter;
int wynik[10001];


int Zad6::start() {
	int i,j;
	scanf("%d %d",&n,&m);
	for( i=0;i<n;++i)
		for( j=0;j<m;++j) {
			scanf("%d",&calaWyspa[i][j].w);
			calaWyspa[i][j].rodzic=MNULL;
		}
	scanf("%d",&T);
	for( i=0;i<T;++i)
		{scanf("%d",&czasy[i].w);czasy[i].ind = i;vT.push_back(czasy[i]);}
	czasy[T].w = czasy[i-1].w+1;
	czasy[T].ind = T;
	vT.push_back(czasy[T]);

	// Zad6::znajdOstP();
	
	Zad6::mn2T();
	int licznik = Zad6::budujUnionFind();
	Zad6::budujRowneWysokosci(licznik);

	return 0;
}



// Tlumaczy liczby z zakresu [1, 10^9] na zbiór to¿samy z T
// T jest od razu posortowany (plus)
inline bool mod_lesser ( czas elem1, czas elem2 ) {return elem1.w < elem2.w;}

void Zad6::mn2T() {
	int i,j,k;
	czas temp;
	//printf("\n");
	for(i=0;i<n;i++) {
		for(j=0;j<m;j++) {
			if(calaWyspa[i][j].w < czasy[0].w )
				k = -1;
			else if (calaWyspa[i][j].w > czasy[T].w )
				k = czasy[T].ind;
			else {
				temp.w = calaWyspa[i][j].w;
				vtIter = lower_bound( vT.begin(), vT.end(), temp, mod_lesser);
				k = ((czas) *(vtIter)).ind;
			}
			calaWyspa[i][j].w = k;
			//printf("%2d ",k);
		}
		//printf("\n");
	}
	//printf("\n");
	for(i=0;i<T;i++) {
		czasy[i].w = czasy[i].ind;
		//printf("%2d ", czasy[i]);
	}
	//printf("\n");
}

int budujUnionFind1Przejscie() {
	int i, j, licznik, im1, jm1;
	licznik = 0;
	i=0;
	j=0; // Pierwszy element
	calaWyspa[i][j].rodzic = &rowneObszaryTymczasowo[licznik];
	rowneObszaryTymczasowo[licznik].__reprezentant = MNULL;
	rowneObszaryTymczasowo[licznik].poleW = &calaWyspa[i][j];
	rowneObszaryTymczasowo[licznik].poleW->ranga = 0;
	rowneObszaryTymczasowo[licznik].licznik = licznik;
	//printf("\n");
	for(j=1;j<m;j++) { // Pierwszy wiersz
		if(calaWyspa[i][j-1].w == calaWyspa[i][j].w) {
			calaWyspa[i][j].rodzic = rowneObszaryTymczasowo[licznik].Find();
		} else {
			licznik++;
			calaWyspa[i][j].rodzic = rowneObszaryTymczasowo[licznik].Find();
			rowneObszaryTymczasowo[licznik].__reprezentant = MNULL;
			rowneObszaryTymczasowo[licznik].poleW = &calaWyspa[i][j];
			rowneObszaryTymczasowo[licznik].poleW->ranga = 0;
			rowneObszaryTymczasowo[licznik].licznik = licznik;
		}		
	}
	for(i=1;i<n;i++) {
		j=0; // Pierwszy element w kolumnie:
		im1 = i-1;
		// Nastepnie sprawdzane jest czy biezace pole wyspy przypadkiem nie nalezy do zbioru powyzej - jeœli tak, dodajemy go.
		if(calaWyspa[im1][j].w == calaWyspa[i][j].w) { 
			calaWyspa[i][j].rodzic = calaWyspa[im1][j].rodzic->Find(); 
		} else {
			licznik++;
			calaWyspa[i][j].rodzic = rowneObszaryTymczasowo[licznik].Find();
			rowneObszaryTymczasowo[licznik].__reprezentant = MNULL;
			rowneObszaryTymczasowo[licznik].poleW = &calaWyspa[i][j];
			rowneObszaryTymczasowo[licznik].poleW->ranga = 0;
			rowneObszaryTymczasowo[licznik].licznik = licznik;
		}
		
		for(j=1;j<m;j++) {
			jm1 = j-1;
			if(calaWyspa[i][jm1].w == calaWyspa[i][j].w) { // Czy pole na lewo ma te sama wartosc?
				calaWyspa[i][j].rodzic = calaWyspa[i][jm1].rodzic->Find();
			} 
			if(calaWyspa[im1][j].w == calaWyspa[i][j].w) { 
				// Czy pole powyzej ma te sam¹ wartosc?
				if(calaWyspa[i][j].rodzic==MNULL) { 
				// A wiec pole na lewo ma inn¹ wartosc
					calaWyspa[i][j].rodzic = calaWyspa[im1][j].rodzic->Find(); 
				} else if(calaWyspa[im1][j].rodzic == calaWyspa[i][jm1].rodzic ) { 
					// Nic nie trzeba robiæ, ale taka sytuacja jest malo prawdopodobna - wszyscy trzej sa w tym samym zbiorze
					calaWyspa[i][j].rodzic = calaWyspa[im1][j].rodzic->Find();
				} else if(calaWyspa[im1][j].w == calaWyspa[i][jm1].w) { // oho - trzeba po³¹czyæ dwa ró¿ne zbiory
					UNION(calaWyspa[im1][j].rodzic ,calaWyspa[i][jm1].rodzic);
					/* UNION (calaWyspa[im1][j].rodzic,   calaWyspa[i][i-1].rodzic); */
				} else { 
						// U gory jest taki sam element, ale na lewo inny - ³¹czymy z rodzicem u gory
						// calaWyspa[i][j].rodzic = calaWyspa[im1][j].rodzic; 
						// ALE! - to powinno byæ zlapane w pierwszym if'ie.
					
						// Lepiej wiêc jest rzuciæ wyjatek
					throw "Problem z okresleniem wartoœci rodzica";
				}
			} else if((calaWyspa[im1][j].w != calaWyspa[i][j].w) &&
				      (calaWyspa[i][jm1].w != calaWyspa[i][j].w)) {
				licznik++;
				calaWyspa[i][j].rodzic = rowneObszaryTymczasowo[licznik].Find();
				rowneObszaryTymczasowo[licznik].__reprezentant = MNULL;
				rowneObszaryTymczasowo[licznik].poleW = &calaWyspa[i][j];
				rowneObszaryTymczasowo[licznik].poleW->ranga = 0;
				rowneObszaryTymczasowo[licznik].licznik = licznik;
			}
		}
	}
	return licznik;
}
int budujUnionFind2Przejscie() {
	int i ,j, wlasnyLicznik;
	wlasnyLicznik = 0;
	//printf("\n");
	rowneObszaryTymczasowoDwa = new ufWezel*[n*m];
	for(i=0;i<n;i++) {
		for(j=0;j<m;j++) {
			int gg = (calaWyspa[i][j].rodzic->__reprezentant==MNULL) ? 
				                     calaWyspa[i][j].rodzic->licznik : calaWyspa[i][j].rodzic->Find()->licznik;
			rowneObszaryTymczasowoDwa[gg] = calaWyspa[i][j].rodzic->Find();
			//rowneObszaryTymczasowoDwa[gg].ranga = calaWyspa[i][j].rodzic->Find()->ranga;
			calaWyspa[i][j].rodzic = rowneObszaryTymczasowoDwa[gg];
			liczby.insert(gg);
			//printf("%2d ",gg);
			
		}
		//printf("\n");
	}

	wlasnyLicznik = liczby.size();
	set<int,less<int> >::iterator lIter;
	int c = 0;
	GRAF = new wierzcholekGrafu[wlasnyLicznik]; // tutaj moze byc n*m !
	for(lIter=liczby.begin();lIter != liczby.end();lIter++) {
		GRAF[c].rownyObszar = rowneObszaryTymczasowoDwa[*lIter];
		rowneObszaryTymczasowoDwa[*lIter]->licznik = c++;
	}

	/*printf("\n");
	for(i=0;i<n;i++) {
		for(j=0;j<m;j++) 
			printf("%2d ",calaWyspa[i][j].rodzic->licznik);
		printf("\n");
	}*/


	return wlasnyLicznik;
}
int Zad6::budujUnionFind() {
	budujUnionFind1Przejscie();
	int licznik = budujUnionFind2Przejscie();
	return licznik;
}


void Zad6::budujRowneWysokosci(int licznik) {
	int i, j, l, jm1, im1, t, obecnaWysokosc, obecnePole, poleNaLewo, poleUGory;
	vector<ufWezel*>::iterator it;
	set<int,less<int> >::iterator gIter;
	set<int,less<int> > * sasiedzi;
	set<int,less<int> > polaczeniaWysp;

	for(l=0;l<licznik;l++) { 
		obecnaWysokosc = GRAF[l].rownyObszar->poleW->w;
		dni[obecnaWysokosc].push_back(GRAF[l].rownyObszar);
	}
	//printf("\n");

	i=0;
	j=0;
	obecnePole = calaWyspa[i][j].rodzic->licznik;
	//printf("%2d ",obecnePole);
	for(j=1;j<m;j++) {
		//printf("%2d ",obecnePole);
		jm1=j-1;
		obecnePole = calaWyspa[i][j].rodzic->licznik;
		poleNaLewo = calaWyspa[i][jm1].rodzic->licznik;
		ustawSasiedztwo(obecnePole, poleNaLewo);
	}
	//printf("\n");
	for(i=1;i<n;i++) {
		im1=i-1;
		j=0;
		obecnePole = calaWyspa[i][j].rodzic->licznik;
		//printf("%2d ",obecnePole);
		poleUGory = calaWyspa[im1][j].rodzic->licznik;
		ustawSasiedztwo(obecnePole, poleUGory);
		
		for(j=1;j<m;j++) {
			jm1 = j-1;

			obecnePole = calaWyspa[i][j].rodzic->licznik;
			//printf("%2d ",obecnePole);
			poleUGory = calaWyspa[im1][j].rodzic->licznik;
			poleNaLewo = calaWyspa[i][jm1].rodzic->licznik;
			ustawSasiedztwo(obecnePole, poleNaLewo);
			ustawSasiedztwo(obecnePole, poleUGory);
		}
		//printf("\n");
	}

	delete [] rowneObszaryTymczasowoDwa;
	// NAJWAZNIEJSZA CZESC
	int nrSasiada, tm1, c, obecnePoleLicznik, doDodania;
	poleWyspy * obecnePoleWskaznik, * poleSasiada;
	t = T; c=0;
	wynik[t] = dni[t].size();
	
	for(;t>0;t--) {
		tm1 = t-1; //ile=wynik[t];
		polaczeniaWysp.clear();
		for(it = dni[t].begin(); it!=dni[t].end(); it++) {
			// Najpierw te z poprzedniej wysokosci
			obecnePoleWskaznik = (*(*(it))).poleW;
			obecnePoleLicznik  = (*(*(it))).poleW->rodzic->licznik;

			obecnePoleWskaznik->rodzic->__reprezentant = MNULL;
			obecnePoleWskaznik->rodzic->poleW->ranga = 0;

			c++;
						
			sasiedzi = &GRAF[obecnePoleLicznik].sasiednieLiczniki;
			for(gIter=sasiedzi->begin(); gIter != sasiedzi->end(); gIter++) {
				nrSasiada = *(gIter);
				if(GRAF[nrSasiada].rownyObszar->poleW->w > t) {
					poleSasiada = GRAF[nrSasiada].rownyObszar->poleW;
					UNION(poleSasiada->rodzic, obecnePoleWskaznik->rodzic);
				}
			}

			doDodania = obecnePoleWskaznik->rodzic->Find()->licznik;
		}
		for(it = dni[t].begin(); it!=dni[t].end(); it++) {
			obecnePoleWskaznik = (*(*(it))).poleW;
			doDodania = obecnePoleWskaznik->rodzic->Find()->licznik;
			polaczeniaWysp.insert(doDodania);
		}
	
		wynik[tm1] = polaczeniaWysp.size();

	}
	for(t=0;t<T;t++)
		printf("%d ", wynik[t]);
}



int main() {
	int wynik = Zad6::start();
	return wynik;
}