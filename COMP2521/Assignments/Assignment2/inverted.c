// Written by Kevin Luong z5164800 and Gordon Xie z5160618
// Group name googzilla_firechrome

#include <stdio.h>
#include "collection.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "inverted.h"

int main(void) {
    Collection collection = getCollection("collection.txt");
    WordList wordList = newWordList();
    int index, index2;
    Node currentNode;
    // for every URL
    for (index = 0; index < collection->size; index++) {
        currentNode = collection->nodes[index];
        // for every word in URL
        for (index2 = 0; index2 < currentNode->wordCount; index2++) {
            addURL(wordList, currentNode->words[index2], currentNode->URL);
        }
    }
    // sorting URL list for each word
    Word currentWord;
    for (index = 0; index < wordList->wordCount; index++) {
        currentWord = wordList->words[index];
        qsort(currentWord->URLs, currentWord->URLCount, sizeof(char *), compareURLs);
    }
    // sorting word list
    qsort(wordList->words, wordList->wordCount, sizeof(Word), compareWords);
    // printing results
    FILE *file = fopen(DESTINATION, "w");
    for (index = 0; index < wordList->wordCount; index++) {
        currentWord = wordList->words[index];
        fprintf(file, "%s ", currentWord->word);
        for (index2 = 0; index2 < currentWord->URLCount; index2++) {
            fprintf(file, "%s ", currentWord->URLs[index2]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    freeCollection(collection);
    freeWordList(wordList);
    return 0;
}


WordList newWordList(void) {
    WordList newList = malloc(sizeof(wordListRep));
    assert(newList != NULL);
    newList->wordCount = 0;
    newList->arraySize = INITIALSIZE;
    newList->words = malloc(INITIALSIZE * sizeof(Word));
    assert(newList->words != NULL);
    return newList;
}

Word newWord(char *word) {
    Word newWord = malloc(sizeof(wordNode));
    assert(newWord != NULL);
    snprintf(newWord->word, WORDSIZE, "%s", word);
    newWord->URLCount = 0;
    newWord->arraySize = INITIALSIZE;
    newWord->URLs = malloc(INITIALSIZE * sizeof(char *));
    assert(newWord->URLs != NULL);
    return newWord;
}

void addURL(WordList wordList, char *word, char *URL) {
    // finding word
    Word current = NULL;
    int index;
    for (index = 0; index < wordList->wordCount; index++) {
        if (strcmp(word, wordList->words[index]->word) == 0) {
            current = wordList->words[index];
            break;
        }
    }
    // word not found, creating new word
    if (current == NULL) {
        // resizing array
        if (wordList->wordCount == wordList->arraySize) {
            wordList->words = realloc(wordList->words, 2 * wordList->arraySize * sizeof(Word));
            assert(wordList->words != NULL);
            wordList->arraySize *= 2;
        }
        current = newWord(word);
        wordList->words[wordList->wordCount] = current;
        wordList->wordCount++;
    }
    // finding URL
    for (index = 0; index < current->URLCount; index++) {
        if (strcmp(current->URLs[index], URL) == 0) {
            break;
        }
    }
    // URL not found, adding
    if (index == current->URLCount) {
        // resizing array
        if (current->URLCount == current->arraySize) {
            current->URLs = realloc(current->URLs, 2 * current->arraySize * sizeof(char *));
            assert(current->URLs != NULL);
            current->arraySize *= 2;
        }
        current->URLs[current->URLCount] = malloc(WORDSIZE * sizeof(char));
        assert(current->URLs[current->URLCount] != NULL);
        snprintf(current->URLs[current->URLCount], WORDSIZE, "%s", URL);
        current->URLCount++;
    }
}

int compareWords(const void *a, const void *b) {
    Word *c = (Word *) a;
    Word *d = (Word *) b;
    char *firstWord = (*c)->word, *secondWord = (*d)->word;
    return strcmp(firstWord, secondWord);
}

int compareURLs(const void *a, const void *b) {
    char **c = (char **) a;
    char **d = (char **) b;
    return strcmp(*c, *d);
}

void freeWordNode(Word wordNode) {
    int i;
    for (i = 0; i < wordNode->URLCount; i++) {
        free(wordNode->URLs[i]);
    }
    free(wordNode->URLs);
    free(wordNode);
}

void freeWordList(WordList wordList) {
    int i;
    for (i = 0; i < wordList->wordCount; i++) {
        freeWordNode(wordList->words[i]);
    }
    free(wordList->words);
    free(wordList);
}



