#include <stdio.h>
#include <stdlib.h>
#include <math.h>



#if defined(_MBCS)
typedef __int64 long64;
#else
typedef long long long64;
#endif

int M, k;
int X, Y, SIZE;
long64 * T;

void data_in();
void init();
void algorithm();
int compare (const void * a, const void * b)
{
	// The return value of this function should represent whether elem1 is considered less than, equal to, 
	// or greater than elem2 by returning, respectively, a negative value, zero or a positive value.
	long64 A = *(long64*)a; 
	long64 B = *(long64*)b;

	if(A < B) return 1;
	else if(A>B) return -1;
	else return 0;
}

int main(int argc, char *argv[]) {
  data_in();
  init();
  algorithm(); 
}

void data_in() {
	scanf("%d %d",&M, &k);
}

void init() {
	int half = sqrt(float(k));
	X = int((2.5*half) < M ? (2.5)*half : M);
	Y = M - X;
	SIZE = X*(X+1)/2;
	T = new long64[SIZE ];
}

void algorithm() {
	int c = 0;
	for(long64 i=M;i>Y;i--) {
		for(long64 j=M;j>=i;j--) {
			T[c++] = i * j;
		}
	}
	qsort(T,SIZE,sizeof(long64),compare);
	long64 lastValue = long64(M)*long64(M) + long64(1),
		   value;
	int i = 0;
	for(c=0;c<k;true){
		value = T[i++];
		if(value< lastValue) {
			printf("%lld\n",value);
			c++;
			lastValue = value;
		}
	}

	//quick sort

}
