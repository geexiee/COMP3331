#include <stdio.h>
#include "captcha.h"
#include <math.h>
//Find the tallness of the hole in the digit as a ratio of the box height. This is for identifying 0s since they have especially tall holes in this regard.
double get_hole_tallness(int height, int width, int pixels[height][width]) {
	int top_hole_row, bottom_hole_row, hole_height;
	double hole_height_ratio;

//First, floodfill the corners and 'middle of side columns' of the bounding box so that only the hole has 0s

	 floodFill(0, 0, height, width, pixels);
     floodFill(0, width - 1, height, width, pixels);
     floodFill(height - 1, 0, height, width, pixels);
     floodFill(height - 1, width - 1, height, width, pixels);
     floodFill((int) floor(height/2), 0, height, width, pixels);
     floodFill((int) floor(height/2), width-1, height, width, pixels);

//Find top row of hole by iterating from the top until 0 is found
	for (int a = height - 1; a >= 0; a--) {
		for (int b = 0; b < width; b++) {
			if (pixels[a][b] == 0) {
				top_hole_row = a;
				printf("g");
				break;
			}
		}
	}

//Find bottom row of hole by iterating from the bottom until 0 is found
	for (int a = 0; a < height; a++) {
		for (int b = 0; b < width; b++) {
			if (pixels[a][b] == 0) {
				bottom_hole_row = a;
				printf("k");
				break;
			}
		}
	}

	hole_height = top_hole_row - bottom_hole_row + 1;
	hole_height_ratio = hole_height/height;
	
	return hole_height_ratio;
}
