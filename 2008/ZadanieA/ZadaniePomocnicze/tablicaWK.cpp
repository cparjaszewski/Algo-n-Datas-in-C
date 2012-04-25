/*
	Program wypisuj¹cy kratkê X x Y z³o¿on¹ z samych kropek
	Deadline: 18 Marca 2008
	Autor: Krzysztof Andrzej Parjaszewski (daguerre)
*/
#include <iostream>
#include <fstream>
using namespace std;

int main ()
{
	int ilosc_kolumn = 0,ilosc_wierszy = 0;
	scanf("%d %d", &ilosc_kolumn, &ilosc_wierszy);

	ofstream myfile;
	myfile.open("wynik.txt");
	
	
	for (int i = 0;i<ilosc_wierszy;i++) {
		for (int j = 0; j< ilosc_kolumn; j++) {
			 myfile << ".";
		}
		myfile << "\n";
	}
	myfile.close();

 
  return 0;;
}
