#include <stdio.h>

int main(void) {
	int numbers[10];
	for (int i = 0; i < 10; i++) {
		scanf("%d", &numbers[i]);
	}
	printf("Numbers were: ");
	for (int i = 0; i < 10; i++) {
		printf("%d ", numbers[i]);
	}
	printf("\n");	
	return 0;
}
