#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "DLList.h"


/* 
    You will submit only this one file.

    Implement the function "removeValue" below. Read the exam paper for 
    detailed specification and description of your task.  

    - DO NOT modify code in the file DLList.h . 
    - You can add helper functions in this file.  
    - DO NOT add "main" function in this file. 
    
*/



void removeValue(DLList L, int value){
	if (L->curr == NULL) {
		return L;
	}
	else if (L->curr->next == NULL) {
		if (L->curr->value == value) {
			free(curr);
			return;
		}
	}
	else {
		while (L->curr	!= NULL) {
			if (L->prev == NULL) {
				struct DLListNode *temp = L->curr->next;
				free(L->curr);
				L->first = temp;
			}
			else if (L->next == NULL) {
				if (L->curr->value == value) {
					L->last = L->curr->prev;
					free(L->curr);
				}
			}
			else {
				if (L->curr->value == value) {
					struct DLListnode *prv = L->curr->prev, *nxt = L->curr->next, *cur = L->curr;
					prev->next = nxt;
					nxt->prev = prv
					free(cur);
				}
			}
			L->curr = L->curr->next;
		}
	}
	return;
}



