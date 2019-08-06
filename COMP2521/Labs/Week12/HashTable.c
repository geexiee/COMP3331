
// HashTable.c ... implementation of HashTable ADT
// Written by John Shepherd, May 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "HashTable.h"
#include "List.h"

#define mix(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8);  \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12); \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5);  \
  a -= b; a -= c; a ^= (c>>3);  \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}

// Types and functions local to HashTable ADT

typedef struct HashTabRep {
	List *lists;  // either use this
	int   nslots; // # elements in array
	int   nitems; // # items stored in HashTable
} HashTabRep;

// convert key into index (from Sedgewick)
unsigned int hash(Key k, int tableSize)
{
// incorporated the mixing from the lecture notes

    unsigned int a = 0x9e3779b9, b = 0x9e3779b9, c = 3923095;
    int len = strlen(k);
    while (len >= 12) {
        a += (k[0] + (k[1] << 8) + (k[2] << 16) + (k[3] << 24));
        b += (k[4] + (k[5] << 8) + (k[6] << 16) + (k[7] << 24));
        c += (k[8] + (k[9] << 8) + (k[10] << 16) + (k[11] << 24));
        mix(a, b, c);
        k += 12; len -= 12;
    }
    while (len >= 3) {
        a += k[0];
        b += k[1];
        c += k[2];
        mix(a, b, c);
        k += 3; len -= 3;
    }
    while (*k != '\0') {
        a += k[0];
        if (k[1] != '\0') b += k[1];
        mix(a, b, c);
        k++;
    }
	return (c % tableSize);

/*
	unsigned int h = 0;
	int a = 31415, b = 27183;
	for (; *k != '\0'; k++) {
		a = a*b % (tableSize-1);
		h = (a*h + *k) % tableSize;
	}

	return (h % tableSize);
*/
}

// Interface functions for HashTable ADT

// create an empty HashTable
HashTable newHashTable(int N)
{
	HashTabRep *new = malloc(sizeof(HashTabRep));
	assert(new != NULL);
	new->lists = malloc(N*sizeof(List));
	assert(new->lists != NULL);
	int i;
	for (i = 0; i < N; i++)
		new->lists[i] = newList();
	new->nslots = N; new->nitems = 0;
	return new;
}

// free memory associated with HashTable
void dropHashTable(HashTable ht)
{
	assert(ht != NULL);
	int i;
    for (i = 0; i < ht->nslots; i++)
		dropList(ht->lists[i]);
    free(ht->lists);
	free(ht);
}

// display HashTable stats
void HashTableStats(HashTable ht)
{
	assert(ht != NULL);
	printf("Hash Table Stats:\n");
	printf("Number of slots = %d\n",ht->nslots);
	printf("Number of items = %d\n",ht->nitems);
	printf("Chain length distribution\n");
	printf("%8s %8s\n","Length","#Chains");
	// array of ints to count chain lengths
    // initial size of the array is 5, and this increases if chains with longer lengths are found
    int maxLength = 4, arraySize = 5;
    int *counter = calloc(arraySize, sizeof(int));
    assert(counter != NULL);
    // counting all chain lengths
    int index, length;
    List curr;
    for (index = 0; index < ht->nslots; index++) {
        curr = ht->lists[index];
        length = ListLength(curr);
        // increasing array size
        if (length > maxLength) {
            counter = realloc(counter, 2 * arraySize * sizeof(int));
            assert(counter != NULL);
            // initialising the new memory values to 0
            memset(&(counter[arraySize]), 0, arraySize * sizeof(int));
            arraySize *= 2;
            maxLength = arraySize - 1;
        }
        counter[length]++;
    }
    // displaying
    for (index = 0; index <= maxLength; index++) {
        if (counter[index] != 0) {
            printf("%8d %8d\n", index, counter[index]);
        }
    }
    free(counter);
}

// insert a new value into a HashTable
void HashTableInsert(HashTable ht, Item it)
{
	assert(ht != NULL);
	int i = hash(key(it), ht->nslots);
	if (ListSearch(ht->lists[i], key(it)) == NULL) {
		ListInsert(ht->lists[i], it);
		ht->nitems++;
	}
}

// delete a value from a HashTable
void HashTableDelete(HashTable ht, Key k)
{
	assert(ht != NULL);
	int h = hash(k, ht->nslots);
	ListDelete(ht->lists[h], k);
}

// get Item from HashTable using Key
Item *HashTableSearch(HashTable ht, Key k)
{
	assert(ht != NULL);
	int i = hash(k, ht->nslots);
	return ListSearch(ht->lists[i], k);
}


