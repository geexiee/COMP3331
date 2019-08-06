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
        printf("Usage: ./searchTfIdf term1 term2 ...\n");
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
    Collection collection = getCollection("collection.txt");
    URL currentURL;
    double tfidf;
    int index2;
    for (index = 0; index < collection->size; index++) {
        // summing tf-idf for all search terms for the current node
        tfidf = 0.0;
        for (index2 = 1; index2 < argc; index2++) {
            tfidf += getTFIDF(argv[index2], collection->nodes[index], collection);
        }
        currentURL = newURL(collection->nodes[index]->URL, tfidf);
        insertURL(allURLs, currentURL);
    }
    // updating matches count
    FILE *file = fopen(INVERTED, "r");
    checkFile(file);
    readInverted(file, allURLs, argc, argv);
    fclose(file);
    // sorting results
    qsort(allURLs->array, allURLs->numURLs, sizeof(URL), compareURLs);
    // printing results
    for (index = 0; index < allURLs->numURLs && index < 30; index++) {
        currentURL = allURLs->array[index];
        if (currentURL->matches == 0) break;
        printf("%s %.6f\n", currentURL->URL, currentURL->rank);
    }
    freeURLArray(allURLs);
    freeCollection(collection);
    return 0;
}

void checkFile(FILE *file) {
    if (file != NULL) return;
    printf("%s%s\n", "Input files missing. This program requires: ", INVERTED);
    exit(0);
}
