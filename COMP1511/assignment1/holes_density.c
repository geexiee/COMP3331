#include <stdio.h>
#include "captcha.h"
#include <math.h>
//Find the density of the hole in the image relative to the bounding box
double get_holes_density(int height, int width, int holes_array[height][width]) {
//floodfill the four corners plus the middle of the first and last column (since the bounding box may have a black pixel in the corners, leading to extra holes being found as the floodfill cannot reach the 0s in between the corners for digits like 6.
	floodFill(0, 0, height, width, holes_array);
	floodFill(0, width - 1, height, width, holes_array);
	floodFill(height - 1, 0, height, width, holes_array);
	floodFill(height - 1, width - 1, height, width, holes_array);
	floodFill((int) floor(height/2), 0, height, width, holes_array);
	floodFill((int) floor(height/2), width-1, height, width, holes_array);

//iterate through the array, remaining zeroes counted by incrementing hole_area	
	int hole_area = 0;
	for (int row = 0; row < height; row++) {
		for (int column = 0; column < width; column++) {
			if (holes_array[row][column] == 0) {
				hole_area++;
			}
		}
	}
	
//total area of bounding box = box_height * box width. Therefore, hole density = hole_area/total_area
	int total_area = height*width;
	return (double)hole_area/total_area;
}

