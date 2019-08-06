//This code was provided in the week 7 lab exercises and then added to
int read_pbm(char filename[], int height, int width, int pixels[height][width]);
int get_pbm_dimensions(char filename[], int *height, int *width);
void print_image(int height, int width, int pixels[height][width]);
void get_bounding_box(int height, int width, int pixels[height][width],
                  int *start_row, int *start_column, int *box_height, int *box_width);
void copy_pixels(int height, int width, int pixels[height][width],
                 int start_row, int start_column, int copy_height, int copy_width,
                 int copy[copy_height][copy_width]);
double get_horizontal_balance(int height, int width, int pixels[height][width]);
double get_horizontal_balance(int height, int width, int pixels[height][width]);
double get_vertical_balance(int height, int width, int pixels[height][width]);
double get_density(int height, int width, int pixels[height][width]);
double get_tallness(int height, int width, int pixels[height][width]);
double get_holes_density(int height, int width, int holes_array[height][width]);
int get_holesf(int height, int width, int pixels[height][width]);
void floodFill(int row, int column, int height, int width, int pixels[height][width]);
int get_hole_position(int height, int width, int pixels[height][width]);
double get_holes_density2(int height, int width, int holes_array[height][width]);
double get_upper_horizontal_balance(int height, int width, int pixels[height][width]);
double get_hole_ratio(int height, int width, int holes_array[height][width]);
