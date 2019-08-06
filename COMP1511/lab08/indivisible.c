#include <stdio.h>
int main(int argc, char *argv[]) {
	int numbers[1000];
	int n = 0;
	while (scanf("%d", &numbers[n++]) != EOF) {}

	printf("Indivisible numbers: ");
	for (int i = 0; i < n-1; i++) {
		int shit = 0;
		for (int b = 0; b < n-1; b++) {
			if (numbers[i]%numbers[b] == 0 && i != b) {
				shit = 1;
			}
		}
		if (shit == 0) {
			printf("%d ", numbers[i]);
		}
	}
	printf("\n");
	
			
	return 0;
}
