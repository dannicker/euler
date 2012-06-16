/* Project Euler #3
 *
 * Largest prime factor of 600851475143;
 *
 **/

#include <stdio.h>
#include <string.h>

unsigned long long largestPrimeFactor(unsigned long long int i){
	unsigned long long f = i/2; // largest possible factor
	printf("First factor candidate in %llu is %llu\n", i, f);
	while(f >2){
		if( f%2 != 0){ // might be prime
			//printf("Checking %llu\n", f);
			if( isPrimeFactor(i, f) )
			return f;
			f-=2; // skip even numbrers
		}else{
			f-=1; // get to add numbers
		}
	}
	return 0;
}

int isPrimeFactor(long long unsigned i, long long unsigned c){
	if(isPrime(c) )
		if( i % c == 0 )
			return 1;
		//else printf("%llu is not prime.\n", c);
	//else printf("%llu is not a factor of %llu.\n", c, i);
	return 0;
}

int isPrime(unsigned long long c){
	if(c%2==0) return 0;
	unsigned long long i = 3;
	unsigned long long max = c/3;
	for(i=3;i< max;i+=2) // odds starting at 2 
		if( c % i == 0 )  // has factor, not prime.
			return 0;
	return 1;
}

int main(int argc, const char** argv){
	unsigned long long num;
	if(argc>1){
		num = (unsigned long long)strtoull(argv[1], argv[1] + strlen(argv[1]), 10);
		printf("%llu\n", largestPrimeFactor(num));
	}
	else printf("%llu\n", largestPrimeFactor(600851475143));
}
