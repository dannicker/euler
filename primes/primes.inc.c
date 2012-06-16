#define _GNU_SOURCE
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <strings.h>

typedef unsigned long long ptype;

typedef struct primeset_t {
	ptype* primes;
	ptype count;
	ptype cur;
	FILE* mapfd;
	int announce;
	int autogen;
} primeset;


long filesize(FILE* fd){
	long oldlt = ftell(fd);
	long lt;
	fseek(fd, 0L, SEEK_END);
	lt = ftell(fd);
	fseek(fd, oldlt, SEEK_SET);
	return lt;
}

primeset* psNew(char* mapfile, int announce ){
	primeset* ps = (primeset*)malloc(sizeof(primeset));
	ps->announce = announce;
	struct stat *s;
	long fsize;
	s = malloc(sizeof(struct stat));
	if(! stat( mapfile, s) ){
		// file does not exist;
		fsize = 0;
	}else{
		fsize = (long)s->st_size;
	}
	ps->mapfd = fopen(mapfile, "w+b");
	if(!ps->mapfd){
		printf("Couldn't fopen %s\n", mapfile);
		exit(-2);
	}
	int pgsize = getpagesize();
	printf( "Map FD # %i\n", fileno(ps->mapfd)); 
	if(fsize < pgsize ){
		int more = sizeof(ptype)*pgsize - sizeof(ptype)*fsize;
		printf("Warning: File is too small by %i bytes... ", more);
		void* buf = malloc(more);
		memset(buf, 0, more);
		fseek(ps->mapfd, fsize, SEEK_SET);
		fwrite(buf, more, 1, ps->mapfd);
		fsize = pgsize;
		printf("Adjusted.\n");
	}
	
	ps->primes = (ptype*)mmap( NULL, fsize,  PROT_READ|PROT_WRITE, MAP_SHARED , fileno(ps->mapfd), 0);
	ps->cur = fsize>=sizeof(ptype)?(ps->primes[0]):1; // next write loc.
	ps->count = fsize/sizeof(ptype);
	ps->autogen = 1;

	if(ps->announce){
		printf("Prime data initialized.  %llu primes, space for %llu\n", ps->cur, ps->count);
	}
}

int psGrow(primeset* ps){
	int pagesize;
	ps->primes = mremap(ps->primes, ps->count * sizeof(ptype), ps->count * sizeof(ptype) + (pagesize = getpagesize() ) * sizeof(ptype), MREMAP_MAYMOVE  );
	if(!ps->primes){
		return 0;
	}
	ps->count += pagesize;
	return 1;
}

int psAdd(primeset* ps, ptype p){
	if(ps->cur == ps->count-1){
		if(!psGrow(ps)){
			return 0;
		}
	}
	ps->primes[ps->cur] = p;
	ps->primes[0] = ++ps->cur;
	if(ps->announce){
		printf("New Prime: %llu\n", p);
	}
	return 1;
}

ptype psMax(primeset* ps){
	return ( ps->cur > 1 ? ps->primes[ps->cur-1] : 0);
}

int isPrime(ptype p, primeset* ps ){
	ptype i;
	ptype max = (ptype)floor(sqrt(p));
	if(psMax(ps) < max){
		if( ! ps->autogen || !genPrimes(ps, max)){
			printf("Couldn't generate primes!\n");
			exit(-1);
		}
	}		
	for(i = 2; i <= max; i++){
		if(p % (ps->primes)[i] == 0) return 0; // factor
	} 	
	return 1; // prime, not % 0 any prime < √p
}


int genPrimes (primeset* ps, ptype max){
	if(!ps){
		printf("bad primeset.\n"); exit(-3);
	}else{
		printf("Primeset at %llx\n", (long long int)ps);
	}
	ptype start = 2;
	if(ps->cur>1){
		start = ps->primes[ps->cur-1];
	}
	else{ // set is empty.
		// add 2.
		if(!psAdd(ps, 2) ) {
			printf("Couldn't add prime %llu to list!\n", 2ULL );
			exit(-1);
		}
				return 0; // failed to add
		start = 3;
	}
	while(start <= max){
		if( isPrime(start, ps) ){
			if(!psAdd( ps, start) )
				return 0; // failed to add
		}else{
			printf("NOT prime:\t%llu\n", start);
		}
		start += 2; // next odd number
	}
	return 1;//generated as many as we wanted
}


