#include <stdio.h>
int main(void) {
int numbers[10];
	for (int i = 0; i < 10; i++) {
		scanf("%d", &numbers[i]);
	}
	printf("Odd numbers were: ");
	for (int i = 0; i < 10; i++) {
		if (numbers[i]%2 != 0) {
			printf("%d ", numbers[i]);
		}
	}
	printf("\n");
	printf("Even numbers were: ");
	for (int i = 0; i < 10; i++) {
		if (numbers[i]%2 == 0) {
		printf("%d ", numbers[i]);
		}
	}
	printf("\n");
	return 0;
}
