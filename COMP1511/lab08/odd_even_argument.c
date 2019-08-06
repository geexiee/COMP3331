#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
	printf("Odd numbers were: ");
	for (int i = 1; i < argc; i++) {
		if (atoi(argv[i])%2 != 0) {
			printf("%s ", argv[i]);
		}
	}
	printf("\n");

	printf("Even numbers were: ");
	for (int i = 1; i < argc; i++) {
		if (atoi(argv[i])%2 == 0) {
			printf("%s ", argv[i]);
		}
	}
	printf("\n");
}
