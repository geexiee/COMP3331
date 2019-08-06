#include <stdio.h>
int main(void) {
	int numbers[1000];
	int n = 0;
	int a = 0;
	while (scanf("%d", &numbers[a++]) != EOF) {
	n++;		
	}
	
	printf("Odd numbers were: ");
	for (int i = 0; i < n; i++) {
		if (numbers[i]%2 != 0) {
			printf("%d ", numbers[i]);
		}
	}
	printf("\n");

	printf("Even numbers were: ");
	for (int i = 0; i < n; i++) {
		if (numbers[i]%2 == 0) {
			printf("%d ", numbers[i]);
		}
	}
	printf("\n");
	return 0;
}
