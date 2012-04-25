#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <stdlib.h>

#if defined(_MBCS)
typedef __int64 long64;
#else
typedef long long long64;
#endif

const int max_k = 100001;
const int MNULL = 0;
struct XPair;
struct YPair;
struct field;

struct fieldList {
	field * value;
	fieldList * next;
};

// declarations
struct field {
public:
	int x, y, val,retX,retY;
	long64 MAX;
	field * maxFrom;
	fieldList * NEXTS;

	void addNext(field * f) {
		fieldList * k = new fieldList();
		k->value = f;
		k->next = this->NEXTS;
		this->NEXTS = k; 
	}

};

struct XPair {
	int x;
	field * t;
};
struct YPair {
	int y;
	field * t;
};


// numbers
int m, n, k, answer;

// field
field T[max_k];

// sorting buckets
XPair X[max_k];
YPair Y[max_k];

int compareXPair(const void * a, const void * b)
{
	// The return value of this function should represent whether elem1 is considered less than, equal to, 
	// or greater than elem2 by returning, respectively, a negative value, zero or a positive value.
	int A = (*(XPair*)a).x; 
	int B = (*(XPair*)b).x; 

	if(A < B) return 1;
	else if(A>B) return -1;
	else {
		int yA = (*(XPair*)a).t->y, 
			yB = (*(XPair*)b).t->y;
		if(yA < yB ) return 1;
		else if(yA > yB) return -1;
		else return 0;	
	} 
}

int compareYPair(const void * a, const void * b)
{
	// The return value of this function should represent whether elem1 is considered less than, equal to, 
	// or greater than elem2 by returning, respectively, a negative value, zero or a positive value.
	int A = (*(YPair*)a).y; 
	int B = (*(YPair*)b).y; 

	if(A < B) return 1;
	else if(A>B) return -1;
	else {
		int xA = (*(YPair*)a).t->x, 
			xB = (*(YPair*)b).t->x;
		if(xA < xB ) return 1;
		else if(xA > xB) return -1;
		else return 0;	
	} 
}




class queueElement {
public:
	queueElement * next;
	queueElement * prev;
	field * value;

	queueElement(field * f,queueElement *n, queueElement*p=MNULL ) {
		this->value = f;
		this->next = n;
		this->prev = p;
	}
	~queueElement() {
		this->next = MNULL;
		this->prev = MNULL;
	}
};

class queue {
public:
	queueElement * first;
	queueElement * last;
	int elCount;
	
	queue() {
		first = MNULL;
		last = MNULL;
		elCount = 0;
	}

	~queue() {
		if (this->elCount==0) return; 
		else if(this->elCount==1) {
			delete (this->first);
			this->first = MNULL;
			this->last = MNULL;
		} else if(this->elCount >1) {
			queueElement * temp = this->first;
			queueElement * tempN = this->first->prev;
			while(tempN != MNULL) {
				delete temp; temp = tempN;tempN = temp->prev;
			}
			if(temp != MNULL) delete temp;
		}
	}

	void add(field * f) {
		queueElement * newElement;
		if(this->elCount==0) {
			newElement = new queueElement(f, MNULL);
			this->first = newElement;
		} else {
			newElement = new queueElement(f, this->last);
			this->last->prev = newElement;
		}
		this->last = newElement;
		elCount++;
	}

	field * takeTop() {
		field * fd; 
		if(this->elCount==0) {return MNULL;}
		else {
			fd = this->first->value;
			queueElement * temp = this->first;
			this->first = this->first->prev;
			delete temp;
			elCount--;
			return fd;
		}
	}

	


};


class stackElement {
public:
	field * f;
	//int minY;
	int maxY;
};
class stack{
public:
	stackElement * top;
	int nPeek;
	stack(int size) {
		this->top = new stackElement[size];
		this->nPeek=-1;
	}
	void push(field * f) {
		stackElement * newSE = &this->top[++this->nPeek];
		newSE->f = f;
		newSE->maxY = -1;
		//newSE->minY = -1;
	}
	stackElement * pop() {
		if(nPeek==-1) return MNULL;
		else return &this->top[this->nPeek--];
	}
	stackElement * peek() {
		return &this->top[this->nPeek];
	}
};
queue * Q;
stack * STACK;

	
int main() {
	// read data
	scanf("%d %d\n%d", &m, &n, &k);
	for(int i=0; i<k; i++) {
		scanf("%d %d %d", &T[i].x , &T[i].y, &T[i].val);
		T[i].MAX = (long64)T[i].val;T[i].NEXTS=MNULL;
		X[i].x = T[i].x; X[i].t = &T[i];
		Y[i].y = T[i].y; Y[i].t = &T[i];
	}

	// init data structures
	qsort(X,k,sizeof(XPair),compareXPair);
	qsort(Y,k,sizeof(YPair),compareYPair);

	for(int i=0; i<k; i++) {
		X[i].t->retX = i;
		Y[i].t->retY = i;
	}


	int f=k;
	if ((X[k-1].t==Y[k-1].t)&&(X[k-1].x==m)&&(Y[k-1].y==n)) {
		f--;
		T[f].val = X[f].t->val;
	}  else {
		T[f].val = 0;T[f].x = m;	T[f].y = n;
		T[f].retX = k;X[k].x = m;X[k].t = &T[f];
		T[f].retY = k;Y[k].y = n;Y[k].t = &T[f];
	}
	STACK = new stack(f);
	STACK->push(&T[f]);
	
	int lastCol = -1;
	stackElement * tPeek;
	for(int i=0;i<f;i++) {
		tPeek = STACK->peek();
		field * currentX = X[i].t;
		if(currentX->x == lastCol) {
			tPeek->f->addNext(currentX);
			tPeek->maxY = currentX->y;
			STACK->push(currentX);
		} else if(currentX->y == tPeek->f->y) {
			STACK->peek()->maxY = currentX->y;
			STACK->pop()->f->addNext(currentX);
			STACK->push(currentX);
			lastCol = currentX->x;
		} else if(currentX->y > tPeek->f->y) {
			STACK->push(currentX);
			lastCol = currentX->x;
		} else { // begin emptying stack (currentX-> y < peek->f->y)
			stackElement * onTop; 
			while(true) {
				onTop = STACK->pop();
				if((onTop==MNULL) || (onTop->f->y <= currentX->y)) break;
				else {
					if (currentX->y > onTop->maxY ) onTop->maxY = currentX->y;
					onTop->f->addNext(currentX);
				}
			} 
			STACK->push(currentX);
		}
	}

	field * currentX = new field();
	currentX->x = 1; currentX->y=1;
	currentX->MAX = 0;currentX->maxFrom = MNULL;
	currentX->NEXTS = MNULL;
	currentX->retX = -1;
	currentX->retY = -1;
	if(currentX->x == lastCol) {
		tPeek->f->addNext(currentX);
		tPeek->maxY = currentX->y;
		STACK->push(currentX);
	} else if(currentX->y == tPeek->f->y) {
		STACK->peek()->maxY = currentX->y;
		STACK->pop()->f->addNext(currentX);
		STACK->push(currentX);
		lastCol = currentX->x;
	} else if(currentX->y > tPeek->f->y) {
		STACK->push(currentX);
		lastCol = currentX->x;
	} else { // begin emptying stack (currentX-> y < peek->f->y)
		stackElement * onTop; 
		while(true) {
			onTop = STACK->pop();
			if((onTop==MNULL) || (onTop->f->y <= currentX->y)) break;
			else {
				if (currentX->y > onTop->maxY ) onTop->maxY = currentX->y;
				onTop->f->addNext(currentX);
			}
		} 
		STACK->push(currentX);
	}


	
	Q = new queue();
	Q->add(&T[f]);
	
	long64 thisMax = 0;


	// perform algorithm
	while(Q->elCount > 0 ) {
		field * top = Q->takeTop();
		if(top != MNULL ) {
			fieldList * fL = top->NEXTS;
			thisMax = top->MAX ;
			while(fL!= MNULL) {
				long64 candidateMax = thisMax + fL->value->val;
				if (candidateMax > fL->value->MAX) {
					fL->value->MAX = candidateMax;
					fL->value->maxFrom = top;
				}
				Q->add(fL->value );
				fL = fL->next;
			}
		}
	}
//		} else {
//			// something gone wrong = explode
//			exit(1);
//		}
//	}

	
	//find max
	int max=0;for(int i=0;i<k;i++) if(T[i].MAX > T[max].MAX) max=i;
	// read path
	int pathLegth = 1;
	field * tempPath = &T[max];
	while((tempPath->x!=m)||(tempPath->y!=n)) {
		tempPath = tempPath->maxFrom;
		pathLegth++;
	}
	if(tempPath->val==0) {
		pathLegth--;
	}

	
	
	

	// write answer
	printf("%lld\n", X[0].t->MAX);
	printf("%d\n",pathLegth);
	tempPath = X[0].t;
	for(int i=0;i<pathLegth;i++) {
		printf("%d %d\n",tempPath->x, tempPath->y);
		tempPath = tempPath->maxFrom;
	}
	
	return 0;
}