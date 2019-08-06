#include <stdio.h>
#include "farnarkle.h"

// return number of farnarkles
int count_farnarkles(int sequence1[N_TILES], int sequence2[N_TILES]) {
    int i = 0;
	int n = 0;
	while(i < N_TILES) {
		if(sequence1[i] == sequence2[i]) {
		n = n + 1;
		}
	i = i + 1;
	}
	return n; 
	
}

