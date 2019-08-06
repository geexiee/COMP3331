#include <stdio.h>

int main(void) {
	int n, row, column;
	printf("Enter size: ");
	scanf("%d", &n);
	row = 0;
	while(row < n) {
	column = 0;	
		while(column < n) {
			if(column == ((n/2) + 1/2)) {
				printf("*");
			}			
			else if (row == ((n/2) + 1/2)){
				printf("*");
			}
			else {
				printf("-");
			}
		column = column + 1;
		}
	printf("\n");	
	row = row + 1;
	}				
}
