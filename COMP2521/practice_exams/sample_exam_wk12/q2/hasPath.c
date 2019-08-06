
/* hasPath.c 
   Written by Ashesh Mahidadia, October 2017
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Graph.h"



/* 
    You will submit only this one file.

    Implement the function "hasPath" below. Read the exam paper for 
    detailed specification and description of your task.  

    - You can add helper functions in this file, if required.  

    - DO NOT modify code in the file BSTree.h . 
    - DO NOT add "main" function in this file. 
*/
int *visited;
void check(Graph g, Vertex v);

int hasPath(Graph g, Vertex src, Vertex dest)
{
	if (adjacent(g, src, dest) == 1) {
		return 1;
	}
	visited = calloc(g->nV, sizeof(int));
	check(g, src);
	if (visited[dest] == 1) {
		return 1;
	}
	return 0;
}

void check(Graph g, Vertex v) {
	int i;
	visited[v] = 1;
	for (i = 0; i < g->nV; i++) {
		if (!g->edges[v][i]) {
			continue;
		}
		if (visited[i] == 0) {
			check(g, i);
		}
	}
}

























/*void dfs(Graph g, Vertex v);

int *visited;  // array of visited

int hasPath(Graph g, Vertex src, Vertex dest)
{
    int i;
    visited = malloc(g->nV*sizeof(int));
    for (i = 0; i < g->nV; i++) {
        visited[i] = -1;
    }
    dfs(g, src);
    if (visited[dest] == 1) {
        return 1;
    } else {
        return 0;
    }
}

void dfs(Graph g, Vertex v) {
    visited[v] = 1;
    Vertex w;
    for (w = 0; w < g->nV; w++) {
        if (!g->edges[v][w]) continue;
        if (visited[w] == -1) dfs(g, w);
    }
}*/
