#include <stdio.h>

int main(void) {
	int n;
	printf("Enter size: \n");
	scanf("%d", &n);
	for (int row = 0; row < n; row++) {
		for (int column = 0; column < n; column++) {
			if (row == column || column == (n - row-1)) {
				printf("*");
			}
			else {
				printf("-");
			}
		}
		printf("\n");
	}
}

