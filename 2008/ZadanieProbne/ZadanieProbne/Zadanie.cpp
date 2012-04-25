#include <iostream>

using namespace std;

 

int main(int argc,char **argv)
{
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);

	// cin >> a>> b;
	if (a > b)
	{
		int temp = a;
		a = b;
		b = temp;
	}
	
	for (int i = a; i <= b;i++) 
	{
		printf("%d",i);
		if ( i < b) 
		{
			printf("\n");
		}
	}

	return 0;
}