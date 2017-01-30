#ifndef _AYTO_H
#define _AYTO_H
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <locale.h>
#include <inttypes.h>
#include <pthread.h>


#define NO_MATCH (0)
#define PERFECT_MATCH (1)
#define GUY (1)
#define GIRL (2)

typedef struct Pair {
	char* guy;
	char* girl;
} Pair_t;

typedef struct Ceremony {
	Pair_t pairs[CARDINALITY];
	uint8_t guys[CARDINALITY];
	uint8_t girls[CARDINALITY];
	uint8_t lights;
	double episode;
} Ceremony_t;

typedef struct Truth {
	Pair_t pair;
	uint8_t guy;
	uint8_t girl;
	uint8_t truth;
	double episode;
} Truth_t;

typedef struct Results {
	uint64_t total;
	uint64_t results[CARDINALITY][CARDINALITY];
	uint64_t bo_numerator;
	uint64_t bo_denominator;
} Results_t;

typedef struct Ayto {
	char** guys;
	char** girls;
	Ceremony_t ceremonies[CARDINALITY*2];
	Truth_t truths[CARDINALITY*2];
	uint8_t numTruths;
	uint8_t numCeremonies;
} Ayto_t;

typedef struct ThreadArg {
	int threadIndex;
	Ayto_t* a;
	Results_t* r;
	double episode;
	pthread_mutex_t* lock;
	uint64_t firstP;
	uint64_t lastP;	
} ThreadArg_t;


int nameToInt(char* name, char** nameArray) {
	for(int i = 0; i < CARDINALITY; i++) {
		if(strcasecmp(nameArray[i], name) == 0) {
			return i;
		}
	}
	return -1;
}

void quickSortCeremony(uint8_t* x, uint8_t* a, Pair_t* b, int first, int last) {
	int pivot,j,i;
	uint8_t temp;
	Pair_t ptemp;

	if(first<last) {
		pivot=first;
		i=first;
		j=last;

		while(i<j) {
			while(x[i]<=x[pivot]&&i<last) {
				i++;
			}
			while(x[j]>x[pivot]) {
				j--;
			}
			if(i<j) {
				temp=x[i];
				x[i]=x[j];
				x[j]=temp;
				
				temp=a[i];
				a[i]=a[j];
				a[j]=temp;
				
				ptemp=b[i];
				b[i]=b[j];
				b[j]=ptemp;
			}
		}

		temp=x[pivot];
		x[pivot]=x[j];
		x[j]=temp;
		
		temp=a[pivot];
		a[pivot]=a[j];
		a[j]=temp;
		
		ptemp=b[pivot];
		b[pivot]=b[j];
		b[j]=ptemp;		
		
		quickSortCeremony(x,a,b,first,j-1);
		quickSortCeremony(x,a,b,j+1,last);
	}
}

int addCeremony(Ceremony_t* c, Ayto_t* a) {	
	if((*c).lights > CARDINALITY) {
		return -2;
	}
	
	for(int i = 0; i < CARDINALITY; i++) {
		int x = nameToInt((*c).pairs[i].guy, (*a).guys);
		int y = nameToInt((*c).pairs[i].girl, (*a).girls);
		if(x != -1 && y != -1) {
			(*c).guys[i] = x;
			(*c).girls[i] = y;
		}
		else {
			return -3;
		}
	}
	
	for (int i = 0; i < CARDINALITY - 1; i++) {
		for (int j = i + 1; j < CARDINALITY; j++) {
			if ((*c).guys[i] == (*c).guys[j] || (*c).girls[i] == (*c).girls[j]) {
				return -5;
			}
		}
	}
	
/* **********************************************		
	for(int i = 0; i < CARDINALITY; i++) {
		printf("%2d ", (*c).guys[i]);
	}
	printf("\n");
	for(int i = 0; i < CARDINALITY; i++) {
		printf("%2d ", (*c).girls[i]);
	}
	printf("\n");
	for(int i = 0; i < CARDINALITY; i++) {
		printf("%s ", (*c).pairs[i].guy);
	}
	printf("\n");
	for(int i = 0; i < CARDINALITY; i++) {
		printf("%s ", (*c).pairs[i].girl);
	}
	printf("\n\n");		
****************************************** */	
	quickSortCeremony((*c).guys, (*c).girls, (*c).pairs, 0, CARDINALITY-1);
/* **********************************************	
	for(int i = 0; i < CARDINALITY; i++) {
		printf("%2d ", (*c).guys[i]);
	}
	printf("\n");
	for(int i = 0; i < CARDINALITY; i++) {
		printf("%2d ", (*c).girls[i]);
	}
	printf("\n");
	for(int i = 0; i < CARDINALITY; i++) {
		printf("%s ", (*c).pairs[i].guy);
	}
	printf("\n");
	for(int i = 0; i < CARDINALITY; i++) {
		printf("%s ", (*c).pairs[i].girl);
	}
	printf("\n\n");	
****************************************** */
	
	if((*a).numCeremonies < (CARDINALITY*2)) {
		(*a).ceremonies[(*a).numCeremonies] = (*c);
		(*a).numCeremonies++;
		return 0;
	}
	else {
		return -4;
	}	
}

int addTruth(Truth_t* t, Ayto_t* a) {	
	if((*t).truth != NO_MATCH && (*t).truth != PERFECT_MATCH) {
		return -1;
	}	
	
	int x = nameToInt((*t).pair.guy, (*a).guys);
	int y = nameToInt((*t).pair.girl, (*a).girls);
	
	if(x != -1 && y != -1) {
		(*t).guy = x;
		(*t).girl = y;
	}
	else {
		return -3;
	}

	
	if((*a).numTruths < (CARDINALITY*2)) {
		(*a).truths[(*a).numTruths] = (*t);
		(*a).numTruths++;
		return 0;
	}
	else {
		return -4;
	}	
}

uint64_t factorial(int n) {
	uint64_t x = n;
	for(n = n - 1 ; n > 0; n--) {
		x *= n;
	}
	return x;
}


void* getResultsT(void* args)
{
	ThreadArg_t* myArgs = (ThreadArg_t*)args;
	
	int threadIndex = (*myArgs).threadIndex;
	Ayto_t* a = (*myArgs).a;	
	Results_t* r = (*myArgs).r;
	pthread_mutex_t* lock = (*myArgs).lock;
	double episode = (*myArgs).episode;
	uint64_t firstP = (*myArgs).firstP;
	uint64_t lastP = (*myArgs).lastP;
	
	uint64_t total = 0;
	uint64_t counts[CARDINALITY * CARDINALITY] = {0};
	
	uint64_t bo_numerator = 0;
	uint64_t bo_denominator = 0;
	
	if(episode <= 0) {
		episode = 999999;
	}
	
	uint8_t leftMatches[CARDINALITY*2];
	uint8_t rightMatches[CARDINALITY*2];
	uint8_t matchesLength = 0;
	
	uint8_t leftNonmatches[CARDINALITY*2];
	uint8_t rightNonmatches[CARDINALITY*2];
	uint8_t nonmatchesLength = 0;
	
	for(int i=0; i < (*a).numTruths; i++) {
		if((*a).truths[i].episode <= episode) {
			if((*a).truths[i].truth == NO_MATCH) {
				leftNonmatches[nonmatchesLength] = (*a).truths[i].guy;
				rightNonmatches[nonmatchesLength] = (*a).truths[i].girl;
				nonmatchesLength++;
			}
			else {
				leftMatches[matchesLength] = (*a).truths[i].guy;
				rightMatches[matchesLength] = (*a).truths[i].girl;
				matchesLength++;	
			}
		}
	}
	
	uint8_t leftBoNonmatches[CARDINALITY*2*CARDINALITY];
	uint8_t rightBoNonmatches[CARDINALITY*2*CARDINALITY];
	uint8_t boNonmatchesLength = 0;
	
	for(int i=0; i < (*a).numCeremonies; i++) {
		if((*a).ceremonies[i].episode <= episode && (*a).ceremonies[i].lights == 0) {
			for(int j = 0; j < CARDINALITY; j++) {
				leftBoNonmatches[boNonmatchesLength] = (*a).ceremonies[i].guys[j];
				rightBoNonmatches[boNonmatchesLength] = (*a).ceremonies[i].girls[j];
				boNonmatchesLength++;
			}
		}
	}
	
	uint8_t lights[CARDINALITY*2];
	uint8_t ceremonies[CARDINALITY*2*CARDINALITY];
	uint8_t ceremoniesLength = 0;
	
	for(int i=0; i < (*a).numCeremonies; i++) {
		if((*a).ceremonies[i].episode <= episode) {
			lights[ceremoniesLength] = (*a).ceremonies[i].lights;
			for(int j = 0; j < CARDINALITY; j++) {
				ceremonies[ceremoniesLength * CARDINALITY + j] = (*a).ceremonies[i].girls[j];
			}			
			ceremoniesLength++;
		}
	}
	
	uint64_t fact = factorial(CARDINALITY);
	uint8_t permuted[CARDINALITY];
	uint8_t elements[CARDINALITY];
	uint8_t identityPermutation[CARDINALITY];

	for(int i =0; i<CARDINALITY; i++) {
		identityPermutation[i] = i;
	}
	
	
	int index;
    int i, j;
    int m;
    int valid;
    int correct;
	
	int index2;
	int i2, j2, x2;
	int m2;
	int valid2;
	int correct2;
	uint8_t permuted2[CARDINALITY];
	uint8_t elements2[CARDINALITY];

 
    for(int p=firstP; p<=lastP; ++p ) {
		m = p;
		memcpy(elements, identityPermutation, sizeof elements);

		if(threadIndex == 0 && (!(p&511))) {
			printf("p==%10d, firstP==%10d, lastP=%10d, num=%'"PRIu64", den=%'"PRIu64"\n", (int)p, (int)firstP, (int)lastP, bo_numerator, bo_denominator);
		}
			
		// Antoine Cormeau's algorithm
		for( i=0; i<CARDINALITY; ++i ) {
			index = m % (CARDINALITY-i);
			m = m / (CARDINALITY-i);
			permuted[i] = elements[index];
			elements[index] = elements[CARDINALITY-i-1];
		}

		valid = 1;
		
		for(i = 0; i < matchesLength; i++) {
			if(permuted[leftMatches[i]] != rightMatches[i]) {
				valid = 0;
				break;
			}
		}
		
		if(!valid) {
			continue;
		}
		
		for(i = 0; i < nonmatchesLength; i++) {
			if(permuted[leftNonmatches[i]] == rightNonmatches[i]) {
				valid = 0;
				break;
			}
		}
		
		if(!valid) {
			continue;
		}
		
		for(i = 0; i < ceremoniesLength; i++) {
			correct = 0;
			for(j = 0; j < CARDINALITY; j++) {
				if(permuted[j] == ceremonies[i * CARDINALITY + j]) {
					correct += 1;
				}
			}
			
			if(correct != lights[i]) {
				valid = 0;
				break;
			}
		}
		
		if(!valid) {
			continue;
		}

		total++;
		//=================================================================
		
		for(int p2=0; p2<fact; ++p2 ) {
			m2 = p2;
			memcpy(elements2, identityPermutation, sizeof elements2);

			//if (p % (10*9*8*7*6*4*3*1) == 0) {
			//	printf("Progress: %3.0f percent\n", (100.0 * p) / fact);
			//}
				
			// Antoine Cormeau's algorithm
			for( i2=0; i2<CARDINALITY; ++i2 ) {
				index2 = m2 % (CARDINALITY-i2);
				m2 = m2 / (CARDINALITY-i2);
				permuted2[i2] = elements2[index2];
				elements2[index2] = elements2[CARDINALITY-i2-1];
			}

			valid2 = 1;
			
			for(i2 = 0; i2 < matchesLength; i2++) {
				if(permuted2[leftMatches[i2]] != rightMatches[i2]) {
					valid2 = 0;
					break;
				}
			}
			
			if(!valid2) {
				continue;
			}
			
			for(i2 = 0; i2 < nonmatchesLength; i2++) {
				if(permuted2[leftNonmatches[i2]] == rightNonmatches[i2]) {
					valid2 = 0;
					break;
				}
			}
			
			if(!valid2) {
				continue;
			}
			
			for(i2 = 0; i2 < boNonmatchesLength; i2++) {
				if(permuted2[leftBoNonmatches[i2]] == rightBoNonmatches[i2]) {
					valid2 = 0;
					break;
				}
			}
			
			if(!valid2) {
				continue;
			}
			
			bo_denominator++;
			
			for(i2=0; i2<CARDINALITY; i2++) {
				if(permuted2[i2] == permuted[i2]) {
					x2 = 0;
					for(j2=0; j2<matchesLength; j2++) {
						if(leftMatches[j2] == i2) {
							x2 = 1;
							break;
						}
					}
					if(x2 == 0) {
						valid2 = 0;
						break;
					}
				}
			}
		
			if(!valid2) {
				continue;
			}
			
			bo_numerator++;
		}
		
		
		//=================================================================
		for( i=0; i<CARDINALITY; ++i ) {
			counts[i * CARDINALITY + permuted[i]]++;
		}
	}

	pthread_mutex_lock(lock);
	(*r).total += total;
	(*r).bo_numerator += bo_numerator;
	(*r).bo_denominator += bo_denominator;
	
	for( int x = 0; x < CARDINALITY; x++ ) {
		for( int y = 0; y < CARDINALITY; y++ ) {
			(*r).results[x][y] += counts[x * CARDINALITY + y];
		}
	}
	pthread_mutex_unlock(lock);
	
	return 0;	
}

int printResults(Ayto_t* a, Results_t* r) {
	uint64_t total = (*r).total;
	uint64_t bon = (*r).bo_numerator;
	uint64_t bod = (*r).bo_denominator;
	
	char x1[64] = "%";
	char x2[64] = "%";
	char ws[64] = "";
	char d1[64] = "";
	setlocale(LC_NUMERIC, "");
	printf("%'d possibilities remain.\n\n", (int)total);
	printf("%'" PRIu64 " == blackout numerator.\n\n", bon);
	printf("%'" PRIu64 " == blackout denominator\n\n", bod);
	printf("%.5f == blackout odds\n\n", 100.0*((double)bon)/((double)bod));
	
	int w = 6;
	int l;
	for(int i = 0; i<CARDINALITY; i++) {
		l = strlen((*a).guys[i]);
		if(l > w) {
			w = l;
		}
		
		l = strlen((*a).girls[i]);
		if(l > w) {
			w = l;
		}
	}
	sprintf(ws,"%d",w);	
	strcat(x1, ws);
	strcat(x1, "s");
	strcat(x2, ws);
	strcat(x2, ".1f");
	for(int i = 0; i < w; i++) {
			strcat(d1, "-");
	}

	for(int x = 0; x < w; x++)
	{
		printf(" ");
	}
     
	for(int x = 0; x < CARDINALITY; x++) {
		printf("|");
		printf(x1,(*a).guys[x]);
		//header << '|' + sprintf("%#{w}s", @guys[x]) 
	}

	printf("\n");
	printf(d1);
	//header << "\n" + ('-' * w)
	
	for(int x = 0; x < CARDINALITY; x++) {
		printf("+");
		printf(x1,d1);
		//header << '+' + sprintf("%#{w}s", ('-' * w))
	}

	printf("\n");
	//header << "\n"

	for(int y = 0; y < CARDINALITY; y++) {
		printf(x1,(*a).girls[y]);
		//header << sprintf("%#{w}s", @girls[y]) 
		
		for(int x = 0; x < CARDINALITY; x++) {
			printf("|");
			printf(x2,(100.0*(*r).results[x][y])/(1.0 * total));
			//header << '|' + sprintf("%#{w}.1f", (100.0 * results[:results][x][y].to_f)/results[:total]) 
		}

		printf("\n");
		printf(d1);
		//header << "\n" + ('-' * w)
	
		for(int x = 0; x < CARDINALITY; x++) {
			printf("+");
			printf(x1,d1);
			//header << '+' + sprintf("%#{w}s", ('-' * w))
		}

		printf("\n");
	}

	return 0;
}

#endif
























