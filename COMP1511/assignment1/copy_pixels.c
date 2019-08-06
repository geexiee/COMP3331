//This code was written and provided by Andre Taylor in the week 7 lab exercises
#include <stdio.h>
#include "captcha.h"

// copy  rectangle of pixels specified by parameters start_row, start_column, copy_height, copy_width
// from array pixels to array copy

void copy_pixels(int height, int width, int pixels[height][width], int start_row, int start_column, int copy_height, int copy_width, int copy[copy_height][copy_width]) {
    for (int row = 0; row < copy_height; row++) {
        for (int column = 0; column < copy_width; column++) {
            copy[row][column] = pixels[start_row + row][start_column + column];
        }
    }
}
