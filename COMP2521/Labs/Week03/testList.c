// testList.c - testing DLList data type
// Written by John Shepherd, March 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "DLList.h"

int main(int argc, char *argv[])
{
	DLList myList;
	myList = getDLList(stdin);
	putDLList(stdout,myList);
	assert(validDLList(myList));
	if (DLListLength(myList) != 0) {
		printf("Current line: %s\nNumber of lines: %d\n", DLListCurrent(myList), DLListLength(myList));
	}
	else {
		printf("This list is empty dude!\n");
	}
	if (atoi(argv[1]) == 0) {
		printf("Now inserting line before current line\n");
		DLListBefore(myList, "Random line that was added using DLListBefore");
	}
	if (atoi(argv[1]) == 1) {
		printf("Now inserting after current line\n");
		DLListAfter(myList, "Random line that was added using DLListAfter");
	}
	if (atoi(argv[1]) == 2) {
		printf("delete current line\n");
		DLListDelete(myList);
	}
	putDLList(stdout, myList);
	assert(validDLList(myList));
	return EXIT_SUCCESS;
	return 0;
}
