#include <stdio.h>

int main(void) {
	int a, b, original, n, i;
	int isPrime = -1;
	printf("Enter number: ");
	scanf("%d", &n);
	original = n;
	i = 2;
	while (i < n) {
		if (n%i == 0) {
			isPrime = isPrime + 1;	
		}
		i = i + 1;
	}

	if (isPrime == -1) {
		printf("%d is prime\n", n);
	}
	else {
		printf("The prime factorization of %d is:\n", n);
		i = 2;	
		while (i <= n){	
			if(n%i == 0) {
				if(n == i) {
					printf("%d = %d\n", i, original);
				}
				else {
					printf("%d * ", i);
				}					
				n = n/i;
			}				
			else {
				i = i + 1;
			}					
		}
	 }
	return 0;
}
