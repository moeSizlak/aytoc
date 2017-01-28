#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include "season5.h"
#include "ayto.h"


int main(int argc, char** argv) {
	
	Results_t r;
	
	int j;
	
	for(int i = 0; i < numEpisodes; i++) {
		j = addCeremony(&c[i], &a);
		if(j != 0) {
			printf("Failed to add ceremony[%d](%d)\n", i, j);
			return -1;
		}
		
		j = addTruth(&t[i], &a);
		if(j != 0) {
			printf("Failed to add truth[%d](%d)\n", i, j);
			return -1;
		}
	}
		
	getResults(&a, 0, &r);
	printResults(&a, &r);
	
	return 0;	
}
