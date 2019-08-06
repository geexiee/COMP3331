//This code was written and provided by Andrew Taylor in the week 7 lab exercises
#include "captcha.h"

// find horizontal balance point (centre of gravity) of image

double get_horizontal_balance(int height, int width, int pixels[height][width]) {
    double column_sum = 0;
    int n_black_pixels = 0;
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            if (pixels[row][column] == 1) {
                column_sum = column_sum + column;
                n_black_pixels = n_black_pixels + 1;
            }
        }
    }
    return (column_sum/n_black_pixels + 0.5)/width;
}
