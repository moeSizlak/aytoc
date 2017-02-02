#ifndef _SEASON5_H
#define _SEASON5_H
#define CARDINALITY (11)
#include "ayto.h"


char* guys[CARDINALITY] = {"Andre","Derrick","Edward","Hayden","Jaylan","Joey","Michael","Mike","Osvaldo","Ozzy","Tyler"};
char* girls[CARDINALITY] = {"Alicia","Carolina","Casandra","Gianna","Hannah","Kam","Kari","Kathryn","Shannon","Taylor","Tyranny"};

int numEpisodes = 3;

Truth_t t[] = {
	{
		.pair = {"Hayden","Gianna"},
		.truth = NO_MATCH,
		.episode = 0.5	
	},
	{
		.pair = {"Andre","Alicia"},
		.truth = NO_MATCH,
		.episode = 1.5	
	},
	{
		.pair = {"Ozzy","Carolina"},
		.truth = NO_MATCH,
		.episode = 2.5	
	}
};
	
Ceremony_t c[] = {
	{
		.pairs = {
			{ "Andre", "Alicia" },
			{ "Derrick", "Kathryn" },
			{ "Edward", "Kam" },
			{ "Hayden", "Shannon" },
			{ "Jaylan", "Casandra" },
			{ "Joey", "Carolina" },
			{ "Michael", "Hannah" },
			{ "Mike", "Kari" },
			{ "Osvaldo", "Tyranny" },
			{ "Ozzy", "Gianna" },
			{ "Tyler", "Taylor" }						
		},
		.lights = 2,
		.episode = 1		
	},
	{
		.pairs = {
			{ "Andre", "Hannah"},
			{ "Derrick", "Alicia" },
			{ "Edward", "Shannon" },
			{ "Hayden", "Taylor" },
			{ "Jaylan", "Kam" },
			{ "Joey", "Carolina" },
			{ "Michael", "Gianna" },
			{ "Mike", "Casandra" },
			{ "Osvaldo", "Kari" },
			{ "Ozzy", "Kathryn" },
			{ "Tyler", "Tyranny" }
		},
		.lights = 0,
		.episode = 2		
	},
	{
		.pairs = {
			{"Andre","Kari"},
			{"Derrick","Hannah" },
			{"Edward","Kam" },
			{"Hayden","Carolina" },
			{"Jaylan","Casandra" },
			{"Joey","Kathryn" },
			{"Michael","Taylor" },
			{"Mike","Alicia" },
			{"Osvaldo","Tyranny" },
			{"Ozzy","Gianna" },
			{"Tyler","Shannon" }
		},
		
		.lights = 4,
		.episode = 3		
	}
};

Ayto_t a = {
	.guys = guys,
	.girls = girls,
	.numTruths = 0,
	.numCeremonies = 0
};
	


#endif