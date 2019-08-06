// Written by Kevin Luong z5164800 and Gordon Xie z5160618
// Group name googzilla_firechrome

#define DESTINATION "pagerankList.txt" // for location where input is taken. see collection.h

typedef struct pageRankNode {
    double rank;
    int index;
} pageRankNode;
typedef pageRankNode *PRN;

// creates and returns a new pageRankNode with given values
PRN newPRN(double rank, int index);

// frees the data associated with given pageRankNode
void freePRN(PRN node);

// comparison function for qsort
int comparePRN(const void *a,const void *b);
