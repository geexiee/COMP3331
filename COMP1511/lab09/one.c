#include <stdio.h>
#define max_characters 4096

int main(void) {
    int word[max_characters]
    printf("Enter a string: ");
    if (fgets(word, max_characters, stdin) != NULL) {
        fputs(word, stdout);
        printf("%s, word);
    }
    return 0;
}
