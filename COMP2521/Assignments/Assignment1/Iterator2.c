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
	IteratorG newIterator = malloc(sizeof(newIterator));

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
	new = malloc(sizeof(Node));
	assert(new != NULL);
	new->value = vl;
	new->prev = new->next = NULL;
	return new;
}

IteratorG IteratorGNew(ElmCompareFp cmp, ElmNewFp copy, ElmFreeFp free);

int add(IteratorG it, void *vp) {
	it->lastused = NULL;
	Node *new_node = newNode(vp), *nxt = it->cursor->next, *prv = it->cursor->prev;
	if (it->nitems == 0) {
		it->first = new_node = it->last = it->cursor;
		new_node->prev = NULL;
		new_node->next = NULL;
		return 1;
	}
	else if (it->cursor = it->first) {
		new_node->next = it->first;
		new_node->prev = new_node;
		it->first = new_node;
		return 1;
	}
	else {
		return 0;
	}
}

int hasNext(IteratorG it) {
	assert(it != NULL);
	if (it->cursor->next != NULL) {
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
	Node *nxt = it->cursor->next;
	//case for if cursor is before first node
	if (it->cursor->prev == NULL) {
		it->cursor->prev = it->first;
		it->cursor->next = it->first->next;
		it->lastused = it->cursor->prev; //store this node in lastused for delete/set functions
		return it->cursor->prev->value;
	}
	//case for if cursor is between 2 nodes
	else if (it->cursor->prev != NULL && it->cursor->next != NULL) {
		it->cursor->next = nxt->next;
		it->cursor->prev = nxt;
		nxt->next = it->cursor;
		nxt->next->prev = it->cursor;
		it->lastused = it->cursor->prev;
		return it->cursor->prev->value;
	}
	//case for if cursor is at the end of the list
	else if (it->cursor->prev != NULL && it->cursor->next == NULL) {
		return NULL;
	}
	return NULL;
}

void * previous(IteratorG it) {
	assert(it != NULL);
	Node *prv = it->cursor->prev;
    //case for if cursor is before first node
    if (it->cursor->prev == NULL) {
        return NULL;
	}
    //case for if cursor is between 2 nodes
    else if (it->cursor->prev != NULL && it->cursor->next != NULL) {
        it->cursor->next = prv;
        it->cursor->prev = prv->prev;
		prv->prev->next = it->cursor;
		prv->prev = it->cursor;
		it->lastused = it->cursor->next;
        return it->cursor->next->value;
    }
    //case for if cursor is at the end of the list
    else if (it->cursor->prev != NULL && it->cursor == NULL) {
		it->cursor = it->last;
		return it->last->data;
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
		return 1;
	}
	//case for if lastused is at last node
	else if (it->lastused == it->last) {
		Node *temp = it->last;
		it->last->next = NULL;
		it->cursor = it->last->next;
		free(temp);
		return 1;
	}
	//all other cases
	else {
		Node *temp = it->lastused, *prv = it->lastused->prev, *nxt = it->lastused->next;
		prv->next = nxt;
		nxt->prev = prv;
		if (it->lastused == it->cursor) {
			it->cursor = it->cursor->next;
		}
		free(temp);
		return 1;
	}
	return 0;
}

int set(IteratorG it, void * vp) {
	assert (it != NULL);
	if (it->lastused == NULL) {
		printf("last call was not next/prev\n");
		return 0;
	}
	else if (it->lastused != NULL) {
		free(it->lastused->value);
		it->lastused->value = vp;
		return 1;
	}
	return 0;
}

void * findNext(IteratorG it, void * vp) {
	Node *nxt = it->cursor->next;
	//case for if cursor is at the end of the list
	if (it->cursor->next == NULL) {
		return NULL;
	}
	//Iterate through list until next value is equal to vp
	while (it->cursor->next->value != vp && it->cursor->next != NULL) {
		it->cursor = it->cursor->next;
	}
	it->cursor->next = nxt->next;
	it->cursor->prev = nxt;
	nxt->next->prev = it->cursor;
	nxt->next = it->cursor;
	it->lastused = it->cursor->prev;
	return it->cursor->prev->value;

}

void * findPrevious(IteratorG it, void * vp) {
    Node *prv = it->cursor->prev;
    //case for if cursor is at the front of the list
    if (it->cursor->prev == NULL) {
        return NULL;
    }
    //Iterate through list backwards until next value is equal to vp
    while (it->cursor->prev->value != vp && it->cursor->prev != NULL) {
        it->cursor = it->cursor->prev;
    }
	//Now the next (left) node is the one where value = vp
    it->cursor->next = prv;
    it->cursor->prev = prv->prev;
    prv->prev->next = it->cursor;
    prv->prev = it->cursor;
	it->lastused = it->cursor->next;
    return it->cursor->next->value;
}

void reset(IteratorG it) {
	//Iterate through list until the cursor is at the front
	while (it->cursor->prev != NULL) {
		it->cursor = it->cursor->prev;
	}
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
