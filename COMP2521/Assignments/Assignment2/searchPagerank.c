// Written by Kevin Luong z5164800 and Gordon Xie z5160618
// Group name googzilla_firechrome

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "invertedInput.h"
#include <ctype.h>

// error message printer
void checkFile(FILE *file);

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: ./searchPagerank term1 term2 ...\n");
        return 0;
    }
    // normalising the argv array
    int index;
    for (index = 1; index < argc; index++) {
        normalise(argv[index]);
    }
    // creating array of all URLs
    URLArray allURLs = newURLArray();
    // loading all URLs
    FILE *file = fopen(PRL, "r");
    checkFile(file);
    char buffer[WORDSIZE], URLName[WORDSIZE];
    double PR;
    index = 0;
    URL currentURL;
    while (fscanf(file, "%s", buffer) != EOF) {
        if (index % 3 == 1) { // skip number of outlinks
            index++;
            continue;
        }
        if (index % 3 == 0) { // getting URL name
            snprintf(URLName, WORDSIZE, "%s", buffer);
            normalise(URLName);
            index++;
            continue;
        }
        PR = atof(buffer);
        currentURL = newURL(URLName, PR);
        insertURL(allURLs, currentURL);
        index++;
    }
    fclose(file);
    // updating matches count
    file = fopen(INVERTED, "r");
    checkFile(file);
    readInverted(file, allURLs, argc, argv);
    fclose(file);
    // sorting results
    qsort(allURLs->array, allURLs->numURLs, sizeof(URL), compareURLs);
    // printing results
    for (index = 0; index < allURLs->numURLs && index < 30; index++) {
        currentURL = allURLs->array[index];
        if (currentURL->matches == 0) break;
        printf("%s\n", currentURL->URL);
    }
    freeURLArray(allURLs);
    return 0;
}

void checkFile(FILE *file) {
    if (file != NULL) return;
    printf("%s%s %s\n", "Input files missing. This program requires: ", INVERTED, PRL);
    exit(0);
}
