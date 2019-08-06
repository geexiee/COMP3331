#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[]) {
	char ch;
	int i;
	while ( (ch = getchar()) != EOF) {
		if ( ch >= 65 && ch <= 90) {
			for (i=0; i<26; i++){
				if (ch%65 == i) {
					ch = argv[1][i];
					}
				}
		}				
		if (ch >= 97 && ch <= 122) {
			for (i=0; i<26; i++) {
				if (ch%97 == i) {
					ch = argv[1][i];
				}
			}
		}
	}
	putchar(ch);
return 0;
}
