// Written by Kevin Luong z5164800 and Gordon Xie z5160618
// Group name googzilla_firechrome

// Contains functions to handle URL datatype
#include <stdio.h>
#include <stdlib.h>
#include "collection.h"
#include <assert.h>
#include <string.h>
#include <math.h>

// internal functions ----------------------------------------------------------

// creates a new, empty collection with given size number of URLs
Collection newCollection(int size);

// creates a new node and inserts the data obtained from the given filename
Node newNode(char *filename);

// frees the given node
void freeNode(Node node);

// -----------------------------------------------------------------------------

Collection getCollection(char *filename) {
    // opening file containing URL list
    char buffer[WORDSIZE];
    snprintf(buffer, WORDSIZE, "%s%s", PREFIX, filename);
    FILE *file = fopen(buffer, "r");
    if (file == NULL) {
        printf("Could not open collection.txt.\n");
        exit(0);
    }
    // finding total number of URLs
    int size = 0;
    while(fscanf(file, "%s", buffer) != EOF) size++;
    // making collection
    Collection collection = newCollection(size);
    // getting URL data
    rewind(file);
    int index = 0;
    while(fscanf(file, "%s", buffer) != EOF) {
        collection->nodes[index++] = newNode(buffer);
// debugging code
/*
        printf("------------------\n");
        printf("(%s)\n", collection->nodes[index - 1]->URL);
        printf("words: %d outlinks: %d\n", collection->nodes[index - 1]->wordCount, collection->nodes[index - 1]->outLinkCount);
        int j = 0;
        while (j < collection->nodes[index - 1]->wordCount) {
            printf("(%s)", collection->nodes[index - 1]->words[j]);
            j++;
        }
        printf("\n");
        for (j = 0; j < collection->nodes[index - 1]->outLinkCount; j++) {
            printf("(%s)", collection->nodes[index - 1]->outLinks[j]);
        }
        printf("\n");
        printf("------------------\n");
*/
// end debugging code
    }
    fclose(file);
    return collection;
}

Collection newCollection(int size) {
    assert(size > 0);
    Collection collection = malloc(sizeof(collectionRep));
    assert(collection != NULL);
    // initialising size and array of nodes
    collection->size = size;
    collection->nodes = malloc(size * sizeof(Node));
    assert(collection->nodes != NULL);

    return collection;
}

void freeCollection(Collection collection) {
    assert(collection != NULL);
    int index;
    for (index = 0; index < collection->size; index++) {
        freeNode(collection->nodes[index]);
    }
    free(collection->nodes);
    free(collection);
}

Node newNode(char *URLname) {
    // opening file
    char filename[WORDSIZE];
    snprintf(filename, WORDSIZE, "%s%s%s", PREFIX, URLname, ".txt");
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open %s\n", filename);
        exit(0);
    }
    // making node
    Node node = malloc(sizeof(collectionNode));
    assert (node != NULL);
    snprintf(node->URL, WORDSIZE, "%s", URLname);
    // entering outlink info
    char buffer[WORDSIZE];
    int arraySize = INITIALSIZE;
    node->outLinkCount = 0;
    node->outLinks = malloc(arraySize * sizeof(char *));
    assert(node->outLinks != NULL);
    while (fscanf(file, "%s", buffer) != EOF) {
        if (strcmp(buffer, "#start") == 0) {
            fscanf(file, "%s", buffer);
            assert(strcmp(buffer, "Section-1") == 0);
            continue;
        }
        if (strcmp(buffer, "#end") == 0) {
            fscanf(file, "%s", buffer);
            assert(strcmp(buffer, "Section-1") == 0);
            break;
        }
        if (strcmp(buffer, node->URL) == 0) continue;
        if (node->outLinkCount == arraySize) {
            node->outLinks = realloc(node->outLinks, arraySize * 2 * sizeof(char *));
            assert(node->outLinks != NULL);
            arraySize *= 2;
        }
        node->outLinks[node->outLinkCount] = malloc(WORDSIZE * sizeof(char));
        assert(node->outLinks[node->outLinkCount] != NULL);
        snprintf(node->outLinks[node->outLinkCount], WORDSIZE, "%s", buffer);
        node->outLinkCount++;
    }
    // entering words
    arraySize = INITIALSIZE;
    node->wordCount = 0;
    node->words = malloc(arraySize * sizeof(char *));
    assert(node->words != NULL);
    while (fscanf(file, "%s", buffer) != EOF) {
        if (strcmp(buffer, "#start") == 0) {
            fscanf(file, "%s", buffer);
            assert(strcmp(buffer, "Section-2") == 0);
            continue;
        }
        if (strcmp(buffer, "#end") == 0) {
            fscanf(file, "%s", buffer);
            assert(strcmp(buffer, "Section-2") == 0);
            break;
        }
        if (node->wordCount == arraySize) {
            node->words = realloc(node->words, arraySize * 2 * sizeof(char *));
            assert(node->words != NULL);
            arraySize *= 2;
        }
        node->words[node->wordCount] = malloc(WORDSIZE * sizeof(char));
        assert(node->words[node->wordCount] != NULL);
        normalise(buffer);
        snprintf(node->words[node->wordCount], WORDSIZE, "%s", buffer);
        node->wordCount++;
    }
    fclose(file);
    return node;
}

void freeNode(Node node) {
    int index;
    for (index = 0; index < node->wordCount; index++) {
        free(node->words[index]);
    }
    for (index = 0; index < node->outLinkCount; index++) {
        free(node->outLinks[index]);
    }
    free(node->words);
    free(node->outLinks);
    free(node);
}

void normalise(char *word) {
    assert(word != NULL);
    // removing leading spaces
    int index = 0, index2;
    while (word[index] == ' ') index++; // finding first non-space character
    for (index2 = 0; index < WORDSIZE; index2++) {
        word[index2] = word[index];
        index++;
    }
    // removing trailing spaces and punctuation
    for (index = 0; index < WORDSIZE; index++) {
        if (word[index] == '\0') break; // finding end of string
    }
    while (word[index] == '\0' || word[index] == ' ' || word[index] == '.' || word[index] == ',' || word[index] == ';' || word[index] == '?') {
        word[index] = '\0';
        index--;
    }
    // converting all letters to lowercase
    for (index = 0; index < WORDSIZE; index++) {
        if (word[index] == '\0') break; // end of string
        if (word[index] >= 'A' && word[index] <= 'Z') {
            word[index] = 'a' + (word[index] - 'A');
        }
    }
}

double getTFIDF(char *string, Node node, Collection collection) {
    assert(string != NULL && node != NULL && collection != NULL);
    // finding term frequency
    int frequency = contains(string, node);
    // finding document frequency
    int docFreq = 0, index;
    for (index = 0; index < collection->size; index++) {
        if (contains(string, collection->nodes[index])) docFreq++;
    }
    // returning tf-idf
    double tfidf = ((1.0 * frequency) / node->wordCount) * (log((1.0 * collection->size)/docFreq)/log(10));
    return tfidf;
}

int contains(char *string, Node node) {
    int frequency = 0, index;
    for (index = 0; index < node->wordCount; index++) {
        if (strcmp(string, node->words[index]) == 0) {
            frequency++;
        }
    }
    return frequency;
}


