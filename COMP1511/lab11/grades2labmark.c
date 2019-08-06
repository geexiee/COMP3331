#include <stdio.h>
#include <string.h>

double grades2labmark(char grades[]);
double grades2labmark(char grades[]) {
	double sum = 0;
	for (int i = 0; i <= strlen(grades); i++) {
		if (grades[i] == 'A') {
			sum = sum + 1;
		}
		else if (grades[i] == 'B') {
			sum = sum + 0.8;
		}
		else if (grades[i] == 'C') {
			sum = sum + 0.5;
		}
		else if (grades[i] == '+') {
			sum = sum + 0.2;
		}
		else if (grades[i] == '.') {
			sum = sum + 0;
		}
	}

	if (sum > 10) {
		return 10;
	}
	return sum;
}
		
int main (int argc, char *argv[]) {
	double mark = grades2labmark(argv[1]);
	printf("%.1f\n", mark);
}
	 
