#include <stdio.h>
int main(void) {
void increment(int *n);

int i = 1;
increment(&i);
printf("%d", i);

return 0;
}
void increment(int *n) {
	*n = *n + 1;
}
