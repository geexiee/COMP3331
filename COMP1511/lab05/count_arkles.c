#include <stdio.h>
#include "farnarkle.h"

// return number of arkles
int count_arkles(int sequence1[N_TILES], int sequence2[N_TILES]) {
int i = 0
int a = 0 
int n = 0   
	while(i < N_TILES && a < N_TILES) {
		if(sequence1(i) == sequence2(a)) {
			n = n+1 
		}	
		i = i + 1, a = a + 1
	}   
	 return 42; 
}
