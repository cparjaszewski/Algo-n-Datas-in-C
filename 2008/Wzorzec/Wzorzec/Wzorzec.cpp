#include <iostream>
using namespace std;

char **pattern=0,**data=0;
int x=0,y=0;
int XX=0,YY=0;

int findPatternHere(int _ox, int _oy) {
	for(int j=0;j<y;j++) {
		for(int i=0;i<x;i++) {
			if (pattern[j][i] != data[_oy + j][_ox + i]) {
				return 0;
			}
		}
	}
	return 1;
}

int main()
{

	
	
	int i=0,j=0;
	/**Get the pattern height, width and data height and width**/
	scanf("%d %d %d %d",&y,&x,&YY,&XX);
	
	/**Allocate memory for the pattern 2d array **/
	pattern = new char*[y];
	for ( i = 0; i < y; ++i)	{
		pattern[i] = new char[x];
	}
	for (i=0;i<y;i++) {
		scanf("%s",pattern[i]);
	}

	/**Allocate memory for the data 2d array **/
	data = new char*[YY];
	for ( i = 0; i < YY; ++i)	{
		data[i] = new char[XX];
	}
	for (i=0;i<YY;i++) {
		scanf("%s",data[i]);
	}
	
	int lastSearchRow = YY - y + 1;
	int lastSearchColumn = XX - x + 1;
	int result = 0;
	
	for(i=0;i<lastSearchRow;i++) {
		for(j=0;j<XX;j++) {
			if (findPatternHere(j,i)==1) {
				result++;
			}
		}		
	}

	printf("%d", result);
	return 0;
}


/**
struct PAir {
	int position;
	int value;	
};

	struct PAir temp, t;
	int search=0, rowsToEnd=YY;
	int a=0,II=0;
	memoryList->clear();
	memoryListTwo->clear();
	list<struct PAir>::iterator it;
	for(a=0;a<YY;a++){
		
        /** Fills the list with each data row 
		for(II=0;II<XX;II++) {
			t.position = a;
			t.value = II;
			memoryListTwo->push_back(t);
		}
		for(i=0;i<x;i++) {
		/** Copies entries from memoryListTwo to memoryList
			memoryList->clear();
			for(it=memoryListTwo->begin(); it != memoryListTwo->end(); ++it) {
				memoryList->push_back(*it);
			}
			memoryListTwo->clear();
			for(it=memoryList->begin(); it != memoryList->end(); ++it) {
				temp.value = ((struct PAir)(*it)).value ;
				temp.position = a;
				search = temp.value + i;
				if (search < XX) {
					if(pattern[a][i]==data[a][search]) {
						memoryListTwo->push_back(temp);	
					}
				}
			}
    	}
		memoryList = new list<struct PAir>();
		for(it=memoryListTwo->begin(); it != memoryListTwo->end(); ++it) {
			memoryList->push_back(*it);
		}
		memoryListTwo->clear();


		searchEngine->push_back(memoryList);
		memoryList = new list<struct PAir>();		

**/