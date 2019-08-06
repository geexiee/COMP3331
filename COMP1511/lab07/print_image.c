#include "captcha.h"
#include <stdio.h>
void print_image(int height, int width, int pixels[height][width])
{
	int row = height-1, col = 0;
	while(row >= 0) {	
		while (col < width) {
		    
		    if (pixels[row][col] == 1) {
			    printf("*");
		    } else {
		        printf("-");
		    }
	        col = col + 1;
	    
	    }
	    col = 0;
	    printf("\n");
		row = row - 1;
	}
}
