#ifndef _AYTO_H
#define _AYTO_H
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>


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
	uint8_t episode;
} Ceremony_t;

typedef struct Truth {
	Pair_t pair;
	uint8_t guy;
	uint8_t girl;
	uint8_t truth;
	uint8_t episode;
} Truth_t;

typedef struct Results {
	uint64_t total;
	uint64_t results[CARDINALITY][CARDINALITY];
	double blackoutOdds;
} Results_t;

typedef struct Ayto {
		char** guys;
		char** girls;
		Ceremony_t ceremonies[CARDINALITY*2];
		Truth_t truths[CARDINALITY*2];
		uint8_t numTruths;
		uint8_t numCeremonies;
} Ayto_t;


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


int getResults(Ayto_t* a, int episode, Results_t* r)
{
	uint64_t total = 0;
	uint64_t counts[CARDINALITY * CARDINALITY] = {0};
	
	if(episode <= 0) {
		episode = 2147483647;
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

 
    for(int p=0; p<fact; ++p ) {
		m = p;
		memcpy(elements, identityPermutation, sizeof elements);

		//if (p % (10*9*8*7*6*4*3*1) == 0) {
		//	printf("Progress: %3.0f percent\n", (100.0 * p) / fact);
		//}
			
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
		
		for( i=0; i<CARDINALITY; ++i ) {
			counts[i * CARDINALITY + permuted[i]]++;
		}
	}

	(*r).total = total;
	
	for( int x = 0; x < CARDINALITY; x++ ) {
		for( int y = 0; y < CARDINALITY; y++ ) {
			(*r).results[x][y] = counts[x * CARDINALITY + y];
		}
	}
	
	return 0;	
}

int printResults(Ayto_t* a, Results_t* r) {
	uint64_t total = (*r).total;
	char x1[64] = "%";
	char x2[64] = "%";
	char ws[64] = "";
	char d1[64] = "";
	setlocale(LC_NUMERIC, "");
	printf("%'d possibilities remain.\n\n", (int)total);
	
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


/*


	
  def getResults(arg = {})
    raise ArgumentError, "Invalid arguments passed to getResults: :episode must be numeric" unless 
      !arg.key?(:episode) || arg[:episode].is_a?(Numeric)
      
    
    if !arg.key?(:episode)
      arg[:episode] = Float::INFINITY
    end

    # total possible winning matchups
    total = 0
    
    # number of matchups where the perfect matches are together
    validMatchups = 0    
    # number of validMatchups that result in a blackout
    validMatchupsBlackouts = 0    
    
    # number of matchups where the perfect matches are together and no matches are apart
    smartMatchups = 0    
    # number of smartMatchups that result in a blackout
    smartMatchupsBlackouts = 0

    counts = Array.new(@cardinality * @cardinality).fill(0)
        
    matches = @truths.select{|x| x.truth == :PERFECT_MATCH && x.episode <= arg[:episode]}.map{|x| [@guys.index(x.guy), @girls.index(x.girl)]}
    leftMatches = matches.map{|x| x[0]}
    rightMatches = matches.map{|x| x[1]}
    matchesLength = matches.length
    
    nonmatches = @truths.select{|x| x.truth == :NO_MATCH && x.episode <= arg[:episode]}.map{|x| [@guys.index(x.guy), @girls.index(x.girl)]}
    leftNonmatches = nonmatches.map{|x| x[0]}
    rightNonmatches = nonmatches.map{|x| x[1]}
    nonmatchesLength = nonmatches.length
    
    smartNonmatches = (nonmatches + @ceremonies.select{|c| c.episode <= arg[:episode] && c.lights == 0}.map{|c| c.pairs.sort_by{|s| @guys.index(s[:guy])}.map{|p| [@guys.index(p[:guy]), @girls.index(p[:girl])]}}.reduce(:+)).uniq
    leftSmartNonmatches = smartNonmatches.map{|x| x[0]}
    rightSmartNonmatches = smartNonmatches.map{|x| x[1]}
    smartNonmatchesLength = smartNonmatches.length
    
    lights = @ceremonies.select{|x| x.episode <= arg[:episode]}.map{|x| x.lights}
    ceremonies = @ceremonies.select{|c| c.episode <= arg[:episode]}.map{|c| c.pairs.sort_by{|s| @guys.index(s[:guy])}.map{|p| @girls.index(p[:girl])}}
    ceremonies = ceremonies.reduce(:+)
    ceremoniesLength = lights.length 
    
    blackoutsValid = (0...@cardinality).collect{|x| (0...@cardinality).to_a - (leftMatches.include?(x) ? [] : [x])}
    blackoutsSmart = blackoutsValid.dup
    
    (0...@cardinality).each do |x|
      if !leftMatches.include?(x)
        blackoutsSmart[x] = blackoutsSmart[x].drop(smartNonmatches.select{|y| y[0] == x}.count)
      end
    end
    
    puts smartNonmatches.to_s
    puts blackoutsSmart.to_s

    #fact = @cardinality.factorial

    permuted = Array.new(@cardinality)
    elements = Array.new(@cardinality)    
    identityPermutation = (0...@cardinality).to_a
    
    identityPermutation.permutation.each do |permuted|
    
      validMatchup = true
      smartMatchup = true
    
      # Verify the matches
      (0...matchesLength).each do |i|
        if permuted[leftMatches[i]] != leftMatches[i]
          validMatchup = false
          smartMatchup = false
          break
        end
      end
      
      # Verify the nonmatches
      if validMatchup
        (0...@cardinality).each do |x|
          if !leftMatches.include?(x)
            blackoutsSmart
          end
        end
      end
      
      if validMatchup
        validMatchups += 1
        validMatchupsBlackouts += 1 if (0...@cardinality).all?{|x| blackoutsValid[x].include?(permuted[x])}
      end
      
      if smartMatchup
        smartMatchups += 1
        smartMatchupsBlackouts += 1 if (0...@cardinality).all?{|x| blackoutsSmart[x].include?(permuted[x])}
      end
      
    
      ########################################
      
      valid = true
      
      # Verify the matches
      (0...matchesLength).each do |i|
        if permuted[leftMatches[i]] != rightMatches[i]
          valid = false
          break
        end
      end
      
      next if !valid

      # Verify the nonmatches
      (0...nonmatchesLength).each do |i|
        if permuted[leftNonmatches[i]] == rightNonmatches[i]
          valid = false
          break
        end
      end
      
      next if !valid

      # Verify the matching ceremonies
      (0...ceremoniesLength).each do |i|
        correct = 0
        (0...@cardinality).each do |j|
          if permuted[j] == ceremonies[i * @cardinality + j]
            correct += 1 
          end
        end
        
        if correct != lights[i]
          valid = false
          break
        end
      end
      
      next if !valid
      
      # Record the result
      total += 1
      (0...@cardinality).each do |i|
        counts[i * @cardinality + permuted[i]] += 1
      end    
    end
    
    results = Array.new(@cardinality){Array.new(@cardinality)}
    
    (0...@cardinality).each do |x|
      (0...@cardinality).each do |y|
        results[x][y] = counts[x * @cardinality + y]
      end
    end
    
    
    return {:total => total, :results => results, :validMatchupsBlackouts => validMatchupsBlackouts, :validMatchups => validMatchups, :smartMatchupsBlackouts => smartMatchupsBlackouts, :smartMatchups => smartMatchups}
  
  end
  
  def printResults(arg = {})
    results = getResults(arg)
    
    puts "#{results[:total].to_s.reverse.gsub(/...(?=.)/,'\&,').reverse} possibilities remain.\n\n"
    puts "validMatchupsBlackouts=#{results[:validMatchupsBlackouts]} / validMatchups=#{results[:validMatchups]} == #{(100*results[:validMatchupsBlackouts].to_f/results[:validMatchups].to_f).round(2)}"
    puts "smartMatchupsBlackouts=#{results[:smartMatchupsBlackouts]} / smartMatchups=#{results[:smartMatchups]} == #{(100*results[:smartMatchupsBlackouts].to_f/results[:smartMatchups].to_f).round(2)}"
    puts "smartMatchupsBlackouts=#{results[:smartMatchupsBlackouts]} / validMatchups=#{results[:validMatchups]} == #{(100*results[:smartMatchupsBlackouts].to_f/results[:validMatchups].to_f).round(2)}"
  
    w = (@guys+@girls).max_by do |e|
      e.length
    end
    w = w.length
    w = 6 if w < 6
    
    header = ' ' * w       
    (0...@cardinality).each do |x|
      header << '|' + sprintf("%#{w}s", @guys[x]) 
    end
    
    header << "\n" + ('-' * w)
    (0...@cardinality).each do |x|
      header << '+' + sprintf("%#{w}s", ('-' * w))
    end
    
    header << "\n"
    
    (0...@cardinality).each do |y|
      header << sprintf("%#{w}s", @girls[y])       
      (0...@cardinality).each do |x|
        header << '|' + sprintf("%#{w}.1f", (100.0 * results[:results][x][y].to_f)/results[:total]) 
      end
      
      header << "\n" + ('-' * w)
      (0...@cardinality).each do |x|
        header << '+' + sprintf("%#{w}s", ('-' * w))
      end
      
      header << "\n"
    end
    
    puts header
  end
  

end



require './season5.rb'

puts S5.printResults()


*/



#endif
























