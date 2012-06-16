/* Project Euler #1 
 * Find sum of all multiples of 3 or 5 under 1000.
 */
#include <stdio.h>
int main (int argc, char** argv){
	unsigned int sum = 0;
	unsigned int i;
	for(i=3;i<1000;i++){
		if(i%3==0 || i%5==0){
			sum += i;
		}
	}
	printf("%u\n", sum);
}
