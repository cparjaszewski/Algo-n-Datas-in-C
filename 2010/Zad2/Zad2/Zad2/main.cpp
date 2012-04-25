#include <stdio.h>

unsigned int 
     n, // size of checkboard
     m, // modulus
	 p; // no of forbidden 3x3 boards

unsigned int 
	current_block[1024], // j
	next_block[1024],    // j + 1
	matcher[1024],       // which confs are illegal 
	f33_board[5];        // translated check-board

unsigned short 
	graph[1024][32];         // pointers to dependants

char
	forbidden[5][9];

// get input data
void data_in();

// initialize data structures
void init();

// perform the main loop 
void loop();

// print the result
void data_out();

// print without preprocessing
void data_out_zero();

bool is_collision(unsigned int i, unsigned int k);


int main() {
	data_in();
	if(p==0) {
		data_out_zero();
	} else {
		init();
		loop();
		data_out();
	}
	return 0;
}

// get input data
void data_in() {
	scanf("%u %u %u", &n, &p, &m);
	for (int i = 0; i<p; i++) {
		scanf("%s", &forbidden[i][0]);
		scanf("%s", &forbidden[i][3]);
		scanf("%s", &forbidden[i][6]);
	}
}

// initialize data structures
void init() {
	// fill current block
	// j
	for(int i=0;i<1024;i++) { current_block[i]=1; }

	// fill next block
	// j + 1
	for(int i=0;i<1024;i++) { next_block[i]   =0; }

	// fill the 3x3 board
	for(int i=0;i<p;i++) {
		int temp=1,value=0;
		for(int k=0;k<9;k++) {
			if(forbidden[i][k]=='x') value += temp;
			temp <<= 1;
		}
		f33_board[i] = value;
	}

	// fill matcher
	// which confs are illegal 
	for(unsigned int i=0;i<1024;i++) {
		unsigned int value = 0;
		unsigned int temp = 1;
		for(unsigned int k=0;k<32;k++) {
			bool iscollision = is_collision(i, k);
			if(iscollision==false) value +=temp;
			temp <<= 1;
		}
		matcher[i] = value;

	}

	// fill graph
	// pointers to dependants
	for(int i=0; i<1024; i++) {
		for(int k=0; k<32; k++) {
			int firstCol = (i & 682) >> 1;
			int value = firstCol + ((k & 1)<<1)+((k & 2)<<2)+((k & 4)<<3)+((k & 8)<<4)+((k & 16)<<5);
			graph[i][k] = value;
		}		
	}
}

// perform the main loop 
void loop() {
	for (int i = 3; i <= n ; i++) {
		for (int j = 0; j < 1024 ; j++) {
			unsigned int match = matcher[j];
			unsigned int value = 1;
			for(int k=0;k<32;k++) {
				if(match & value) {
					next_block[j] = current_block[graph[j][k]] + next_block[j]  ;
				} else {
					int t = 1;
				}
				value <<= 1;
			}
		}
		for (int j = 0; j < 1024 ; j++) { 
			current_block[j] = next_block[j] % m;
			next_block[j] = 0;
		}
	}
}

// print the result
void data_out() {
	unsigned int sum = 0;
	for (int i = 0; i < 1024 ; i++) { 
		sum = (sum + current_block[i]) % m;
	}
	printf("%u",sum);
}

// print without preprocessing
void data_out_zero() {
	unsigned int sum = 1;
	for (int i = 0; i < 5*n ; i++) { 
		sum = (sum * 2) % m;
	}
	printf("%u",sum);

}


// preprocessing, we may hasten this, but - 960 is not worth
bool is_collision(unsigned int i, unsigned int k) {
	bool result = false;
	for(unsigned int a =0; a<p; a++) {
		unsigned int aboard = f33_board[a];
       // upper collision
	   if( (i &  1)    ==(aboard &   1) &&  (i &   2)    ==(aboard &   2) && ((k & 1)<<2)==(aboard &   4) &&
		  ((i &  4)<<1)==(aboard &   8) && ((i &   8)<<1)==(aboard &  16) && ((k & 2)<<4)==(aboard &  32) &&
		  ((i & 16)<<2)==(aboard &  64) && ((i &  32)<<2)==(aboard & 128) && ((k & 4)<<6)==(aboard & 256) ) {
			result = true;
			break;
	   }

	   // middle collision
	   if(((i &   4)>>2)==(aboard &   1) && ((i &   8)>>2)==(aboard &   2) && ((k & 2)<<1)==(aboard &  4) &&
		  ((i &  16)>>1)==(aboard &   8) && ((i &  32)>>1)==(aboard &  16) && ((k & 4)<<3)==(aboard & 32) &&
		   (i &  64)    ==(aboard &  64) &&  (i & 128)    ==(aboard & 128) && ((k & 8)<<5)==(aboard & 256) ) {
			result = true;
			break;
	   }

	   // lower collision
	   if(((i &  16)>>4)==(aboard &   1) && ((i &  32)>>4)==(aboard &   2) && ( k &  4)    ==(aboard &   4) &&
		  ((i &  64)>>3)==(aboard &   8) && ((i & 128)>>3)==(aboard &  16) && ((k &  8)<<2)==(aboard &  32) &&
		  ((i & 256)>>2)==(aboard &  64) && ((i & 512)>>2)==(aboard & 128) && ((k & 16)<<4)==(aboard & 256) ) {
			result = true;
			break;
	   }
	}
	return result;
}
