// crawl.c ... build a graph of part of the web
// Written by John Shepherd, September 2015
// Uses the cURL library and functions by Vincent Sanders <vince@kyllikki.org>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <curl/curl.h>
#include "stack.h"
#include "queue.h"
#include "set.h"
#include "graph.h"
#include "html.h"
#include "url_file.h"

#define BUFSIZE 1024

void setFirstURL(char *, char *);
void normalise(char *, char *, char *, char *, int);

// given a URL
// returns 1 if the URL is not inside UNSW
int notAllowed(char *URL) {
    char copy[BUFSIZE];
    strcpy(copy, URL);
    if (strstr(copy, "unsw.edu.au") != NULL) return 0;
    return 1;
}

// given an array of chars
// sets all chars to NULL
void BZERO(char *array, int size) {
    int i;
    for (i = 0; i < size; i++) {
        array[i] = '\0';
    }
}

int main(int argc, char **argv)
{
	URL_FILE *handle;
	char buffer[BUFSIZE];
	char baseURL[BUFSIZE];
	char firstURL[BUFSIZE];
	char next[BUFSIZE];
	int  maxURLs;

	if (argc > 2) {
		strcpy(baseURL,argv[1]);
		setFirstURL(baseURL,firstURL);
		maxURLs = atoi(argv[2]);
		if (maxURLs < 40) maxURLs = 40;
	}
	else {
		fprintf(stderr, "Usage: %s BaseURL MaxURLs\n",argv[0]);
		exit(1);
	}
		

	// add firstURL to the ToDo list
    Queue toDo = newQueue();
    enterQueue(toDo, firstURL);
	// initialise Graph to hold up to maxURLs
    Graph graph = newGraph(maxURLs);
	// initialise set of Seen URLs
    Set seen = newSet();
    insertInto(seen, firstURL);
	// while (ToDo list not empty and Graph not filled) {
    char *nextURL = NULL;
    URL_FILE *URLFILE;
    char html[BUFSIZE];
    while (emptyQueue(toDo) != 1 && nVertices(graph) < maxURLs) {
        printf ("a\n");
	//    grab Next URL from ToDo list
        nextURL = leaveQueue(toDo);
        URLFILE = url_fopen(nextURL, "r");
	//    if (not allowed) continue
        if (notAllowed(nextURL)) continue;
	//    foreach line in the opened URL {
        int pos;
        char result[BUFSIZE];
        while (url_fgets(html, BUFSIZE, URLFILE) != NULL) {
            pos = 0;
            //BZERO(result, BUFSIZE);
            memset (result, 0, BUFSIZE);
            while ((pos = GetNextURL(html, nextURL, result, pos)) > 0) {
                if (nVertices(graph) < maxURLs || (isElem(seen, nextURL) && isElem(seen, result))) {
                    addEdge(graph, nextURL, result);
                }
                if (!isElem(seen, result)) {
                    insertInto(seen, result);
                    enterQueue(toDo, result);
                }
                //BZERO(result, BUFSIZE);
                memset(result, 0, BUFSIZE);
            }
        }
        url_fclose(URLFILE);
        sleep(1);
    }
    showGraph(graph, 0);
	if (!(handle = url_fopen(firstURL, "r"))) {
		fprintf(stderr,"Couldn't open %s\n", next);
		exit(1);
	}
	while(!url_feof(handle)) {
		url_fgets(buffer,sizeof(buffer),handle);
		//fputs(buffer,stdout);
		int pos = 0;
		char result[BUFSIZE];
		memset(result,0,BUFSIZE);
		while ((pos = GetNextURL(buffer, firstURL, result, pos)) > 0) {
			printf("Found: '%s'\n",result);
			memset(result,0,BUFSIZE);
		}
	}
	url_fclose(handle);
	sleep(1);
	return 0;
}

// setFirstURL(Base,First)
// - sets a "normalised" version of Base as First
// - modifies Base to a "normalised" version of itself
void setFirstURL(char *base, char *first)
{
	char *c;
	if ((c = strstr(base, "/index.html")) != NULL) {
		strcpy(first,base);
		*c = '\0';
	}
	else if (base[strlen(base)-1] == '/') {
		strcpy(first,base);
		strcat(first,"index.html");
		base[strlen(base)-1] = '\0';
	}
	else {
		strcpy(first,base);
		strcat(first,"/index.html");
	}
}
