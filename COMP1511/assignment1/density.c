#include <stdio.h>
#include "captcha.h"

//find the density of black pixels within the bounding box by iterating through the array and incrementing n_black_pixels when a 1 is found, thereby counting the total area of black pixels. 
double get_density(int height, int width, int pixels[height][width]) {
	 int n_black_pixels = 0, row, column, area = (height * width);
     for (row = 0; row < height; row++) {
		for (column = 0; column < width; column++) {
			if (pixels[row][column] == 1) {
				n_black_pixels++;
			}
	    }
    }
    return (double)n_black_pixels/area; //divide black pixel area by box area to find density of black pixels
}
