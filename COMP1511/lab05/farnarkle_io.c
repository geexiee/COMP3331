#include <stdio.h>
#include "farnarkle.h"

// read N_TILES tiles into array tiles
// return 1 if successful, 0 otherwise
int read_tiles(int tiles[N_TILES]) {
	int i = 0;	
	while (i < N_TILES) {		
		if (scanf("%d", &tiles[i]) != 1) {
			return 0;
		}
		i = i + 1;
	}
    return 1; 
}

// print tiles on a single line
void print_tiles(int tiles[N_TILES]) {
	int i = 0;	
	while (i < (N_TILES)) {		
		printf("%d ", tiles[i]);
		i = i + 1;
	}
	printf("\n");
}

