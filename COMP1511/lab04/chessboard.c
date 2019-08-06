#include <stdio.h>

int main(void) {
	int size;
	int i = 1;
	printf("Enter size: ");
	scanf("%d", &size);
	while (i <= size*size) {
		if (i%2 == 0) {
			printf("*");
		}
		else {
			printf("-");
		}
		if (i%size 	== 0) {
			printf("\n");
		}		
		i = i + 1;	
	}
	return 0;
}
