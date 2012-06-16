/*
 * project euler #2 
 *
 * Find sum of even-valued fibonacci numbers <= 4 million
 *
 */
#include <stdio.h>

inline unsigned int isEven(unsigned int i){
	return i%2?0:1;
}

inline unsigned int nextFib(unsigned int prev1, unsigned int prev2){
	return prev1 + prev2;
}

int main(int argc, char** argv){
	unsigned long sum = 2;
	unsigned int prev1 = 1, prev2 = 2;
	unsigned int next;
	while( (next = nextFib(prev1, prev2) ) <= 4000000){
		if( isEven(next) ){
			sum += next;
		}
		prev1=prev2;
		prev2=next;
	}
	printf("%lu\n", sum);
}

