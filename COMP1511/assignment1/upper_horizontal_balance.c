#include <stdio.h>
#include "captcha.h"
#include <math.h>
// find horizontal balance point (centre of gravity) of the top half of an image

double get_upper_horizontal_balance(int height, int width, int pixels[height][width]) {
    double column_sum = 0;
    int n_black_pixels = 0;
    for (int row = (int) floor(height/2); row < height; row++) { //makes it iterate through rows from halfway up the image
        for (int column = 0; column < width; column++) {
            if (pixels[row][column] == 1) {
                column_sum = column_sum + column;
                n_black_pixels = n_black_pixels + 1;
            }
        }
    }
    return (column_sum/n_black_pixels + 0.5)/width;
}
