/* listIteratorG.c ... Generic List Iterator implementation
   Written by Gordon Xie (z5160618 )


*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "listIteratorG.h"

typedef struct Node {
	void  *value;
	struct Node *next;
	struct Node *prev;
} Node;

typedef struct IteratorGRep {
	
	ElmCompareFp cmp;
	ElmNewFp copy;
	ElmFreeFp free;

	int nitems; //number of items in list	
	Node *first; //First node
	Node *cursor; //Pointer to the cursor which points to next
	Node *last; //Last node
	Node *lastused; //pointer to last thing returned by prev/next

} IteratorGRep;

IteratorG IteratorGNew(ElmCompareFp cmp, ElmNewFp copy, ElmFreeFp free) {
	IteratorG newIterator = malloc(sizeof *newIterator);
	
	newIterator->cmp = cmp;
	newIterator->copy = copy;
	newIterator->free = free;
	newIterator->nitems = 0;

	newIterator->first = NULL;
	newIterator->last = NULL;
	newIterator->cursor = NULL;
	newIterator->lastused = NULL;
	
	return newIterator;
}
//Create a new iterator node
static Node *newNode(void *vl)
{
	Node *new;
	new = malloc(sizeof(new));
	assert(new);
	new->value = vl;
	new->prev = new->next = NULL;
	return new;
}

int add(IteratorG it, void *vp) {
	it->lastused = NULL;
	//case for if list is empty
	if (it->nitems == 0) {
		Node *new_node = newNode(vp);
		new_node->prev = NULL;
		new_node->next = NULL;
		it->first = new_node = it->last = it->cursor;
		it->nitems++;
		return 1;
	}
	//case for if cursor is at end of list
	else if (it->cursor == NULL) {
		Node *new_node = newNode(vp);
		new_node->prev = it->last;
		new_node->next = NULL;
		it->last = new_node;
		it->nitems++;
		return 1;	
	}	
	//case for if cursor is at front of list
	else if (it->cursor == it->first) {
		Node *new_node = newNode(vp);
		new_node->next = it->first;
		it->first->prev = new_node;
		new_node->prev = NULL;
		it->first = new_node;
		it->nitems++;
		return 1;
	}
	//all other cases
	else {
		Node *new_node = newNode(vp), *prv = it->cursor->prev;
		new_node->next = it->cursor;
		new_node->prev = prv;
		prv->next = new_node;
		it->cursor->prev = new_node;
		it->nitems++;
		return 1;
	}
	return 0;
}

int hasNext(IteratorG it) {
	assert(it != NULL);
	if (it->cursor != NULL) {
		return 1;
	}
	else {
		return 0;
	}
}

int hasPrevious(IteratorG it) {
	assert(it != NULL);
	if (it->cursor->prev != NULL) {
		return 1;
	}
	else {
		return 0;
	}
}

void * next(IteratorG it) {
	assert(it != NULL);
	//case for if cursor is at the end of the list
    if (it->cursor == NULL) {
    return NULL;
    }
	//case for if cursor is between 2 nodes
	else if (it->cursor->prev != NULL && it->cursor != NULL) {
		it->lastused = it->cursor;
		it->cursor = it->cursor->next;
		return it->lastused->value;
	}
	//case for if cursor is at front of list
    else if (it->cursor->prev == NULL) {
        it->cursor->prev = it->first;
        it->cursor = it->first->next;
        it->lastused = it->cursor->prev; //store this node in lastused for delete/set functions
        return it->cursor->prev->value;
    }
	return NULL;
}

void * previous(IteratorG it) {
	assert(it != NULL);
    //case for if cursor is at end of list
	if (it->cursor == NULL) {
		it->cursor = it->last;
        it->lastused = it->cursor;
        return it->cursor->value;
    }
    //case for if cursor is between 2 nodes
    else if (it->cursor->prev != NULL && it->cursor != NULL) {
        it->cursor = it->cursor->prev;
		it->lastused = it->cursor;
        return it->cursor->value;
    }
    //case for if cursor is at front of the list
    else if (it->cursor->prev == NULL) {
        return NULL;
    }
	return NULL;
}

int delete(IteratorG it) {
	assert(it != NULL);
	if (it->lastused == NULL || it->nitems == 0) {	//Check if last call to iterator was either next/prev/findnext/findprev and it was successful
		printf("last call was not next/prev\n");
		return 0;
	}
	//case for if lastused is first node
	else if (it->lastused == it->first) {
		Node *temp = it->first;
		it->first = it->first->next;
		it->first->prev = NULL;
		it->cursor = it->first;
		free(temp);
		it->lastused = NULL;
		it->nitems--;
		return 1;
	}
	//case for if lastused is at last node
	else if (it->lastused == it->last || it->lastused->next == NULL) {
		Node *temp = it->last;
		it->last = it->last->prev;
		it->last->next = NULL;
		it->cursor = NULL;
		free(temp);
		it->lastused = NULL;
		it->nitems--;
		return 1;
	}
	//all other cases
	else {
		Node *temp = it->lastused;
		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;
		if (it->lastused == it->cursor) {
			it->cursor = it->cursor->next;
		}
		free(temp);
		it->lastused = NULL;
		it->nitems--;
		return 1;
	}
	return 0;
}

int set(IteratorG it, void * vp) {
	assert (it != NULL);
	//make sure that last call was prev/next
	if (it->lastused == NULL) {
		printf("last call was not next/prev\n");
		return 0;
	}
	else if (it->lastused != NULL) {
		free(it->lastused->value);
		it->lastused->value = vp;
		it->lastused = NULL;
		return 1;
	}
	return 0;
}

void * findNext(IteratorG it, void * vp) {
	assert(it != NULL);
	Node *curr = it->cursor;
	//Iterate through list until cursor (next) value is equal to vp
	while (curr != NULL) {
		if (it->cmp(curr->value, vp) == 0) {
			it->cursor = curr->next;
			it->lastused = curr;
			return curr;
		}
		curr = curr->next;
	}
	return it->lastused = NULL;	
}

void * findPrevious(IteratorG it, void * vp) { 
	assert(it != NULL);
    Node *curr = it->cursor;
	while (curr != NULL) {
		if (it->cmp(curr->prev->value, vp) == 0) {
			it->cursor = curr->prev;
			it->lastused = it->cursor;
			return it->cursor;
		}
		curr = curr->prev;		
	}
	return it->lastused = NULL;
}

void reset(IteratorG it) {
	assert(it != NULL);
	it->cursor = it->first;
	it->lastused = NULL;
	return;
}

void freeIt(IteratorG it) {
    assert(it != NULL);
    Node *curr, *prev;
    curr = it->first; 
    while (curr != NULL) {
        prev = curr;
        curr = curr->next;
        free(prev->value);
        free(prev);
    }
    free(it);
}

