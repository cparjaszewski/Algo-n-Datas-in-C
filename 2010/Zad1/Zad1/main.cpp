#include <stdio.h>
#include <math.h>

#if defined(_MBCS)
typedef __int64 long64;
#else
typedef long long long64;
#endif

struct list {
	list * next;
	long64 * value;
} * listOfHeads;
const int TABLE_SIZE = 500000;

int main() {
	long64 BIGTABLE[TABLE_SIZE]; 
	for(long64 i=0;i<TABLE_SIZE;i++) {
		BIGTABLE[i]=-1;
	}
	long64 m;
	listOfHeads = new list();
	list * head = listOfHeads;
	list * last = listOfHeads;
	//listOfHeads = 
	long64 number, amount, temp, scale;

	scanf("%lld",&m);
	for(long64 i=0;i<m;i++) {
		scanf("%lld %lld",&number,&amount);
		temp = number;
		scale = 1;
		while(((temp %2)==0) && (temp >0)) {
			temp/=2;scale*=2;
		}
		temp = temp/2;
		if(BIGTABLE[temp]==-1) {
			BIGTABLE[temp] = 0;
			
			last->next = new list();
			last->value = &BIGTABLE[temp];
			last=last->next;
		} 
		BIGTABLE[temp] += amount * scale;
	}
	list * tempList = head;
	long64 tempX = 0;
	long64 sum = 0;
	while(tempList->next) {
		tempX = *tempList->value;
		while(tempX >0) {
			if((tempX%2)==1) {
				sum++;
			}
			tempX /= 2;
		}
		tempList = tempList->next;
	}
	printf("%lld",sum);

	return 0;
}