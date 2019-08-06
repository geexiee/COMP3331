#include <stdio.h>
int main(void) {
	int numbers[1000];
	int n = 0;
	for (int i = 0; i < 1000; i++) {
		n++;
		scanf("%d", &numbers[i]);
		if (numbers[i] < 0) {
			break;
		}
	}
	printf("Odd numbers were: ");
	for (int i = 0; i < n-1; i++) {
		if (numbers[i]%2 != 0) {
			printf("%d ", numbers[i]);
		}
	}
	printf("\n");

	printf("Even numbers were: ");
	for (int i = 0; i < n-1; i++) {
		if (numbers[i]%2 == 0) {
			printf("%d ", numbers[i]);
		}
	}
	printf("\n");
	return 0;
}
