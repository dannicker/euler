#include "../primeset.c"
#include <stdlib.h>

/* test underlying storage structure  ( search / ordered add )

int main(int argc, char** argv){
	ull i;
	long long int pos;
	long int value;
	primeset* ps = psNew();
	for(i=0;i<100;i++){
		value = (float)random()/(float)RAND_MAX*9999;
		printf("Adding Element: %li ... ", value);
		if( 0 > (pos = psAdd(ps, value ) ) ){
			printf("Couldn't add an element!");
		}
		printf("added at %lli \n", pos);
		psDump(ps, 1);
	}
	psDump(ps, 1);

}
*/

int main( int argc, char** argv){
	primeset* ps = psInit();
	FILE* output = NULL;
	if(argc>1){
		output = fopen(argv[1], "w+");
	}
	genPrimes(ps, 1000000000000, output);
	psDump(ps, 1);
}	
