// Written by Kevin Luong z5164800 and Gordon Xie z5160618
// Group name googzilla_firechrome

#define WORDSIZE 256
#define PREFIX "" // directory to take input from, leave as "" for current directory
#define INITIALSIZE 5

typedef struct collectionNode {
    char URL[WORDSIZE];
    int wordCount;
    char **words;
    int outLinkCount;
    char **outLinks;
} collectionNode;
typedef collectionNode *Node;

typedef struct collectionRep {
    int size;
    Node *nodes;
} collectionRep;
typedef collectionRep *Collection;

// external functions

// given the name of a file containing a list of URL files
// creates a collection of all the URL files
Collection getCollection(char *filename);

// frees the data associated with the given collection
void freeCollection(Collection collection);

// normalises a given string;
// leading and trailing spaces removed
// all letters converted to lowercase
// punctuation marks at the end of the string removed (. , ; ?)
void normalise(char *word);

// given a string (word), a node and the collection
// returns the tf-idf
double getTFIDF(char *string, Node node, Collection collection);

// given a string (word) and a node
// returns the number of times the word occurs in the node
int contains(char *string, Node node);
