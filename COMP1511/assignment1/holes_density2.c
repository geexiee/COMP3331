//This is a variation of finding hole density where the total area is the area of the hole plus the area of the number, rather than the area of the bounding box. This is specifically for numbers with especially  large or small holes like 0 or 4 to differentiate it from other numbers
#include <stdio.h>
#include "captcha.h"
#include <math.h>
double get_holes_density2(int height, int width, int holes_array[height][width]) {
//Floodfill the outside of the digit so remaining 0s are holes
	floodFill(0, 0, height, width, holes_array);
	floodFill(0, width - 1, height, width, holes_array);
	floodFill(height - 1, 0, height, width, holes_array);
	floodFill(height - 1, width - 1, height, width, holes_array);
	floodFill((int) floor(height/2), 0, height, width, holes_array);
	floodFill((int) floor(height/2), width-1, height, width, holes_array);

//iterate through array, counting number of 0s (hole area) and number of 1s (number area)
	int hole_area = 0, number_area = 0;
	for (int row = 0; row < height; row++) {
		for (int column = 0; column < width; column++) {
			if (holes_array[row][column] == 0) {
				hole_area++;
			}
			if (holes_array[row][column] == 1) {
				number_area++;
			}
		}
	}

	int total_area = hole_area+number_area;
	return (double)hole_area/total_area;
}
