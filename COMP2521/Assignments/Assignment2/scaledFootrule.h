// Written by Kevin Luong z5164800 and Gordon Xie z5160618
// Group name googzilla_firechrome

#define INITIALSIZE 5
#define WORDSIZE 256
#define MATCH -1

typedef struct rankList {
    int size;
    int arraySize;
    char **pages;
} rankList;
typedef rankList *RankList;

typedef struct aggregateRep {
    int pageCount;
    char **pages; // array containing the union of the pages found in the rank lists
    int pagesArraySize;

    int listCount;
    RankList *lists;
    int listsArraySize;
} aggregateRep;
typedef aggregateRep *Aggregate;

// given a filename
// opens the file and returns the rank list in that file
RankList getRankList(char *filename);

// frees the data associated with given rank list
void freeRankList(RankList list);

// returns an empty aggregate of rank lists
Aggregate newAggregate();

// frees the data associated with given aggregate
void freeAggregate(Aggregate aggregate);

// inserts the given rank list into the given aggregate
void insertRankList(Aggregate aggregate, RankList list);

// given an element number, a position number, and an aggregate of rank lists
// returns the sum of the scaled footrule distances of placing that element in that position
double weight(int element, int position, Aggregate aggregate);

// given a 2x2 array of doubles
// prints the array
void printMatrix(double **matrix, int size);

// given a 2x2 positive matrix
// finds a minimum cost assignment for the matrix, and returns it
// sets the cells corrresponding to each match to the MATCH value
double minAssign(double **matrix, int size);

// given a 2x2 positive matrix
// performs row and column operations to reduce the matrix, ensuring every row and every column has at least one zero
void reduce(double **matrix, int size);

// given a 2x2 positive matrix, and arrays to store lines
// returns the minimum number of lines required to cover every zero in the matrix
// changes the corresponding rows and columns of the given line arrays to the iteration value
int minLines(double **matrix, int size, int *rowLines, int *colLines, int iteration);

// given a 2x2 matrix and an entry, and two integer pointers
// sets the value of the integer pointers to the number of zeroes in the entry's row and column, respectively
void countZeroes(double **matrix, int size, int row, int col, int *rowNum, int *colNum);

// given a 2x2 positive matrix, arrays containing lines, and an iteration value (to identify lines)
// finds the smallest uncovered value, and subtracts that value from all uncovered entries in the matrix
// adds the subtracted value to entries covered by two lines
void moreZero(double **matrix, int size, int *rowLines, int *colLines, int iteration);













