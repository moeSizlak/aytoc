#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "season5.h"
#include "ayto.h"

#define NUMTHREADS (8)

#define EPISODE (-1)


int main(int argc, char** argv) {
	
	pthread_t thread[NUMTHREADS];
	ThreadArg_t threadArgs[NUMTHREADS];
	pthread_mutex_t lock;
	Results_t r = {0};
	time_t start, end;
	int q;
	
	start = clock();
	
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
	
	if(q = pthread_mutex_init(&lock,NULL)) {
		printf("ERROR; return code from pthread_mutex_init() is %d\n", q);
		return -1;
	}
	
	uint64_t fact = factorial(CARDINALITY);
	uint64_t chunk = fact/NUMTHREADS;
	int remainder = fact % NUMTHREADS;
	uint64_t next = 0;
	
	for(int g = 0; g < NUMTHREADS; g++) {
		threadArgs[g].threadIndex = g;
		threadArgs[g].a = &a;
		threadArgs[g].r = &r;
		threadArgs[g].episode = EPISODE;
		threadArgs[g].lock = &lock;
		threadArgs[g].firstP = next;
		threadArgs[g].lastP = (next + chunk - 1) + (g < remainder ? 1 : 0);
		next = threadArgs[g].lastP + 1;		
	}
	
	for(int g = 0; g < NUMTHREADS; g++) {
		q = pthread_create(&thread[g], NULL, getResultsT, (void *)&(threadArgs[g])); 
		if(q) {
			printf("ERROR; return code from pthread_create() is %d\n", q);
			return -1;
		}
	}

	for(int g = 0; g < NUMTHREADS; g++) {
		q = pthread_join(thread[g], NULL);
		if (q) {
			printf("ERROR; return code from pthread_join() is %d\n", q);
			return -1;
		}
	}
		
	end = clock();
	
	printResults(&a, &r);
	printf("Time was: %d ms\n", (int)(1000*(end-start)/CLOCKS_PER_SEC));
	
	return 0;	
}
