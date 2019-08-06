// Written by Kevin Luong z5164800 and Gordon Xie z5160618
// Group name googzilla_firechrome

#include "collection.h"

#define LINESIZE 1024
#define INVERTED "invertedIndex.txt"
#define PRL "pagerankList.txt"

typedef struct URLNode {
    char URL[WORDSIZE];
    int matches;
    double rank;
} URLNode;
typedef URLNode *URL;

typedef struct URLArrayRep {
    URL *array;
    int arraySize;
    int numURLs;
} URLArrayRep;
typedef URLArrayRep *URLArray;

// creates and returns a new URLArray
URLArray newURLArray();

// frees the data associated with given URLArray
void freeURLArray(URLArray array);

// inserts the given URL into the given URL array
void insertURL(URLArray array, URL node);

// comparison function for qsort
int compareURLs(const void *a, const void *b);

// creates and returns a new URL node with given information
URL newURL(char *URLName, double rank);

// frees the data associated with given URL node
void freeURL(URL url);

// given an opened inverted index file, an array of URLs, and the argv array of search terms
// reads the given file and updates the number of search terms matched to each URL
void readInverted(FILE *file, URLArray allURLs, int argc, char **argv);
