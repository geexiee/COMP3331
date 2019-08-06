// Graph.c ... implementation of Graph ADT
// Written by John Shepherd, May 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Graph.h"
#include "Queue.h"

// graph representation (adjacency matrix)
typedef struct GraphRep {
	int    nV;    // #vertices
	int    nE;    // #edges
	int  **edges; // matrix of weights (0 == no edge)
} GraphRep;

// check validity of Vertex
int validV(Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}

// make an edge
Edge mkEdge(Graph g, Vertex v, Vertex w)
{
	assert(g != NULL && validV(g,v) && validV(g,w));
	Edge new = {v,w}; // struct assignment
	return new;
}

// insert an Edge
// - sets (v,w) and (w,v)
void insertEdge(Graph g, Vertex v, Vertex w, int wt)
{
	assert(g != NULL && validV(g,v) && validV(g,w));
	if (g->edges[v][w] == 0) {
		g->edges[v][w] = wt;
		g->edges[w][v] = wt;
		g->nE++;
	}
}

// remove an Edge
// - unsets (v,w) and (w,v)
void removeEdge(Graph g, Vertex v, Vertex w)
{
	assert(g != NULL && validV(g,v) && validV(g,w));
	if (g->edges[v][w] != 0) {
		g->edges[v][w] = 0;
		g->edges[w][v] = 0;
		g->nE--;
	}
}

// create an empty graph
Graph newGraph(int nV)
{
	assert(nV > 0);
	int v, w;
	Graph new = malloc(sizeof(GraphRep));
	assert(new != 0);
	new->nV = nV; new->nE = 0;
	new->edges = malloc(nV*sizeof(int *));
	assert(new->edges != 0);
	for (v = 0; v < nV; v++) {
		new->edges[v] = malloc(nV*sizeof(int));
		assert(new->edges[v] != 0);
		for (w = 0; w < nV; w++)
			new->edges[v][w] = 0;
	}
	return new;
}

// free memory associated with graph
void dropGraph(Graph g)
{
	assert(g != NULL);
	// not needed for this lab
}

// display graph, using names for vertices
void showGraph(Graph g, char **names)
{
	assert(g != NULL);
	printf("#vertices=%d, #edges=%d\n\n",g->nV,g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf("%d %s\n",v,names[v]);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf("\t%s (%d)\n",names[w],g->edges[v][w]);
			}
		}
		printf("\n");
	}
}


// find a path between two vertices using breadth-first traversal
// only allow edges whose weight is less than "max"
int findPath(Graph g, Vertex src, Vertex dest, int max, int *path)
{
	assert(g != NULL);
    // edge case where src is dest
    if (src == dest) {
        path[0] = src;
        return 1;
    }
    // creating visited array
    int *visited = calloc(g->nV, sizeof(int));
    // todo queue for BFS (breadth first search)
    Queue toDo = newQueue();
    Vertex current, next;
    QueueJoin(toDo, src);
    int index, size; // used later
    // storing the shortest paths to each vertex (from src)
    int **shortestPaths = calloc(g->nV, sizeof(int *));
    for (index = 0; index < g->nV; index++) {
        shortestPaths[index] = calloc(g->nV, sizeof(int));
    }
    int *pathSizes = calloc(g->nV, sizeof(int));
    // initialising shortestPath and pathSizes
    shortestPaths[src][0] = src;
    pathSizes[src] = 1;
    // BFS
    while (!QueueIsEmpty(toDo)) {
        current = QueueLeave(toDo);
        if (visited[current]) continue;
        visited[current] = 1;
        for (next = 0; next < g->nV; next++) {
            if (g->edges[current][next] >= max || g->edges[current][next] == 0) continue;
            if (next == dest) {
                // found destination vertex, returning answer
                for (index = 0; index < pathSizes[current]; index++) {
                    path[index] = shortestPaths[current][index];
                }
                path[index] = next;
                size = index + 1;
                // freeing
                free(visited);
                dropQueue(toDo);
                free(pathSizes);
                for (index = 0; index < g->nV; index++) {
                    free(shortestPaths[index]);
                }
                free(shortestPaths);
                return size;
            }
            else if (pathSizes[next] == 0 || pathSizes[next] > (pathSizes[current] + 1)) {
                // changing shortest path to next vertex
                pathSizes[next] = pathSizes[current] + 1;
                for (index = 0; index < pathSizes[current]; index++) {
                    shortestPaths[next][index] = shortestPaths[current][index];
                }
                shortestPaths[next][index] = next;
            }
            if (!visited[next]) QueueJoin(toDo, next);
        }
    }
    // freeing
    free(visited);
    dropQueue(toDo);
    free(pathSizes);
    for (index = 0; index < g->nV; index++) {
        free(shortestPaths[index]);
    }
    free(shortestPaths);
	return 0; // no answer found, returning 0
}
