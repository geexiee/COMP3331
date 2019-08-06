#include <stdio.h>

int main(void) {
	int n, a, total = 0, b;
	printf("Enter number: ");
	scanf("%d", &n);
	printf("The factors of %d are:\n", n);
	a = 1;
	while (a <= n) {
		if (n%a == 0 ) {
			printf("%d\n", a);
		total = total + a;
		b = total - n;
		}
	a = a + 1;
	} 
	printf("Sum of factors = %d\n", total);
	if (n == b) {
		printf("%d is a perfect number\n", n);
	}
	else if (n != b) {
		printf("%d is not a perfect number\n", n);
	}
	
	return 0;
}
