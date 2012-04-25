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
class rangeTree {
public:
	rangeTree * TL, * TR, *BL, *BR;
	rangeTree * parent;
	field * myField;
	field * maxX, * maxY, * minX, * minY;
	int xFirstHalf, xSecondHalf;
	rangeTree(field * f);
	rangeTree(XPair * Xps, YPair * Yps, int count);
	void Add(field * f);
	void AddLeft(XPair * Xps, YPair * Yps);
	void AddRight(XPair * Xps, YPair * Yps);
	rangeTree * Find(field * f);
	field * FindDown();
	field * FindLeft();
};
rangeTree * RT;


// declarations
struct field {
	int x, y, val,retX,retY;
	long64 MAX;
	field * maxFrom;
	rangeTree * rt; 
};

struct XPair {
	int x;
	field * t;
};

struct YPair {
	int y;
	field * t;
};
struct XYPair {
	int x, y;
	field * t;
};

struct friendsArray {
	int friendCount;
	field * friends[max_k];
};

friendsArray currentFriends;

// numbers
int m, n, k,answer;

// field
field T[max_k];

// sorting buckets
XPair X[max_k], XX[max_k];
YPair Y[max_k], YY[max_k];
XYPair XY[max_k];


int compareXPair(const void * a, const void * b)
{
	// The return value of this function should represent whether elem1 is considered less than, equal to, 
	// or greater than elem2 by returning, respectively, a negative value, zero or a positive value.
	int A = (*(XPair*)a).x; 
	int B = (*(XPair*)b).x; 

	if(A < B) return -1;
	else if(A>B) return 1;
	else {
		int yA = (*(XPair*)a).t->y, 
			yB = (*(XPair*)b).t->y;
		if(yA < yB ) return -1;
		else if(yA > yB) return 1;
		else return 0;	
	} 
}

int compareYPair(const void * a, const void * b)
{
	// The return value of this function should represent whether elem1 is considered less than, equal to, 
	// or greater than elem2 by returning, respectively, a negative value, zero or a positive value.
	int A = (*(YPair*)a).y; 
	int B = (*(YPair*)b).y; 

	if(A < B) return -1;
	else if(A>B) return 1;
	else {
		int xA = (*(YPair*)a).t->x, 
			xB = (*(YPair*)b).t->x;
		if(xA < xB ) return -1;
		else if(xA > xB) return 1;
		else return 0;	
	} 
}


int compareXYPair(const void * a, const void * b)
{
	// The return value of this function should represent whether elem1 is considered less than, equal to, 
	// or greater than elem2 by returning, respectively, a negative value, zero or a positive value.
	int Ax = (*(XYPair*)a).x; 
	int Ay = (*(XYPair*)a).y; 
	int Bx = (*(XYPair*)b).x; 
	int By = (*(XYPair*)b).y; 


	if((Ax <= Bx) && (Ay <= By)) return -1;
	else return 1;
}


class fReturn {
public:
	field * f;
	bool isEmpty;

	void getFriends(rangeTree * rt ) {
		// find Down(x,y) and Left(x,y) - start to search for point between

		int tempFriendCount = 0;
		field * down=MNULL, * left=MNULL;
		if(f->rt->BL != MNULL) {
			down = f->rt->BL->maxX;
		}
		if(f->rt->BL != MNULL) {
			left = f->rt->BL->maxY;
		}
		/*int 
			down = X[f->retX-1].t,
			left = Y[f->retY-1].t;
		while((T[down].y>f.y)&&(T[down].retX>0)) down = X[T[down].retX-1].t;
		while((T[left].x>f.x)&&(T[left].retY>0)) left = Y[T[left].retY-1].t;*/

		if(this->f->retX==0) {
			tempFriendCount=0;
		} else if(this->f->retY== 0){
			tempFriendCount=0;
		} else if((down->retX==0)||(left->retY==0)) {
			tempFriendCount=0;
		}
		else if(down==left) {
			currentFriends.friends[tempFriendCount++] = down;
		} else {//search in between			
			currentFriends.friends[tempFriendCount++] = down;
			//int begin = T[down].retX,
			//	end   = T[left].retX; 
			field * mostRightDown = down;
			field * mostLeftUp = left;
			field * candidate = left->rt->BR->maxY;//T[Y[T[down].retY+1].t];

			while(true){
				if((candidate->x>mostLeftUp->x)&&(candidate->y>mostRightDown->y)) {
					if(candidate->y < mostLeftUp->y) {
						currentFriends.friends[tempFriendCount++] = candidate;
						candidate=X[candidate->retX-1].t;
					} else if(candidate->y == mostLeftUp->y) {
						candidate=Y[candidate->retY-1].t;							
					} else {
						candidate=X[candidate->retX-1].t;							
					}
				} else if(candidate->x<mostLeftUp->x) { 
					// this is a not good row - we must search higher (T[Y[candidate.retY+1].t]
					candidate = Y[candidate->retY+1].t;
				} else if(candidate->x==mostLeftUp->x) {
					if(candidate->y==mostLeftUp->y) {
						currentFriends.friends[tempFriendCount++] = candidate;
						// added the most left up friend  - escape loop
						break;
					} else {
						// skip it - go to the upper row T[Y[T[down].retY+1].t]
						candidate = Y[candidate->retY+1].t;
					}
				} else if(candidate->y==mostLeftUp->y) {
					candidate = X[candidate->retX-1].t;
				} else if(candidate->x > mostRightDown->x) {
					candidate = Y[candidate->retY+1].t;
				}
			}
		}
		currentFriends.friendCount = tempFriendCount;
	}
};
class queueElement {
public:
	queueElement * next;
	queueElement * prev;
	field value;

	queueElement(field f,queueElement *n, queueElement*p=MNULL ) {
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

	void add(field f) {
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

	fReturn takeTop() {
		fReturn fr; fr.isEmpty=true;
		if(this->elCount==0) {return fr;}
		else {
			fr.isEmpty=false;
			fr.f = &this->first->value;
			queueElement * temp = this->first;
			this->first = this->first->prev;
			delete temp;
			elCount--;
			return fr;
		}
	}

	


};




rangeTree::rangeTree(field * f) {
	this->parent = MNULL;
	this->TL=MNULL;	  this->TR=MNULL;	this->BL=MNULL;	  this->BR=MNULL;
	this->maxX=f; this->maxY=f; this->minX=f; this->minY=f;
	this->myField = f;
	this->myField->rt = this;
}

rangeTree::rangeTree(XPair * Xps, YPair * Yps, int count) {
		this->parent = MNULL;
		this->TL=MNULL;	  this->TR=MNULL;	this->BL=MNULL;	  this->BR=MNULL;
		this->maxX=Xps[count-1].t; 
		this->maxY=Yps[count-1].t;
		this->minX=Xps[0].t; 
		this->minY=Yps[0].t; 
		this->xFirstHalf  = count/2-1+count%2;
		this->xSecondHalf = count - this->xFirstHalf-1;
		this->myField = Xps[this->xFirstHalf].t;
		this->myField->rt = this;
		if(count > 4) {
			// Xps comes sorted here
			// Take the first half of X (the left part) and put it's point into Yps_minor
			// The the second half of X (the right part) and put it's point int Yps_major
			for(int i=0;i<xFirstHalf;i++) {
				Yps[i].y = Xps[i].t->y;
				Yps[i].t = Xps[i].t;
			}
				
			qsort(Yps                , this->xFirstHalf,sizeof(YPair), compareYPair);
			this->AddLeft(Xps, Yps);

			for(int i=xFirstHalf+1;i<count;i++) {
				Yps[i].y = Xps[i].t->y;
				Yps[i].t = Xps[i].t;
			}

			qsort(&Yps[this->xFirstHalf+1], this->xSecondHalf,sizeof(YPair),  compareYPair);
			this->AddRight(&Xps[this->xFirstHalf+1],&Yps[this->xFirstHalf+1]);
		} else {
			this->minX = Xps[0].t;		this->maxX = Xps[count-1].t;
			this->minY = Xps[0].t;		this->maxY = Xps[count-1].t;
			for(int i=0;i<count;i++) {
				if ((Xps[i].t->y < this->minY->y) || 
				   ((Xps[i].t->y == this->minY->y) && (Xps[i].t->x < this->minY->x))) {
					this->minY = Xps[i].t;
				}
				if ((Xps[i].t->y > this->maxY->y) || 
				   ((Xps[i].t->y == this->maxY->y) && (Xps[i].t->x > this->minY->x))) {
					this->maxY = Xps[i].t;
				}
			}

			for(int i=0;i<xFirstHalf;i++) this->Add(Xps[i].t);
			for(int i=this->xFirstHalf+1;i<count;i++) this->Add(Xps[i].t);
		}
	}

void rangeTree::Add(field * f) {
if(this->myField->x < f->x) {
	if(this->myField->y < f->y) {
		if(this->TR==MNULL) {
			this->TR = new rangeTree(f);
			this->TR->parent = this;
		}
		else {
			this->TR->Add(f);
			if( (this->maxY->y > f->y ) || 
			   ((this->maxY->y == f->y) && (this->maxY->x < f->x))) {
				this->TR->maxY = f;
			}
			if( (this->maxX->x > f->x ) || 
			   ((this->maxX->x == f->x) && (this->maxX->y < f->y))) {
				this->TR->maxX = f;
			}
		}
	} else {
		if(this->BR==MNULL) {
			this->BR = new rangeTree(f);
			this->BR->parent = this;
		}
		else {
			this->BR->Add(f);
			if( (this->minY->y < f->y ) || 
			   ((this->minY->y == f->y) && (this->minY->x > f->x))) {
				this->BR->minY = f;
			}
			if( (this->maxX->x > f->x ) || 
			   ((this->maxX->x == f->x) && (this->maxX->y < f->y))) {
				this->BR->maxX = f;
			}
		}
	}
} else {
	if(this->myField->y < f->y) {
		if(this->TL==MNULL) {
			this->TL = new rangeTree(f);
			this->TL->parent = this;
		}
		else {
			this->TL->Add(f);
			if( (this->maxY->y > f->y ) || 
			   ((this->maxY->y == f->y) && (this->maxY->x < f->x))) {
				this->TL->maxY = f;
			}
			if( (this->minX->x < f->x ) || 
			   ((this->minX->x == f->x) && (this->minX->y > f->y))) {
				this->TL->maxX = f;
			}
		}
	} else {
		if(this->BL==MNULL) {
			this->BL = new rangeTree(f);
			this->BL->parent = this;
		}
		else {
			this->BL->Add(f);
			if( (this->minY->y < f->y ) || 
			   ((this->minY->y == f->y) && (this->minY->x > f->x))) {
				this->BL->minY = f;
			}
			if( (this->minX->x < f->x ) || 
			   ((this->minX->x == f->x) && (this->minX->y > f->y))) {
				this->BL->maxX = f;
			}
		}
	}
}
}

void rangeTree::AddLeft(XPair * Xps, YPair * Yps) {
// Yps & Xps comes sorted here
// partition Yps on higher the this->myField->y and others
for(int i=0;i<this->xFirstHalf;i++) {
	Xps[i].x = Yps[i].t->x;
	Xps[i].t = Yps[i].t;
}

// we need to partition Yps
int down; 

for(down=0;down < this->xFirstHalf;down++) {
	if (Yps[down].y > this->myField->y) break;
	/*if ((Yps[down].y == this->myField->y) && ((T[Yps[down].t].x > this->myField->x)))break;*/
}		
int up = this->xFirstHalf - down;

if(down==0) this->BL=MNULL; 
else {
	qsort(Xps,down,sizeof(XPair),compareXPair);
	this->BL = new rangeTree(Xps, Yps , down  );
	this->BL->parent = this;
}

if(up==0) this->TL=MNULL;
else {
	qsort(Xps,up,sizeof(XPair),compareXPair);
	this->TL = new rangeTree(Xps, &Yps[down] , up);
	this->TL->parent = this;
}
}

void rangeTree::AddRight(XPair * Xps, YPair * Yps) {
	// Yps & Xps comes sorted here
	// partition Yps on higher the this->myField->y and others
	for(int i=0;i<this->xSecondHalf;i++) {
		Xps[i].x = Yps[i].t->x;
		Xps[i].t = Yps[i].t;
	}

	// we need to partition Yps
	int down; 

	for(down=0;down < this->xSecondHalf;down++) {
		if (Yps[down].y > this->myField->y) break;
		if ((Yps[down].y == this->myField->y) && ((Yps[down].t->x > this->myField->x)))break;
	}		
	int up = this->xSecondHalf - down;

	if(down==0) this->BR=MNULL; 
	else {
		qsort(Xps,down,sizeof(XPair),compareXPair);
		this->BR = new rangeTree(Xps, Yps , down  );
		this->BR->parent = this;
	}

	if(up==0) this->TR=MNULL;
	else {
		qsort(Xps,up,sizeof(XPair),compareXPair);
		this->TR = new rangeTree(Xps, &Yps[down] , up);
		this->TR->parent = this;
	}
}

rangeTree * rangeTree::Find(field * f){
return MNULL;
}

field * rangeTree::FindDown() {
return MNULL;
}

field * rangeTree::FindLeft() {
return MNULL;
}


queue * Q;

	
int main() {
	// read data
	scanf("%d %d\n%d", &m, &n, &k);
	for(int i=0; i<k; i++) {
		scanf("%d %d %d", &T[i].x , &T[i].y, &T[i].val);
		T[i].MAX = (long64)T[i].val;
		X[i].x = T[i].x; X[i].t = &T[i];
		Y[i].y = T[i].y; Y[i].t = &T[i];
	}

	// init data structures
	qsort(X,k,sizeof(XPair),compareXPair);
	qsort(Y,k,sizeof(YPair),compareYPair);
	//qsort(XY,k,sizeof(XYPair),compareXYPair);
	for(int i=0; i<k; i++) {
		X[i].t->retX = i;
		Y[i].t->retY = i;
		XX[i] = X[i];
		YY[i] = Y[i];
	}
	// init range tree
	RT = new rangeTree(XX,YY,k);

	int f=k;
	if ((X[k-1].t==Y[k-1].t)&&(X[k-1].x==m)&&(Y[k-1].y==n)) {
		f--;
		T[f].val = X[f].t->val;
	}  else {
		T[f].val = 0;T[f].x = m;	T[f].y = n;
		T[f].retX = k;X[k].x = m;X[k].t = &T[f];
		T[f].retY = k;Y[k].y = n;Y[k].t = &T[f];
		T[f].rt = RT;
//	fReturn * frtemp = new fReturn();frtemp->f = &T[f];frtemp->isEmpty=false;frtemp->getFriends();delete frtemp;
		for(int i=0;i<currentFriends.friendCount;i++) X[currentFriends.friends[i]->retX].t->maxFrom = &T[f];
	}
	
	Q = new queue();
	Q->add(T[f]);
	
	long64 thisMax = 0;


	// perform algorithm
	//while(Q->elCount > 0 ) {
	//	fReturn fr = Q->takeTop();
	//	if(fr.isEmpty ==false) {
	//		/*rangeTree * rt = RT->find(fr.f);
	//		fr.getFriends(rt);*/
	//		thisMax = (long64)X[fr.f->retX].t->MAX ;
	//		for(int i=0;i<currentFriends.friendCount;i++) {
	//			long64 candidateMax = thisMax + X[currentFriends.friends[i]->retX].t->val;
	//			if (candidateMax > X[currentFriends.friends[i]->retX].t->MAX) {
	//				X[currentFriends.friends[i]->retX].t->MAX = candidateMax;
	//				X[currentFriends.friends[i]->retX].t->maxFrom = X[fr.f->retX].t;
	//			}
	//			Q->add(*currentFriends.friends[i]);
	//		}
	//	} else {
	//		// something gone wrong = explode
	//		exit(1);
	//	}
	//}

	
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