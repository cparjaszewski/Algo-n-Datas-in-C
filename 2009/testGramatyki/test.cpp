#include <stdio.h>
#include <stdlib.h>
#include "test.h"

const char NOT_TERMINAL[8] = {'A','B','C','D','E','F','G','H'};
const char TERMINAL[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

const int PRIMES[10] = {7,13,29,31,19,17,11,37,41,43};

FILE * plik;


int main(int argc, char *argv[])
{
	int ile_problemow;
	int i,n,t,s;
	int not_terminal;
	int terminal;
    int temp_n,temp_t,temp_n_n,temp_t_t;
	int count_n,count_t;

	int dlugosc;

	char *mode = "w";


	FILE *plik_wynik;
	
	plik_wynik = fopen("wynik.list", mode);
    ile_problemow = get_rand(10);

	if (plik_wynik == NULL)
	{
		printf("blad otwarcia pliku");
		return 1;
	}
	else
	{
		
		fprintf(plik_wynik,"%d\n",ile_problemow);
		for(i=0;i<ile_problemow;i++)
		{
			not_terminal = get_rand(512);
			temp_n = get_rand(512); 
			temp_n = (temp_n % 2) ? temp_n+1:temp_n;
			temp_n_n = temp_n;
			count_n = gcd(temp_n,512);

			terminal = get_rand(204);
			temp_t = get_rand(204); 
			temp_t = (temp_t % 2) ? temp_t + 1:temp_t;
			temp_t_t = temp_t;
			count_n = gcd(temp_t,204);

			dlugosc = get_rand(500);
			fprintf(plik_wynik,"%d %d\n", not_terminal,terminal);
			fprintf(plik_wynik,"A %c %c\n",NOT_TERMINAL[temp_n % 512 % 8],NOT_TERMINAL[(temp_n + temp_n_n) % 512 % 8]);
			temp_n = temp_n + temp_n_n % 512;

			for(n=0;n<not_terminal-1;n++)
			{
				fprintf(plik_wynik,"%c %c %c\n",NOT_TERMINAL[temp_n % 511 % 8],NOT_TERMINAL[(temp_n + temp_n_n) % 512 % 8],NOT_TERMINAL[(temp_n + 2*temp_n_n) % 512 % 8]);
				temp_n = temp_n + 2*temp_n_n;
			}

			for(t=0;t<terminal;t++)
			{

				fprintf(plik_wynik,"%c %c\n",NOT_TERMINAL[temp_t % 511 % 8],TERMINAL[(temp_t + temp_t_t) % 204 % 26]);
				temp_t += temp_t_t;
			}
			for(s=0;s<dlugosc;s++)
			{
				fprintf(plik_wynik,"%c",TERMINAL[get_rand(26)]);
			}
			fprintf(plik_wynik,"\n");
		}
		fclose(plik_wynik);
   }
   return 0;

}

int get_rand(int max)
{
	int n;
	n = rand() % max; 
	
	return n;
}
unsigned int gcd(unsigned int u, unsigned int v)
{
    int shift;
    int diff;
 
    /* GCD(0,x) := x */
    if (u == 0 || v == 0)
      return u | v;
 
    /* Let shift := lg K, where K is the greatest power of 2
       dividing both u and v. */
    for (shift = 0; ((u | v) & 1) == 0; ++shift) {
        u >>= 1;
        v >>= 1;
    }
 
    while ((u & 1) == 0)
      u >>= 1;
 
    /* From here on, u is always odd. */
    do {
        while ((v & 1) == 0)  /* Loop X */
          v >>= 1;
 
        /* Now u and v are both odd, so diff(u, v) is even.
           Let u = min(u, v), v = diff(u, v)/2. */
        if (u <= v) {
            v -= u;
        } else {
            diff = u - v;
            u = v;
            v = diff;
        }
        v >>= 1;
    } while (v != 0);
 
    return u << shift;
}