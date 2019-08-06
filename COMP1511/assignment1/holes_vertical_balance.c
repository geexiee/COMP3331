#include <stdio.h>
#include "captcha.h"
#include <math.h>
//Find the position of a hole in a digit
int get_hole_position(int height, int width, int pixels[height][width]) {

	floodFill(0, 0, height, width, pixels);
	floodFill(0, width - 1, height, width, pixels);
	floodFill(height - 1, 0, height, width, pixels);
	floodFill(height - 1, width - 1, height, width, pixels);
	floodFill((int) floor(height/2), 0, height, width, pixels);
	floodFill((int) floor(height/2), width-1, height, width, pixels);
	
	int up_pixels = 0, down_pixels = 0, n_white_pixels = 0;
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            if (pixels[row][column] == 0 && row > (int) floor(height/2)) {
				up_pixels++; //count the number of white pixels (holes) that are above halfway of the number
				n_white_pixels++;
			}
			else if (pixels[row][column] == 0 && row < (int) floor(height/2)) {
				down_pixels++; //count the number of white pixels that are below halfway up the number
				n_white_pixels++;
			}
        }
    }
	if (n_white_pixels == 0) {
		return 9999;
	}
	else if (up_pixels >= down_pixels) {
		return 1; // returning 1 means the hole is on the top side of the number
	}	
	else if (down_pixels > up_pixels) {
		return 2; //returning 2 mean that the hole is on the bottom side of the number
	}
	return 69;
}
