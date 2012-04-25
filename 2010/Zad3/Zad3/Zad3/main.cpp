#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#if defined(_MBCS)
typedef __int64 long64;
#else
typedef long long long64;
#endif

struct number {
	int x;
	int y;
	long64 value;
	number(int _x, int _y) {
		x = _x; y = _y;
		value = ((long64) x)*((long64) y);
	}
	number(long64 _x, long64 _y) {
		x = int(_x); y = int(_y);
		value = _x * _y;
	}
};


class mHeap {
	public:
		int heapSize;
		int nextElement;
		number ** T;
		
		// makeHeap
		mHeap(int x, int y);

		// insert a new number
		void insert(int x, int y);

		// delete T[0] & tidy
		number * deleteMax();
};

class mSet { // classical bst
	public:
	
	number * valueNumber;
	
	mSet * leftSon;
	mSet * rightSon;
	mSet(long64 x, long64 y);
	bool select(long64 x, long64 y);
	void insert(long64 x, long64 y);
	void Delete(long64 x, long64 y);
	void Delete(number * n);
	void setMaxDeleted(long64 value);
	mSet * getMaximalLower();
	mSet * getMinimalHigher();
private:
	long64 maxDeleted;
	
	mSet(number * n);
	bool select(number * n);
	void insertl(number * n, bool left);
	void tidy(long64 value);
	void deleteTree();

};

int M, k;
mHeap * root;
mSet  * mySet;
long64 buff[1024];
int a[98];

void data_in();
void init();
void algorithm();

int main(int argc, char *argv[]) {
  data_in();
  init();
  algorithm(); 
}

void data_in() {
	scanf("%d %d",&M, &k);
}

void init() {
	root = new mHeap(M, M);
	mySet = new mSet(M, M);
	

	for(int i=0;i<98;i++) {
		int d = k-(i*1024)<0?0:(k-(i*1024));
		a[i] = (d < 1024)?d:1024;
	}
}

void algorithm() {
	// main loop 
	long64 lastValue = long64(M)*long64(M)+long64(1);
	mySet->setMaxDeleted(lastValue);
	long64 value;
	int i;
	for(int j =0;j<98;j++) {
		for(i=0;i<a[j];true) {
			number * n = root->deleteMax();
			if(n==0) {
				break;
			}
			value = n->value;
			
			if(mySet ->select(n->x,   n->y-1)==false) {
				root ->insert(n->x,   n->y-1);
				mySet->insert(n->x,   n->y-1);
			}
			if(mySet ->select(n->x-1, n->y-1)==false) {
				root ->insert(n->x-1, n->y-1);
				mySet->insert(n->x-1, n->y-1);
			}
			if(mySet ->select(n->x-1, n->y)==false) {
				root ->insert(n->x-1, n->y);
				mySet->insert(n->x-1, n->y);
			}
			if(n->x > n->y ) {
				for(int h=n->y+1;h<n->x;h++) {
					if(mySet ->select(h, n->y)==false) {
						root ->insert(h, n->y);
						mySet->insert(h, n->y);
					}
				}
			}
			if (lastValue > value) {
				buff[i++] =n->value;
				lastValue = value;
				mySet->Delete(n);
				mySet->setMaxDeleted(value);
				delete n;
				n = 0;
			} 
		}
		for(int z = 0; z < a[j]; z++ )  fprintf(stdout, "%lld\n", buff[z]);
	}
}

// makeHeap
mHeap::mHeap(int x, int y) {
	this->T = new number*[3];
	this->heapSize = 3;
	this->nextElement = 1;
	number * n = new number(x, y);
	this->T[0] = n;
	this->T[1] = 0;
	this->T[2] = 0;
}

// insert a new number
void mHeap::insert(int x, int y) {
	// we want only positive values
	if((x<=0) || ( y<=0)) return;
	
	number * n = new number(x, y);
	int index = nextElement;
	long64 value = n->value;

	if(this->heapSize/(index+1) < 2) { //resize mHeap
		int begin      = this->heapSize;
		this->heapSize = this->heapSize * 2 + 1;
		this->T = (number **)realloc(T,sizeof(number*) * heapSize);

		for(int i = begin;i<heapSize;i++) {
			T[i] = 0; // zero new fields
		}
	}

	T[index] = n;
	this->nextElement++;
	number * temp;
	while (true) { // correct mHeap parent-son order 
		int parentIndex = (index-1)/2;
		if(value > T[parentIndex]->value) {
			// replace son with father:
			temp = T[index];
			T[index] = T[parentIndex];
			T[parentIndex] = temp;
			index = parentIndex;
		} else {
			break; // mHeap is in order
		}
	}
}

// delete T[0] & tidy
number * mHeap::deleteMax() {
	// empty mHeap - nothing to delete
	if(this->T[0] == 0) return 0;

	// decrease the next element counter
	this->nextElement--;
	number * max = new number(*T[0]),
	       * lastElement = T[this->nextElement],
		   * temp;

	int leftSon = 1,
		rightSon = 2,
		index = 0;
	long64 lastValue = lastElement->value;
		
	if(this->nextElement > 0) {
		T[index] = lastElement;
		T[this->nextElement]=0;
	} else {
		T[index] = 0;
		return max;
	}

	// Tidy
	while(true) {
		rightSon = (index+1)*2;
		leftSon = rightSon-1;
		long64 diffLeft  = 999999999, 
			   diffRight = 999999999;

		if((this->T[leftSon]==0)&&(this->T[rightSon]==0)) {
			// heap is in order
			break;
		} 
		if (this->T[leftSon]!=0) {
			diffLeft = lastValue - T[leftSon]->value;
		} 
		if (this->T[rightSon]!=0) {
			diffRight = lastValue - T[rightSon]->value;
		}

		if(diffLeft < diffRight) {
			// leftSon is higher:
			temp = T[leftSon];
			T[leftSon] = lastElement;
			T[index] = temp;
			index = leftSon;
		} else {
			// rightSon is higher:
			temp = T[rightSon];
			T[rightSon] = lastElement;
			T[index] = temp;
			index = rightSon;
		}
	}
	return max;
}

mSet::mSet(long64 x, long64 y) {
	this->leftSon = 0;
	this->rightSon = 0;
	this->valueNumber = new number(x, y);
}
mSet::mSet(number  * n) {
	this->leftSon = 0;
	this->rightSon = 0;
	this->valueNumber = n;
}

bool mSet::select(long64 x, long64 y) {
	number * n = new number(x, y);
	return this->select(n);
}
bool mSet::select(number * n) {
	long64 diff = n->value - this->valueNumber->value;
	if(diff == 0) {
		if ((n->x == this->valueNumber->x) || (n->x == this->valueNumber->y)) {
			return true;
		}
	} 
	if (diff<0) {
		if (this->leftSon == 0) return false;
		else return leftSon->select(n);
	} else if (diff>=0) {
		if (this->rightSon == 0) return false;
		else return rightSon->select(n);
	}
	return false;
}
void mSet::insert(long64 x, long64 y) {
	number * n = new number(x, y);
	this->insertl(n, x%2==1);
}
void mSet::insertl(number * n, bool left) {
	long64 thisValue = this->valueNumber->value;
	if(thisValue == 0) {
		this->valueNumber = n;	
		this->leftSon = 0;
		this->rightSon = 0;
	}

	long64 diff = n->value - this->valueNumber->value;
	if(diff < 0) {
		if(this->leftSon == 0) {
			this->leftSon = new mSet(n);
		} else {
			this->leftSon->insertl(n, left);
		}
	} else if(diff > 0) {
		if(this->rightSon == 0) {
			this->rightSon = new mSet(n);
		} else {
			this->rightSon->insertl(n, left);
		}
	} else if ((n->x != this->valueNumber->x )&&(n->x != this->valueNumber->y )) {
		if(this->rightSon == 0) {
			this->rightSon = new mSet(n);
		} else {
			this->rightSon->insertl(n, left);
		}
	}
	if((left==true)&&(this->rightSon==0)) {
		if(this->leftSon !=0) {
			number * tempRootNumber = this->valueNumber;
			mSet * left = this->leftSon->leftSon;
			this->valueNumber = this->leftSon->valueNumber;
			this->rightSon = new mSet(tempRootNumber);
			this->rightSon->leftSon = this->leftSon->rightSon;
			delete this->leftSon;
			this->leftSon = left;
		}		
	}

}
void mSet::Delete(long64 x, long64 y) {
	number * n = new number(x, y);
	this->Delete(n);
}


void mSet::Delete(number * n) {
	long64 diff = n->value - this->valueNumber->value;
	if(diff==0) {
		mSet * maximalLower = this->getMaximalLower();
		if(maximalLower==0) {
			if(this->rightSon!=0) {
				mSet * right = this->rightSon;
				this->valueNumber = right->valueNumber;
				this->rightSon = right->rightSon;
				this->leftSon = right->leftSon;
			} else {
				this->valueNumber = 0;
			}
		} else {
			this->valueNumber=maximalLower->valueNumber;
			maximalLower = 0;
		}
	} else if(diff < 0) {
		this->leftSon->Delete(n);
		if(this->leftSon->valueNumber==0) {
			delete this->leftSon;
			this->leftSon = 0;
		}
	} else {
		this->rightSon->Delete(n);
		if(this->rightSon->valueNumber==0) {
			delete this->rightSon;
			this->rightSon = 0;
		}
	}
	if((this->valueNumber!=0) &&(this->valueNumber->value==0)) {
		if(this->leftSon !=0) {
			if(this->rightSon!=0) {
				mSet * maxLower = this->getMaximalLower();
				delete this->valueNumber;
				this->valueNumber = new number(maxLower->valueNumber->x, maxLower->valueNumber->y);
				delete maxLower;
				maxLower = 0;
			}
			else {
				mSet * left = this->leftSon;
				this->leftSon = this->leftSon->leftSon;
				delete this->valueNumber;
				this->valueNumber = new number(left->valueNumber->x, left->valueNumber->y);
				delete left;
				left = 0;
			}
		}
		else if(this->rightSon !=0 ) {
			if(this->leftSon!=0) {
				mSet * minHigher = this->getMinimalHigher();
				delete this->valueNumber;
				this->valueNumber = new number(minHigher->valueNumber->x, minHigher->valueNumber->y);
				delete minHigher;
				minHigher = 0;
			}
			else {
				mSet * right = this->rightSon;
				this->rightSon = this->rightSon->rightSon;
				delete this->valueNumber;
				this->valueNumber = new number(right->valueNumber->x, right->valueNumber->y);
				delete right;
				right = 0;
			}
		}
	}
}
	

mSet * mSet::getMaximalLower() {
	mSet * temp = this->leftSon;
	mSet * prevTemp = 0;
	if(temp==0) {
		return temp;
	} 
	if((temp->rightSon==0)&&(this->leftSon !=0)) {
		this->leftSon = this->leftSon->leftSon ;
	}
	while(temp->rightSon != 0) {
		if(temp->rightSon->rightSon==0) {
			prevTemp = temp->rightSon;
		}
		temp = temp->rightSon;
	}
	if(prevTemp!=0) {
		prevTemp->rightSon = temp->leftSon;
	}
	return temp;
}
mSet * mSet::getMinimalHigher() {
	mSet * temp = this->rightSon;
	mSet * prevTemp = 0;
	if(temp==0) {
		return temp;
	} 
	if((temp->leftSon==0)&&(this->rightSon !=0)) {
		this->rightSon = this->rightSon->rightSon ;
	}
	while(temp->leftSon != 0) {
		if(temp->leftSon->leftSon==0) {
			prevTemp = temp->leftSon;
		}
		temp = temp->leftSon;
	}
	if(prevTemp!=0) {
		prevTemp->leftSon = temp->rightSon;
	}
	return temp;
}

void mSet::tidy(long64 value) {
	mSet * temp = this;
	while(temp!=0) {
		if(temp->valueNumber->value > value) {
			//delete right sub-tree:
			if(temp->rightSon!=0) {
				temp->rightSon->deleteTree();
				//delete temp->rightSon;
				temp->rightSon = 0;
			}
		}
		temp=temp->leftSon;
	}
}
void mSet::deleteTree() {
	mSet * temp = this;
	if((temp->leftSon==0) && (temp->rightSon==0)) return;
	if(temp->rightSon!=0) {
		if ((temp->rightSon->rightSon==0)&&(temp->rightSon->leftSon==0)) {
			//delete temp->rightSon;
			temp->rightSon = 0;
		} else if ((temp->rightSon->rightSon==0)&&(this->rightSon->leftSon!=0)) {
			temp->rightSon = temp->rightSon->leftSon;
			//delete temp->rightSon;
			temp->rightSon = 0;
		} else if ((this->rightSon->rightSon!=0)&&(this->rightSon->leftSon==0)) {
			temp->rightSon = temp->rightSon->rightSon;
			/*delete temp->rightSon;*/
			temp->rightSon = 0;
		} else {
			temp->rightSon->deleteTree();
		}
	} 
	if(temp->leftSon !=0) {
		if ((temp->leftSon->rightSon==0)&&(temp->leftSon->leftSon==0)) {
			/*delete temp->leftSon;*/
			temp->leftSon = 0;
		} else if ((temp->leftSon->rightSon==0)&&(this->leftSon->leftSon!=0)) {
			temp->leftSon = temp->leftSon->leftSon;
			/*delete temp->leftSon;*/
			temp->leftSon = 0;
		} else if ((this->leftSon->rightSon!=0)&&(this->leftSon->leftSon==0)) {
			temp->leftSon = temp->leftSon->rightSon;
			/*delete temp->leftSon;*/
			temp->leftSon = 0;
		} else {
			temp->leftSon->deleteTree();
		}
	}
	
}
void mSet::setMaxDeleted(long64 value) {
	this->maxDeleted = value;
	tidy(value);
}
