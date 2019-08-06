#include <stdio.h>
#include "captcha.h"
//Find the vertical balance point of an image
double get_vertical_balance(int height, int width, int pixels[height][width]) {
    double row_sum = 0;
    int n_black_pixels = 0;
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            if (pixels[row][column] == 1) {
                row_sum = row_sum + row;
                n_black_pixels = n_black_pixels + 1;
            }
        }
    }
    return (row_sum/n_black_pixels + 0.5)/height;
}
