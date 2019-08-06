#include <stdio.h>
#include "captcha.h"
//Find the number of holes in a digit
int get_holesf(int height, int width, int pixels[height][width]) {
	int holes_array[height][width];

//Copy pixels array to holes_array and use holes_array for holes related programs so if I need to refer back to pixels, the outside 0s won't be filled with 3s (since im changing them to 3s using floodfill)
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			holes_array[i][j] = pixels[i][j];
		}
	}

//floodfill starting from the four corners of the image, turning all 0s found into 3s. This is assuming that I'm not using the bounding box and that the digit won't start in any of the four corners.
	floodFill(0, 0, height, width, holes_array);
	floodFill(0, width - 1, height, width, holes_array);
	floodFill(height - 1, 0, height, width, holes_array);
	floodFill(height - 1, width - 1, height, width, holes_array);
	
//Iterate through the entire array after floodfilling the corners. When it finds a remaining zero, that means it must be a hole. Therefore, increment holes by 1 and then floodfill that space to turn the 0s into 3s to prevent overcouning the holes. 
	int holes = 0;
	for (int row = 0; row < height; row++) {
		for (int column = 0; column < width; column++) {
			if (holes_array[row][column] == 0) {
				holes ++;
				floodFill(row, column, height, width, holes_array);
			}
		}
	}
//holes = number of holes in the digit.
	return holes;
}
		

