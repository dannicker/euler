#include "primes.inc.c"

int main(int argc, char* argv[]){
	printf("Prime Generator .9\n");
	if(argc<2){
		printf("Please provide data filenae\n");	
		exit(-2);
	}
	primeset* ps = psNew(argv[1], 1);
	ptype max = 1000000000L;
	printf("Generating primes up to %llu ... \n", max);
	genPrimes(ps, max);
}
