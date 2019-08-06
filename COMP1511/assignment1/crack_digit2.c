//This code was written and provided by Andrew Taylor in the week 7 lab exercises
#include <stdio.h>
#include "captcha.h"

void analyze_image(int height, int width, int pixels[height][width]);
int identify_digit(int holes, double hole_position, double holes_density, double vertical_balance, double horizontal_balance, double density, double tallness);

int main(int argc, char *argv[]) {
    int height, width;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <image-file>\n", argv[0]);
        return 1;
    }

    // first get image dimensions
    if (get_pbm_dimensions(argv[1], &height, &width) != 1) {
        return 1;
    }

    // now declare appropriate array
    int pixels[height][width];
    if (read_pbm(argv[1], height, width, pixels)) {
        analyze_image(height, width, pixels);
    }
    return 0;
}

// extract digit bounding box from image
// extract and declare holes_array
// determine attributes

void analyze_image(int height, int width, int pixels[height][width]) {
    int start_row, start_column, box_width, box_height;
	get_bounding_box(height, width, pixels, &start_row, &start_column, &box_height, &box_width);

    int box_pixels[box_height][box_width];
    copy_pixels(height, width, pixels, start_row, start_column, box_height, box_width, box_pixels);

	int holes_array[height][width];
	for (int a = 0; a < height; a++) {
		for (int b = 0; b < width; b++) {
			holes_array[a][b] = pixels[a][b];
		}
	}

    double horizontal_balance = get_horizontal_balance(box_height, box_width, box_pixels);
	double vertical_balance = get_vertical_balance(box_height, box_width, box_pixels);
	double density = get_density(box_height, box_width, box_pixels);
	double holes_density = get_holes_density(height, width, holes_array);
	int holes = get_holesf(height, width, holes_array);
	double tallness = get_tallness(box_height, box_width, box_pixels);
	double hole_position = get_hole_position(box_height, box_width, box_pixels);
	double digit = identify_digit(holes, holes_density, vertical_balance, horizontal_balance, density, tallness, hole_position);

	printf("%f\n", digit);
}

// identify digit based on combination of attributes

int identify_digit(int holes, double hole_position, double holes_density, double vertical_balance, double horizontal_balance, double density, double tallness) {
	return hole_position;
}
