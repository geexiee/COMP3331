//This code was written and provided by Andrew Taylor in the week 7 lab exercises and then added to by myself
#include <stdio.h>
#include "captcha.h"

void analyze_image(int height, int width, int pixels[height][width]);
int identify_digit(int holes, int hole_position, double holes_density, double vertical_balance, double horizontal_balance, double density, double tallness, double hole_density2, double upper_horizontal_balance, double hole_ratio);

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
	int hole_position = get_hole_position(height, width, pixels);
	double hole_density2 = get_holes_density2(height, width, holes_array);
    double upper_horizontal_balance = get_upper_horizontal_balance(box_height, box_width, box_pixels);
	double hole_ratio = get_hole_ratio(height, width, holes_array);

	int digit = identify_digit(holes, hole_position, holes_density, vertical_balance, horizontal_balance, density, tallness, hole_density2, upper_horizontal_balance, hole_ratio);
    
	printf("%d\n", digit);
}

// identify digit based on combination of attributes

int identify_digit(int holes, int hole_position, double holes_density, double vertical_balance, double horizontal_balance, double density, double tallness, double hole_density2, double upper_horizontal_balance, double hole_ratio) {
//identify 0
	if (holes == 1) {
		if (hole_ratio > 0.35) {
//			if(0.4675<horizontal_balance<0.5305) {
//				if(0.468< vertical_balance < 0.528) {
					return 0;
//				}
//			}
		}
	
//identify 4
		else if (hole_density2 < 0.1) {
			return 4;
		}
//identify 9
//		if (0.478 < vertical_balance < 0.556) {
//			if (0.4885 < horizontal_balance < 0.5695) {
				if (hole_position == 1) {
					return 9;
				}
//			}
//		}
//identify 6
		if (hole_position == 2) {
			return 6;
		}		
	}
//identify 8
	else if (holes == 2) {
		return 8;
	}
	else if (holes == 0) {
//identify 7
//		if (0.5 < vertical_balance < 0.654) {
//			if (0.5 < upper_horizontal_balance) {
//				return 7;
//			}
//		}
//identify 2
		if (0.5 < vertical_balance < 0.654) {
			if (0.5 < upper_horizontal_balance) {
				return 2;
			}
		}
//identify 1
//		else if (0.4395 < vertical_balance < 0.5325) {
//			if (0.435 < horizontal_balance < 0.615) {
//				return 1;
//			}
//		}
//identify 3
//		else if (0.524 < horizontal_balance < 0.62) {
//			if (0.449 < vertical_balance < 0.545) {
//				return 3;
//			}
//		}	
		else {
			return 5;
		}
	}
//since my programs have most difficulty identifying 5, if none of the conditions are met it is most likely to be a 5
	return 5;
}
