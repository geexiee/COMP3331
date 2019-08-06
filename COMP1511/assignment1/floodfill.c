#include <stdio.h>
#include "captcha.h"
//Function to replace zeroes in the array with 3s using floodfill. Used for holes
void floodFill(int row, int column, int height, int width, int pixels[height][width]) {
    if (column >= 0 && column < width && row >=0 && row < height && pixels[row][column] == 0) {
        pixels[row][column] = 3;
//Floodfill spreads in eight directions, turning all found 0s into 3s and then continuing on with this process until all adjacent 0s are turned into 3s. 
        floodFill(row + 1, column, height, width, pixels);
		floodFill(row + 1, column - 1, height, width, pixels);
		floodFill(row + 1, column + 1, height, width, pixels);
        floodFill(row - 1, column, height, width, pixels);
		floodFill(row - 1, column + 1, height, width, pixels);
		floodFill(row - 1, column - 1, height, width, pixels);
        floodFill(row, column + 1, height, width, pixels);
        floodFill(row, column - 1, height, width, pixels);
     }
}
