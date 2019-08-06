#include "captcha.h"
//Find the tallness (height/width) of the digit
double tallness;
double get_tallness(int height, int width, int pixels[height][width]) {
	tallness = height/width;
	return tallness;
}
