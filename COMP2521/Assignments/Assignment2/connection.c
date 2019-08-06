// Written by Kevin Luong z5164800 and Gordon Xie z5160618
// Group name googzilla_firechrome

// contains functions to assist with calculating weighted pagerank
#include <stdio.h>
#include <stdlib.h>
#include "collection.h"
#include <assert.h>
#include <string.h>

int **getConnections(Collection collection) {
    assert(collection != NULL);
    // creating matrix
    int **connections = malloc((collection->size + 1) * sizeof(int *));
    assert(connections != NULL);
    int index;
    for (index = 0; index <= collection->size; index++) {
        connections[index] = calloc(collection->size + 1, sizeof(int));
        assert(connections[index] != NULL);
    }
    // inserting connections
    int index2, index3;
    Node node;
    // for every node
    for (index = 0; index < collection->size; index++) {
        node = collection->nodes[index];
        // for every outlink
        for (index2 = 0; index2 < node->outLinkCount; index2++) {
            // going through nodes to find the one linked to
            for (index3 = 0; index3 < collection->size; index3++) {
                if (index3 == index) continue;
                if (strcmp(collection->nodes[index3]->URL, node->outLinks[index2]) == 0) {
                    connections[index][index3] = 1;
                    break;
                }
            }
        }
        connections[index][collection->size] = node->outLinkCount;
    }
    // counting number of inlinks
    int total;
    for (index = 0; index < collection->size; index++) {
        total = 0;
        for (index2 = 0; index2 < collection->size; index2++) {
            if (connections[index2][index]) total++;
        }
        connections[collection->size][index] = total;
    }
// testing
/*
    for (index = 0; index <= collection->size; index++) {
        for (index2 = 0; index2 <= collection->size; index2++) {
            printf("%d ", connections[index][index2]);
        }
        printf("\n");
    }
*/
// end testing
    return connections;
}

void freeConnections(int **connections, int size) {
    int i;
    for (i = 0; i <= size; i++) {
        free(connections[i]);
    }
    free(connections);
}

double weightIn(int v, int u, int **connections, int size) {
    if (connections[v][u] == 0) return 0.0; // return 0 if there is no link
    double uInLinks = connections[size][u];
    double sum = 0.0;
    int index;
    for (index = 0; index < size; index++) {
        if (connections[v][index] == 1 && v != index) {
            sum += connections[size][index];
        }
    }
    return uInLinks/sum;
}

double weightOut(int v, int u, int **connections, int size) {
    if (connections[v][u] == 0) return 0.0; // return 0 if there is no link
    double uOutLinks = connections[u][size];
    if (uOutLinks == 0) uOutLinks = 0.5;
    double sum = 0.0;
    int index;
    for (index = 0; index < size; index++) {
        if (connections[v][index] == 1 && v != index) {
            sum += connections[index][size];
            if (connections[index][size] == 0) {
                sum += 0.5; // to stop division by 0 errors
            }
        }
    }
    return uOutLinks/sum;
}



