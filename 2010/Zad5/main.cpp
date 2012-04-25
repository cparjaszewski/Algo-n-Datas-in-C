#include <stdio.h>
#include <stdlib.h>


#if defined(_MBCS)
typedef __int64 long64;
#else
typedef long long long64;
#endif

const long64 EMPTY = 1000000000000000001ll;

const short MNULL = 0;


class mAVL {
public:
	

	char balance;
	long64 value;
	mAVL * leftNode;
	mAVL * rightNode;
	mAVL * succ;
	mAVL * pred;
	mAVL * parent;

	mAVL(long64 x) {
		this->clear();
		this->value = x;
	}

	~mAVL() {
		this->clear();
	}

	
	bool insert(long64 x, int i) {
 		if(this->value == EMPTY) {
			this->value = x;
			return true;
		} else if(this->value==x) return false;
		else if(x > this->value ) {
			if(this->rightNode == MNULL) {
				this->rightNode = new mAVL(x);
				// update preds
				if(this->succ!=MNULL) { this->rightNode->succ = this->succ; this->succ->pred = this->rightNode; }
				// update succs
				this->rightNode->pred = this; this->succ = this->rightNode;
				// update parent
				this->rightNode->parent = this;
				if(this->balance <= 0) {
					// there was no left son
					// or there was only the left son
					this->balance += 1;
					return (balance==1);
				} else {
					// there should no other situation
					mAVL* root = this;while(root->parent) root = root->parent;
					printf("\n1 i=%d insert bug @ %lld\n",i,x);root->printTree(0,false);exit(1);// return false;
				}
			} else {
				bool result = this->rightNode->insert(x,i);
				if(result) this->balance += 1;
				if(this->balance==2) {
					if(this->rightNode->balance >= 0) this->RightRightRotate();
					else if(this->rightNode->balance < 0) this->RightLeftRotate();
					else {
						// there should no other situation
						printf("\n2 i=%d insert bug @ %lld\n",i,x);exit(1);// return false;
					}
					result &= (this->parent->balance!=0);
				} else {
					result &= (this->balance!=0);
				}
				return result;
			}
		} else if(x < this->value ) {
			if(this->leftNode == MNULL) {
				this->leftNode = new mAVL(x);
				this->leftNode->parent = this;
				// update preds
				if(this->pred!=MNULL) {this->leftNode->pred = this->pred; this->pred->succ = this->leftNode;}
				// update succs
				this->leftNode->succ = this;       this->pred = this->leftNode;
				// update parent
				if(this->balance >= 0) {
					// there was no left son
					// or there was only the left son
					this->balance -= 1;
					return (balance==-1);
				} else {
					// there should no other situation
					printf("\n3 i=%d insert bug @ %lld\n",i,x);exit(1);//return false;
				}
			} else {
				bool result = this->leftNode->insert(x,i);
				if(result==true) {
					this->balance -= 1;
				} 
				if(this->balance==-2) {
					if(this->leftNode->balance <= 0) {
						this->LeftLeftRotate();
					} else if(this->leftNode->balance > 0) {
						this->LeftRightRotate();
					} else {
						// there should no other situation
						printf("\n4 i=%d insert bug @ %lld\n",i,x);exit(1);//return false;
					}
					result &= (this->parent->balance!=0);
				} else {
					result &= (this->balance!=0);
				}
				return result;
			}
		} else {
			printf("\n5 i=%d  insert bug @ %lld\n",i,x);exit(1);//return false;
		}
	}

	bool _delete(long64 x,int i) {
		if(this->value==x) {
			// so - we have found it 
			mAVL  * temp;
			if((this->leftNode==MNULL) && (this->rightNode==MNULL)) {
				// LEAF (no left no right node)
				// update succ & pred
				if(this->pred!=MNULL) this->pred->succ = this->succ;
				if(this->succ!=MNULL) this->succ->pred = this->pred;
				// update parent and other ancestors
				if(this->parent!=MNULL) {
					bool wasLeftNode = false;
					if(this->parent->rightNode==this) this->parent->rightNode = MNULL;
					else if(this->parent->leftNode == this) {wasLeftNode = true;this->parent->leftNode = MNULL;}
					this->parent->decreaseBalance(wasLeftNode);
					delete this;
				} else {
					this->clear(); // root case
				}
				//this->balanceCorrect();
				return true;
			} else if((this->rightNode==MNULL) || (this->leftNode==MNULL)) {
				if(this->rightNode==MNULL) {
					temp = this->leftNode;
					// update succ & pred
					if(temp->pred!=MNULL) {
						this->pred = temp->pred;
						temp->pred->succ = this;
					} else this->pred = MNULL;
				}
				else {
					temp = this->rightNode; 
					// update succ & pred
					if(temp->succ!=MNULL) {
						this->succ = temp->succ;
						temp->succ->pred = this;
					} else this->succ = MNULL;
				}

				// update parent and other ancestors
				if(temp->parent != MNULL) {
					temp->parent->decreaseBalance(temp);
					if(temp->parent->leftNode == temp) temp->parent->leftNode = MNULL;
					else if(temp->parent->rightNode == temp) temp->parent->rightNode = MNULL;
					if((temp->parent->leftNode == MNULL) && (temp->parent->rightNode == MNULL)) temp->parent->balance = 0;
				}
			
				this->value = temp->value;
				delete temp; temp=MNULL;
				//this->balanceCorrect();
				return true;
			} else {
				bool isTempLeftChild=false;
				if(this->pred!=MNULL) {
					temp = this->pred;
					if(temp->leftNode != MNULL) {
						if(temp->parent->leftNode == temp) {isTempLeftChild=true; temp->parent->leftNode = temp->leftNode;}
						else if(temp->parent->rightNode == temp) temp->parent->rightNode = temp->leftNode;
						temp->leftNode->parent = temp->parent;
					} else {
						if(temp->parent->leftNode == temp) {isTempLeftChild=true; temp->parent->leftNode = MNULL;}
						else if(temp->parent->rightNode == temp) temp->parent->rightNode = MNULL;
					}
					// update pred & succ
					this->pred = temp->pred;
					if(temp->pred!=MNULL) temp->pred->succ = this;
				} else {
					temp = this->succ;
					/*temp->parent->decreaseBalance(temp);
					temp->parent->rotateCorrect();*/
					if(temp->rightNode != MNULL) {
						if(temp->parent->leftNode == temp) {isTempLeftChild=true; temp->parent->leftNode = temp->rightNode;}
						else if(temp->parent->rightNode == temp) temp->parent->rightNode = temp->rightNode;
						temp->rightNode->parent = temp->parent;
					} else {
						if(temp->parent->leftNode == temp) {isTempLeftChild=true; temp->parent->leftNode = MNULL;}
						else if(temp->parent->rightNode == temp) temp->parent->rightNode = MNULL;
					}
					// update pred & succ
					this->succ = temp->succ;
					if(temp->succ!=MNULL) this->succ->pred = this;
				}
				temp->parent->decreaseBalance(isTempLeftChild);
				//temp->parent->rotateCorrect();
				// update value
				this->value = temp->value;
				delete temp;
				temp=MNULL;
				this->rotateCorrect();
				//this->balanceCorrect();
				return true;
			}
		} else if(x < this->value ) {
			if(this->leftNode==MNULL) {
				return false;
			} else {
				bool veryLeaf = ((this->leftNode->value==x)&&(this->leftNode->leftNode ==MNULL) && (this->leftNode->rightNode==MNULL));
				bool res = this->leftNode->_delete(x,i);
				if(veryLeaf) this->leftNode = MNULL;
				if(this->balance == 2) {
					bool res = false;
					if(this->rightNode->balance>=0) res=this->RightRightRotate();
					else res=this->RightLeftRotate();
					if(res&&(this->parent->parent !=MNULL)) this->parent->parent->decreaseBalance(this->parent);
				}
				return res;
			}
		} else if(x > this->value ) {
			if(this->rightNode==MNULL) {
				return false;
			} else {
				bool veryLeaf = ((this->rightNode->value==x)&&(this->rightNode->leftNode ==MNULL) && (this->rightNode->rightNode==MNULL));
				bool res = this->rightNode->_delete(x,i);
				if(veryLeaf) this->rightNode = MNULL;
				if(this->balance == -2) {
					bool res = false;
					if(this->leftNode->balance<= 0) res=this->LeftLeftRotate();
					else res=this->LeftRightRotate();
					if(res&&(this->parent->parent !=MNULL)) this->parent->parent->decreaseBalance(this->parent);
				}
				return res;
			}
		} else {
			printf("\n1 delete bug @ %lld\n",x);exit(1);// return false;// something wrong has happened
		}
	}
	long64 lower(long64 x) {
		if(this->value==x) return x;
		else if(x < this->value ) {
			if(this->leftNode==MNULL) {
				if(this->pred ==MNULL) return EMPTY;
				else return this->pred->value;
			} else return this->leftNode->lower(x);
		} else if(x > this->value)  {
			if(this->rightNode == MNULL) return this->value;
			else return this->rightNode->lower(x);
		} else {
			printf("\n1 lower bug @ %lld\n",x);exit(1);//return EMPTY;
		}
	}
	long64 upper(long64 x) {
		if(this->value==x) return x;
		else if(x < this->value ) {
			if(this->leftNode == MNULL) return this->value;
			else return this->leftNode->upper(x);
		} else if(x > this->value)  {
			if(this->rightNode==MNULL) {
				if(this->succ ==MNULL) return EMPTY;
				else return this->succ->value;
			} else return this->rightNode->upper(x);
		} else {
			printf("\n1 upper bug @ %lld\n",x);exit(1);//return EMPTY;
		}
	}

	bool RightRightRotate() {
		if(this->balance!=2) {
			exit(1);// return false;// some error occured;
		}
		if(this->rightNode->balance < 0) {
			exit(1);//return false;// some error occured;
		}
		mAVL * A = this;
		mAVL * B = this->rightNode;
		mAVL * C = B->leftNode;
		// update C:
		if(C!=MNULL) C->parent = A;
		// update B:
		bool BwasRightweighted = B->balance > 0;
		B->leftNode = A; 
		B->parent = A->parent;
		if(B->parent!=MNULL) {
			if(B->parent->leftNode == A) {
				B->parent->leftNode = B;
				//B->parent->balance += 1;
			} else if(B->parent->rightNode == A) {
				B->parent->rightNode = B;
				//B->parent->balance -= 1;
			}
		}
		B->balance -= 1;
		// update A:
		A->parent = B; A->rightNode = C;
		if(BwasRightweighted) A->balance -=2;
		else A->balance -= 1;
		return BwasRightweighted;
	}
	bool RightLeftRotate() {
		if(this->balance!=2) {
			exit(1);//return false; // some error occured;
		}
		mAVL * A = this;
		mAVL * B = this->rightNode;
		mAVL * C = B->leftNode;
		mAVL * D = C->leftNode;
		mAVL * E = C->rightNode;
		// update E: 
		
		if(E!= MNULL) E->parent = B;
		// update D: 
		if(D!= MNULL) D->parent = A; 
		// update C:
		C->leftNode = A; C->rightNode = B;
		C->parent = A->parent;
		if(C->parent!=MNULL) {
			if(C->parent->leftNode == A) C->parent->leftNode = C;
			else if(C->parent->rightNode == A) C->parent->rightNode = C;
		}
		bool CwasRightOverweighted = C->balance > 0;
		bool CwasLeftOverweighted = C->balance  < 0;
		C->balance = 0;
		// update B:
		B->leftNode = E; B->parent = C;
		if(((E==MNULL)&&(B->rightNode!=MNULL))||CwasLeftOverweighted) B->balance +=2;
		else B->balance +=1;
		// update A:
		A->rightNode = D; A->parent = C; 
		if(((D==MNULL)&&(A->leftNode!=MNULL))||CwasRightOverweighted) A->balance -= 3;
		else A->balance -= 2;
		return true;
	}

	bool LeftRightRotate() {
		if(this->balance!=-2) {
			exit(1);//return false; // some error occured;
		}
		mAVL * A = this;
		mAVL * B = this->leftNode;
		mAVL * C = B->rightNode;
		mAVL * D = C->rightNode;
		mAVL * E = C->leftNode;
		// update E: 
		if(E!= MNULL) E->parent = B;
		// update D: 
		if(D!= MNULL) D->parent = A; 
		// update C:
		C->rightNode = A; C->leftNode = B;
		C->parent = A->parent;
		if(C->parent!=MNULL) {
			if(C->parent->leftNode == A) {
				C->parent->leftNode = C;
				//C->parent->balance += 1;
			} else if(C->parent->rightNode == A) {
				C->parent->rightNode = C;
				//C->parent->balance -= 1;
			}
		}
		bool CwasLeftOverweighted = C->balance < 0;
		bool CwasRightOverweighted = C->balance  > 0;
		C->balance = 0;
		// update B:
		B->rightNode = E; B->parent = C;
		if(((E==MNULL) && (B->leftNode!=MNULL))||CwasRightOverweighted)B->balance -=2;
		else B->balance -=1;
		// update A:
		A->leftNode = D; A->parent = C; 
		if(((D==MNULL) && (A->rightNode!=MNULL))||CwasLeftOverweighted) A->balance += 3;
		else A->balance += 2;
		return true;
	}

	bool LeftLeftRotate() {
		if(this->balance!=-2) {
			exit(1);//return false; // some error occured;
		}
		if(this->leftNode->balance > 0) {
			exit(1);//return false;// some error occured;
		}
		mAVL * A = this;
		mAVL * B = this->leftNode;
		mAVL * C = B->rightNode;
		// update C:
		if(C!=MNULL) C->parent = A;
		// update B:
		bool BwasLeftweighted = B->balance < 0;
		B->rightNode = A; 
		B->parent = A->parent;
		if(B->parent!=MNULL) {
			if(B->parent->leftNode == A) B->parent->leftNode = B;
			else if(B->parent->rightNode == A) B->parent->rightNode = B;
		}
		B->balance += 1;
		// update A:
		A->parent = B;  A->leftNode = C;
		if(BwasLeftweighted) A->balance +=2;
		else A->balance += 1;
		return BwasLeftweighted;
	}

	void printTree(int i, int indent=0,bool skipPrint = true) {
		if(skipPrint) {if(indent==0) {this->balanceCorrect(i);this->succPredCheck(i);return;}}
		else{
			for(int k=0;k<indent;k++) printf(" ");
			printf("V=%lld ln=",this->value);
			// print leftNode
			if(this->leftNode==MNULL) printf("NULL rn=");
			else printf("%lld rn=",this->leftNode->value);
			// print rightNode
			if(this->rightNode==MNULL) printf("NULL succ=");
			else printf("%lld succ=",this->rightNode->value);
			// print succ
			if(this->succ==MNULL) printf("NULL pred=");
			else printf("%lld pred=",this->succ->value);
			// print pred
			if(this->pred==MNULL) printf("NULL");
			else printf("%lld",this->pred->value);
			// print balance:
			printf(" bal=%d ",this->balance);
			if((this->balance>=2)||(this->balance<=-2)) {
				printf("\nTREE is unbalanced ! ! !\nexiting...");
				exit(1);//return;
			}
			printf("\n");
			if(this->leftNode != MNULL) { printf("L:"); this->leftNode->printTree(i, indent + 2,skipPrint);  }
			if(this->rightNode !=MNULL) { printf("R:"); this->rightNode->printTree(i, indent + 2,skipPrint); }
			if(indent==0){
				this->balanceCorrect(i);
				this->succPredCheck(i);
			}
		}
	}

	void rotateCorrect() {
		bool res;
		if(this->balance==-2) {
			if(this->leftNode->balance<=0) res=this->LeftLeftRotate();
			else res=this->LeftRightRotate();
			if(res&&(this->parent->parent !=MNULL)) {
				this->parent->parent->decreaseBalance(this->parent);
				//this->parent->parent->rotateCorrect();
			}
		} else if(this->balance==2) {
			if(this->rightNode->balance >= 0) res=this->RightRightRotate();
			else res=this->RightLeftRotate();
			if(res&&(this->parent->parent !=MNULL)) {
				this->parent->parent->decreaseBalance(this->parent);
				//this->parent->parent->rotateCorrect();
			}
		}
	}

private:
	void clear() {
		this->leftNode = MNULL;
		this->rightNode = MNULL;
		this->parent = MNULL;
		this->succ = MNULL;
		this->pred = MNULL;
		this->balance = 0;
		this->value = EMPTY;
	}

	

	void balanceCorrect(int i) {
		int r=0,l=0;
		if(this->rightNode!=MNULL) {r = this->rightNode->getHeight();this->rightNode->balanceCorrect(i);}
		if(this->leftNode!=MNULL) {l = this->leftNode->getHeight();this->leftNode->balanceCorrect(i);}
		int diff = r - l;
		if(this->balance != diff) {
			printf("i=%d balanceIncorrect for %lld",i,this->value);exit(1);//return ;
		}

		if((this->balance<-1)||(this->balance>1)) {
			this->rotateCorrect();
		}
	}

	void succPredCheck(int i) {
		mAVL *root = this;
		while(root->parent !=MNULL) root = root->parent;
		mAVL * minNode=root, * maxNode=root;
		while(minNode->leftNode !=MNULL) minNode = minNode->leftNode;
		while(maxNode->rightNode !=MNULL) maxNode = maxNode->rightNode;
		int treeSize = root->getSize();treeSize--;
		mAVL * tempMin=minNode, * tempMax=maxNode;
		for(int k=0;k<treeSize;k++) tempMin = tempMin->succ;
		for(int k=0;k<treeSize;k++) tempMax = tempMax->pred;
		if((tempMin!=maxNode) && (tempMax!=minNode)) {
			printf("i=%d succPredCheck for %lld",i,this->value);exit(1);// return ;// wrong succ || pred order
		}
	}

	int getSize() {
		if((this->leftNode==MNULL) && (this->rightNode==MNULL)) return 1;
		else {
			int l=0, r=0;
			if(this->leftNode !=  MNULL) l = this->leftNode->getSize();
			if(this->rightNode != MNULL) r = this->rightNode->getSize();
			return r + l + 1;
		}
	}

	int getHeight() {
		if ((this->leftNode==MNULL) && (this->rightNode==MNULL)) {
			return 1;
		} else {
			int l=0,r=0;
			if(this->leftNode != MNULL) l = 1 + this->leftNode->getHeight();
			if(this->rightNode !=MNULL) r = 1 + this->rightNode->getHeight();
			if(l>r) return l; 
			else return r;
		}
	}
	void decreaseBalance(mAVL * who) {
		if(this->leftNode == who) this->balance +=1;
		else this->balance -=1;
		if((this->balance ==0) && (this->parent != MNULL)) this->parent->decreaseBalance(this);
		this->rotateCorrect();
	}

	void decreaseBalance(bool isLeftChild) {
		if(isLeftChild) this->balance +=1;
		else this->balance -=1;
		if((this->balance ==0) && (this->parent != MNULL)) this->parent->decreaseBalance(this);
		this->rotateCorrect();
	}
	
};

int N, i=0;
long64 currentValue,x;
char currentChar;
mAVL * tree;

int main() {
	scanf("%d\n",&N);
	tree = new mAVL(EMPTY);
	/*int whenStop = 90012;*/
	while(i < N) {
	scanf("%c %lld\n", &currentChar, &currentValue);
	if((currentChar ==10)||(currentChar ==13)) continue;
		/*if(i==whenStop) {
			int x = 1;
		}*/
		switch(currentChar) {
			case 'I': 
				tree->insert(currentValue,i); 
				while(true) if(tree->parent !=MNULL) tree = tree->parent; else break;
				//if(i>=whenStop) tree->printTree(i,0,true);
				break;
			case 'D':
				/*if(i >= whenStop) {*/
					// printf("\n%c %lld\n", currentChar, currentValue);
					if (tree->_delete(currentValue,i))printf("OK\n");
					else  printf("BRAK\n");
					
				/*} else {
					
					tree->_delete(currentValue);
				}*/
				
				while(true) if(tree->parent !=MNULL) tree = tree->parent; else break;
				/*if(tree->leftNode != MNULL) tree->leftNode->rotateCorrect();
				if(tree->rightNode != MNULL) tree->rightNode->rotateCorrect();*/
				//tree->printTree(0,true);
				//if(i>=whenStop) tree->printTree(i,0,true);
				break;
			case 'U':
				x = tree->upper(currentValue);
				if(x==EMPTY) printf("BRAK\n");
				else printf("%lld\n",x);
				//tree->printTree();
				break;
			case 'L':
				x = tree->lower(currentValue);
				if(x==EMPTY) printf("BRAK\n");
				else printf("%lld\n",x);
				//tree->printTree();
				break;
		}
		//tree->printTree();
		i++;
	}
	//tree->printTree();
	/*printf("OOKK\n");*/

	return 0;
}