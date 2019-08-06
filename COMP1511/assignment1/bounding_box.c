//This code was written and provided by Andrew Taylor in the week 7 Lab solutions
#include "captcha.h"

// find minimum rectangle containing all black pixels in image
void get_bounding_box(int height, int width, int pixels[height][width], int *start_row, int *start_column, int *box_height, int *box_width) {
    int min_row = height - 1;
    int max_row = 0;
    int min_column = width - 1;
    int max_column = 0;
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            if (pixels[row][column] == 1) {
                if (row < min_row) {
                    min_row = row;
                }
                if (row > max_row) {
                    max_row = row;
                }
                if (column < min_column) {
                    min_column = column;
                }
                if (column > max_column) {
                    max_column = column;
                }
            }
        }
    }
    if (min_row <= max_row) {
        *start_row = min_row;
        *box_height = max_row -  min_row + 1;
        *start_column = min_column;
        *box_width = max_column -  min_column + 1;
    } else {
        // no black pixels in image
        *start_row = 0;
        *box_height = 0;
        *start_column = 0;
        *box_width = 0;
    }
}		
