#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define true 1
#define false 0

#define PS_GROW_STEP 3

typedef unsigned long long ull;

typedef struct primeset_t{
	ull* data;
	long long maxsize;
	long long size;
} primeset;

primeset* psNew(){
	primeset* ps = (primeset*)malloc(sizeof(primeset));
	ps->maxsize = 0;
	ps->size = 0;
	ps->data = NULL;
}

primeset * psInit(){
	primeset* ps = psNew();
	int primes[8] = { 2, 3, 5, 7, 9, 11, 17, 19 };
	int i;
	for(i=0;i<8;i++){
		psAdd(ps, primes[i]);
	}
	return ps;
}

int psGrow(primeset* ps, long long amt){
	ull* newdata = realloc(ps->data, (ps->maxsize + amt) * sizeof(ull) );
	if(!newdata) return false;
	ps->data = newdata;
	ps->maxsize += amt;
	return true;
}

long long psIndex(primeset* ps, ull value){
	if(ps->size == 0){
		//printf("Zero size ps.\n");
		return 0; // (belongs at) begginning
	}
	if(ps->data[0] >= value) { 
		//printf("First element is larger than %llu.\n", value);
		return 0; // belongs at beginning
	}
	if(ps->data[ps->size-1] <= value ){
		//printf("Last element is <= %llu.\n", value);
		return ps->size; // belongs at end
	}
	int dir=1;
	long long i, space, max, min;
	max = ps->size-1;// we'll gradually constrict
	min = 0;			  // this range
	i=ps->size/2;
	while(1){
		//printf("Bubble sort loop begins, looking for %llu, min=%lli, max=%lli, i = %lli, [i] = %llu \n", value, min, max, i, ps->data[i] );
		if( ps->data[i] == value ) return i; // match
		if (ps->data[i] > value ){
			//printf("[i] > value \n");
			if( i == 0 || ps->data[i-1] < value ) // would-be position
				return i;
			if( ps->data[i-1] == i){
				return i-1;
			}
			dir = -1; // get smaller, [i-1] is still bigger than, 
		}
		else{ // [i] < value
			//printf("[i] < value \n");
			if( i == ps->size-1 )  // would-be pos
				return i;
			if( ps->data[i+1] >= value)
				return i+1;
			dir = 1; // next guess gets bigger
		}
		// how much to move?
		if(dir>0) { 
			space = max - i;
			min = i;
		} else{
			space = i - min;
			max = i;
		}
		i += dir * space / 2; // half the distance in that direction	
		//printf("Bubble sort loop ends, min=%lli, max=%lli, i = %lli, dir = %i", min, max, i, dir);
		// and ... neverending loop
	}
	// hmm, hopefully we never get here!
}

	

// this fn. returns an index, therfore 
// zero is an acceptable number from
// this function.  -1 is the 'error' code
int psAdd(primeset* ps, ull value){
	//if necessary grow to fit one more
	if(ps->size == ps->maxsize){
		if(!psGrow(ps, PS_GROW_STEP ) ){
			return -1;
		}
	}
	if(ps->size == 0){
		ps->data[ps->size++] = value;
		return 0; // careful, that's 0!
	}
	long long index = psIndex(ps, value);
	//printf("Index is %lli.\n", index);
	if(index == ps->size){
		ps->data[ps->size++]= value;
		return index;
	}
	if(ps->data[index] == value)
		return index; // already contained within array
	// now we need to shift...
	long long p ;
	for(p= ps->size;p>index;p--){
		ps->data[p] = ps->data[p-1];
	}
	ps->data[index] = value;
	ps->size++;
	return index;
}

int psDump(primeset* ps, int full ){
	printf("Primeset size %llu, space for %llu", ps->size, ps->maxsize);
	if(!full){
		printf("\n");
		return true;
	}
	printf(", elements:\n\t" );
	ull i = 0;
	for( i = 0; i<ps->size; i++){
		if(i) printf(", ");
		printf("[%3llu] = %llu", i, ps->data[i]);
	}
	printf("\n");
	return true;
}

ull psLast(primeset* ps){
	if(ps->size == 0) return 0;
	return ps->data[ps->size-1];
}

int genPrimes (primeset* ps, ull limit, FILE* output ){
	int generated = 0;
	ull prime, delta, candidate, largest;
	long long place;
	if(ps->size && (prime = psLast(ps)) > limit){
		return generated;
	}
	char* buf;
	if(output != NULL){
		fseek(output, 0, SEEK_END);
		buf = malloc(1024);
	}
	delta = ( prime +3) % 6;
	candidate = prime + delta;
	while(candidate < limit){
		 largest = sqrt(candidate);
		for( place=0;place<ps->size;place++){
			if( candidate % ps->data[place] == 0 ){
				break;
			}
			if( largest <= ps->data[place] ){
				psAdd(ps, candidate);
				if(output != NULL){
					int len = sprintf(buf, "%llu\n", candidate);
					fwrite(buf, len, sizeof(char), output);
				}
				generated++;
				break;
			}
		}
		delta ^= 6;
		candidate += delta;
	}
	return generated;
}
 
