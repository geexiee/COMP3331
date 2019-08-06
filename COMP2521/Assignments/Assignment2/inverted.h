// Written by Kevin Luong z5164800 and Gordon Xie z5160618
// Group name googzilla_firechrome

#define DESTINATION "invertedIndex.txt" // for location where input is taken. see collection.h

typedef struct wordNode {
    char word[WORDSIZE];
    int URLCount;
    char **URLs;
    int arraySize;
} wordNode;
typedef wordNode *Word;

typedef struct wordListRep {
    Word *words;
    int wordCount;
    int arraySize;
} wordListRep;
typedef wordListRep *WordList;

// functions

// Creates and returns an empty word list
WordList newWordList(void);

// Creates and returns a word node using the given string as the word
Word newWord(char *word);

// given a word list, a word, and a URL
// makes a new word if the word is not already in the list
// adds the URL to the word's list of URLS if not already in the list
void addURL(WordList wordList, char *word, char *URL);

// comparison function for qsort
// given two words, returns 0 if they are equal, 1 if they are out of order, -1 otherwise
int compareWords(const void *a,const void *b);

// comparison function for qsort for strings
// given two words, returns 0 if they are equal, 1 if they are out of order, -1 otherwise
int compareURLs(const void *a,const void *b);

// frees the data associated with a given word node
void freeWordNode(Word wordNode);

// frees the data associated with a given wordlist
void freeWordList(WordList wordList);
