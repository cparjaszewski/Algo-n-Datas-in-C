#include <stdio.h>

// THIS IS TRY (IN-RANGE PRINTING) FUNCTION:
// 1) Function allocates memory (int) for a & b (while a,b are smaller than 1000).
int a,b;

int main() {
	// 2) Function reads input data with scanf() function "%d" - should be for int format.
	scanf("%d %d",&a, &b);

	// 3) If a is greater than b, we exchange them.
	if (a > b) {int temp = a;a=b;b=temp;}

	// 4) the for-loop is printing desired number to output with "%d" format. The '\n' stays for newline character.
	for(int i=a;i<=b;i++) {printf("%d\n",i);}

  // 5) return 0 - the program ends.
	return 0;
}