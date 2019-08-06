// Written by Kevin Luong z5164800 and Gordon Xie z5160618
// Group name googzilla_firechrome

#include <stdio.h>
#include "collection.h"
#include <stdlib.h>
#include <assert.h>
#include "connection.h"
#include <math.h>
#include <string.h>
#include "pagerank.h"

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("Usage: ./pagerank d diffPR maxIterations\n");
        return 0;
    }
    double d = atof(argv[1]), diffPR = atof(argv[2]);
    int maxIterations = atoi(argv[3]);
    assert(d > 0.0 && diffPR > 0.0 && maxIterations > 0);

    // code converted from pseudocode in assignment specification
    Collection collection = getCollection("collection.txt");
    int **connections = getConnections(collection);
    int n = collection->size;
    // N by 2 array to store current and previous pageranks
    double *pageRanks[2];
    int arrayIndex;
    for (arrayIndex = 0; arrayIndex < 2; arrayIndex++) {
        pageRanks[arrayIndex] = malloc(n * sizeof(double));
    }
    // initialising pageRanks
    int index;
    for (index = 0; index < n; index++) {
        pageRanks[0][index] = 1.0/n;
    }
    // looping
    int iteration = 0, prevArrayIndex = 0, i, j;
    arrayIndex = 1;
    double diff = diffPR, sum;
    while (iteration < maxIterations && diff >= diffPR) {
        iteration++;
        for (i = 0; i < n; i++) {
            sum = 0.0;
            for (j = 0; j < n; j++) {
                if (connections[j][i]) {
                    sum += pageRanks[prevArrayIndex][j] * weightIn(j, i, connections, n) * weightOut(j, i, connections, n);
                }
            }
            pageRanks[arrayIndex][i] = ((1.0 - d)/n) + (d * sum);
        }
        diff = 0.0;
        for (i = 0; i < n; i++) {
            diff += fabs(pageRanks[arrayIndex][i] - pageRanks[prevArrayIndex][i]);
        }
        prevArrayIndex = arrayIndex;
        if (arrayIndex == 1) arrayIndex = 0;
        else arrayIndex = 1;
    }
    // copying results array (to not forget which pageRank corresponds to which URL)
    PRN pageRankResults[n];
    for (i = 0; i < n; i++) {
        pageRankResults[i] = newPRN(pageRanks[prevArrayIndex][i], i);
    }
    // sorting results
    qsort(pageRankResults, n, sizeof(PRN), comparePRN);
    // printing results
    FILE *file = fopen(DESTINATION, "w");
    for (i = 0; i < n; i++) {
        j = pageRankResults[i]->index;
        fprintf(file, "%s, %d, %.7f\n", collection->nodes[j]->URL, connections[j][collection->size], pageRankResults[i]->rank);
    }
    // freeing data
    fclose(file);
    for (i = 0; i < n; i++) {
        freePRN(pageRankResults[i]);
    }
    free(pageRanks[0]);
    free(pageRanks[1]);
    freeConnections(connections, collection->size);
    freeCollection(collection);
    return 0;
}

PRN newPRN(double rank, int index) {
    PRN newNode = malloc(sizeof(pageRankNode));
    assert(newNode != NULL);
    newNode->rank = rank;
    newNode->index = index;
    return newNode;
}

void freePRN(PRN node) {
    free(node);
}

int comparePRN(const void *a, const void *b) {
    PRN *c = (PRN *) a;
    PRN *d = (PRN *) b;
    if ((*c)->rank > (*d)->rank) return -1;
    if ((*c)->rank < (*d)->rank) return 1;
    return 0;
}


