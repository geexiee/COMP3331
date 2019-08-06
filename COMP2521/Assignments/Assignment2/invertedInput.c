// Written by Kevin Luong z5164800 and Gordon Xie z5160618
// Group name googzilla_firechrome

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "invertedInput.h"
#include <ctype.h>
#include <assert.h>

URLArray newURLArray() {
    URLArray newArray = malloc(sizeof(URLArrayRep));
    assert(newArray != NULL);
    newArray->numURLs = 0;
    newArray->array = malloc(INITIALSIZE * sizeof(URL));
    assert(newArray->array != NULL);
    newArray->arraySize = INITIALSIZE;
    return newArray;
}

void freeURLArray(URLArray rep) {
    int index;
    for (index = 0; index < rep->numURLs; index++) {
        freeURL(rep->array[index]);
    }
    free(rep->array);
    free(rep);
}

void insertURL(URLArray rep, URL node) {
    assert(rep != NULL && node != NULL);
    if (rep->arraySize == rep->numURLs) {
        rep->array = realloc(rep->array, 2 * rep->arraySize * sizeof(URL));
        assert(rep->array != NULL);
        rep->arraySize *= 2;
    }
    rep->array[rep->numURLs] = node;
    rep->numURLs++;
}

int compareURLs(const void *a, const void *b) {
    URL *c = (URL *) a;
    URL *d = (URL *) b;
    if ((*c)->matches > (*d)->matches) {
        return -1;
    }
    else if ((*c)->matches < (*d)->matches) {
        return 1;
    }
    else {
        if ((*c)->rank > (*d)->rank) {
            return -1;
        }
        if ((*c)->rank > (*d)->rank) {
            return 1;
        }
        else {
            return 0;
        }
    }
}

URL newURL(char *URLName, double rank) {
    URL newURL = malloc(sizeof(URLNode));
    assert(newURL != NULL);
    snprintf(newURL->URL, WORDSIZE, "%s", URLName);
    newURL->rank = rank;
    newURL->matches = 0;
    return newURL;
}

void freeURL(URL url) {
    free(url);
}

void readInverted(FILE *file, URLArray allURLs, int argc, char **argv) {
    int start, skip, index, index2;
    char term[WORDSIZE], buffer[WORDSIZE], URLName[WORDSIZE];
    // reading line by line
    while (fgets(buffer, WORDSIZE - 1, file) != NULL) {
        skip = 0;
        start = 0;
        for (index = 0; buffer[index] != '\0'; index++) {
            if (skip == 1) break; // flag to skip unneeded terms
            if (buffer[index] == ' ') {
                // first term is the search term
                if (start == 0) {
                    strncpy(term, &(buffer[start]), index - start);
                    term[index - start] = '\0'; // ending the string
                    start = index + 1;
                    // checking to ensure the term is a search term
                    skip = 1;
                    for (index2 = 0; index2 < argc; index2++) {
                        if (strcmp(argv[index2], term) == 0) {
                            skip = 0;
                            break;
                        }
                    }
                }
                // getting URLs for the search term
                else {
                    strncpy(URLName, &(buffer[start]), index - start);
                    URLName[index - start] = '\0'; // ending the string
                    start = index + 1;
                    // going through list of URLs and updating matching count
                    for (index2 = 0; index2 < allURLs->numURLs; index2++) {
                        if (strcmp(allURLs->array[index2]->URL, URLName) == 0) allURLs->array[index2]->matches++;
                    }
                }
            }
        }
    }
}
