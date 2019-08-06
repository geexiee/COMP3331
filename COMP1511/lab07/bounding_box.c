#include <stdio.h>
#include "captcha.h"
void get_bounding_box(int height, int width, int pixels[height][width],
	int *start_row, int *start_column, int *box_height, int *box_width);
int row, column, min_row, max_row, min_column, max_column;
min_row = height -1;
max_row = 0;
min_column = width -1;
max_column = 0

for (row = 0; row < height; row++) {
	for (column = 0; column < width; column++) {
		if (pixels[row][column] == 1) {	
			if (row <= min_row) {
				min_row = row;
			}
			if (row >= max_row) {
				max_row = row;
			}
			if (column <= min_column) {
				min_column = column;
			}
			if (column >= max_column) {
				max_column = column;
			}
		}
	}
}

if (min_row <= max_row) {
	start_row = min_row;
	}
if (min_column <= max_column) {
	start_column = min_column;
	}

box_height = max_row - min_row + 1;
box_width = max_column - min_column + 1;

printf("Bounding box height %d pixels, width %d pixels\n", box_height, box_width);
printf("Bounding box bottom left corner is row %d column %d \n", start_row, start_column);

