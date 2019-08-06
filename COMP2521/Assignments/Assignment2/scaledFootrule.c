// Written by Kevin Luong z5164800 and Gordon Xie z5160618
// Group name googzilla_firechrome

#include "scaledFootrule.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: ./scaledFootrule file1 file2 ...\n");
        return 0;
    }
    Aggregate aggregate = newAggregate();
    RankList list;
    int index;
    for (index = 1; index < argc; index++) {
        list = getRankList(argv[index]);
        insertRankList(aggregate, list);
    }
    // no lists given
    if (aggregate->pageCount == 0) return 0;
    // making matrix
    int index2;
    double **matrix = malloc(aggregate->pageCount * sizeof(double *));
    assert(matrix != NULL);
    for (index = 0; index < aggregate->pageCount; index++) {
        matrix[index] = malloc(aggregate->pageCount * sizeof(double));
        assert(matrix[index] != NULL);
        for (index2 = 0; index2 < aggregate->pageCount; index2++) {
            matrix[index][index2] = weight(index, index2, aggregate);
        }
    }
// making test matrix
/*
    double array0[] = {10,19,8,15,19};
    double array1[] = {10,18,7,17,19};
    double array2[] = {13,16,9,14,19};
    double array3[] = {12,19,8,18,19};
    double array4[] = {14,17,10,19,19};
    matrix[0] = array0;
    matrix[1] = array1;
    matrix[2] = array2;
    matrix[3] = array3;
    matrix[4] = array4;
    aggregate->pageCount = 5;
    printMatrix(matrix, aggregate->pageCount);
    printf("\n");
*/
//
    // finding minimum assignment
    double minDistance = minAssign(matrix, aggregate->pageCount);
    // printing result
    printf("%.6f\n", minDistance);
    int x, y;
    for (x = 0; x < aggregate->pageCount; x++) {
        for (y = 0; y < aggregate->pageCount; y++) {
            if (matrix[y][x] == MATCH) {
                printf("%s\n", aggregate->pages[y]);
                break;
            }
        }
    }

// printing aggregate
/*
    int index998, index999;
    RankList printList;
    printf("Union of pages: ");
    for (index998 = 0; index998 < aggregate->pageCount; index998++) {
        printf("%s ", aggregate->pages[index998]);
    }
    printf("\nLists:\n");
    for (index998 = 0; index998 < aggregate->listCount; index998++) {
        printList = aggregate->lists[index998];
        for (index999 = 0; index999 < printList->size; index999++) {
            printf("%s ", printList->pages[index999]);
        }
        printf("\n");
    }
*/
//
// printing matrix
//    printf("%lf\n", minAssign(matrix, aggregate->pageCount));
//    printMatrix(matrix, aggregate->pageCount);
//

    for (index = 0; index < aggregate->pageCount; index++) {
        free(matrix[index]);
    }
    free(matrix);
    freeAggregate(aggregate);
    return 0;
}

RankList getRankList(char *filename) {
    assert(filename != NULL);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open %s\n", filename);
        exit(0);
    }
    // making new rank list
    RankList newList = malloc(sizeof(rankList));
    assert(newList != NULL);
    newList->size = 0;
    newList->arraySize = INITIALSIZE;
    newList->pages = malloc(INITIALSIZE * sizeof(char *));
    assert(newList->pages != NULL);
    // inserting list elements
    char buffer[WORDSIZE];
    char *word;
    while (fscanf(file, "%s", buffer) != EOF) {
        word = malloc(WORDSIZE * sizeof(char));
        assert (word != NULL);
        snprintf(word, WORDSIZE, "%s", buffer);
        // resizing array
        if (newList->size == newList->arraySize) {
            newList->pages = realloc(newList->pages, 2 * newList->arraySize * sizeof(char *));
            assert(newList->pages != NULL);
            newList->arraySize *= 2;
        }
        newList->pages[newList->size] = word;
        newList->size++;
    }
    fclose(file);
    return newList;
}

void freeRankList(RankList list) {
    assert(list != NULL);
    int index;
    for (index = 0; index < list->size; index++) {
        free(list->pages[index]);
    }
    free(list->pages);
    free(list);
}

Aggregate newAggregate() {
    Aggregate new = malloc(sizeof(aggregateRep));
    assert(new != NULL);

    new->pageCount = 0;
    new->pages = malloc(INITIALSIZE * sizeof(char *));
    assert(new->pages != NULL);
    new->pagesArraySize = INITIALSIZE;

    new->listCount = 0;
    new->lists = malloc(INITIALSIZE * sizeof(RankList));
    assert(new->lists != NULL);
    new->listsArraySize = INITIALSIZE;

    return new;
}

void freeAggregate(Aggregate aggregate) {
    assert(aggregate != NULL);
    int index;
    for (index = 0; index < aggregate->pageCount; index++) {
        free(aggregate->pages[index]);
    }
    free(aggregate->pages);
    for (index = 0; index < aggregate->listCount; index++) {
        freeRankList(aggregate->lists[index]);
    }
    free(aggregate->lists);
    free(aggregate);
}

void insertRankList(Aggregate aggregate, RankList list) {
    assert(aggregate != NULL && list != NULL);
    // inserting list
    if (aggregate->listCount == aggregate->listsArraySize) {
        aggregate->lists = realloc(aggregate->lists, 2 * aggregate->listsArraySize * sizeof(RankList));
        assert(aggregate->lists != NULL);
        aggregate->listsArraySize *= 2;
    }
    aggregate->lists[aggregate->listCount] = list;
    aggregate->listCount++;
    // updating union of pages
    int index, index2, found;
    char *word;
    for (index = 0; index < list->size; index++) {
        found = 0;
        for (index2 = 0; index2 < aggregate->pageCount; index2++) {
            if (strcmp(aggregate->pages[index2], list->pages[index]) == 0) {
                found = 1;
                break;
            }
        }
        // page not found in search, updating union
        if (found == 0) {
            // updating array size
            if (aggregate->pageCount == aggregate->pagesArraySize) {
                aggregate->pages = realloc(aggregate->pages, 2 * aggregate->pagesArraySize * sizeof(char *));
                assert(aggregate->pages != NULL);
                aggregate->pagesArraySize *= 2;
            }
            word = malloc(WORDSIZE * sizeof(char));
            assert(word != NULL);
            snprintf(word, WORDSIZE, "%s", list->pages[index]);
            aggregate->pages[aggregate->pageCount] = word;
            aggregate->pageCount++;
        }
    }
}

double weight(int element, int position, Aggregate aggregate) {
    assert(element < aggregate->pageCount && position < aggregate->pageCount);
    double positionFrac = (1.0 * position + 1) / aggregate->pageCount;
    int index, listPos;
    double sum = 0.0;
    RankList list;
    // summing up the distance to each list
    for (index = 0; index < aggregate->listCount; index++) {
        list = aggregate->lists[index];
        // finding the element in the list
        for (listPos = 0; listPos < list->size; listPos++) {
            if (strcmp(aggregate->pages[element], list->pages[listPos]) == 0) break;
        }
        if (listPos == list->size) continue; // element not found, continue on to next list
        sum += fabs(((1.0 * listPos + 1.0) / list->size) - positionFrac);
    }
    return sum;
}

void printMatrix(double **matrix, int size) {
    int x, y;
    for (y = size - 1; y >= 0; y--) {
        for (x = 0; x < size; x++) {
            printf("%.6f ", matrix[y][x]);
        }
        printf("\n");
    }

}

double minAssign(double **originalMatrix, int size) {
    // uses the hungarian method to find a minimum cost assignment
    // see http://www.math.harvard.edu/archive/20_spring_05/handouts/assignment_overheads.pdf

    // copying matrix (to allow for row and column operations)
    int x,y;
    double **matrix = malloc(size * sizeof(double *));
    assert(matrix != NULL);
    for (y = 0; y < size; y++) {
        matrix[y] = malloc(size * sizeof(double));
        assert(matrix[y] != NULL);
        for (x = 0; x < size; x++) {
            matrix[y][x] = originalMatrix[y][x];
        }
    }
    
    // performing row and column operations to reduce the matrix
    // operations made to entire rows or entire columns of the matrix do not change its minimal assignment(s)
    // thus, a minimal assignment to this matrix is also a minimal assignment to the original matrix
    reduce(matrix, size);

    // creating arrays to represent lines through rows and columns
    int *rowLines = calloc(size, sizeof(int));
    assert(rowLines != NULL);
    int *colLines = calloc(size, sizeof(int));
    assert(colLines != NULL);
    
    int lines, iteration = 1;
    // covering the zeroes in the matrix with the minimum number of lines
    // if this number of lines is equal to the size of the matrix, then an optimal assignment can be found by selecting zeroes
    lines = minLines(matrix, size, rowLines, colLines, iteration);
    while (lines < size) {
        // further changing the matrix to produce more zeroes, thus increasing the minimum number of required lines
        // in essence, this function subtracts all entries by the lowest uncovered value, then adds that value to every line
        // this creates more zeroes out of the uncovered entries, whilst preserving single-covered zeroes
        // double covered zeroes will be removed, but this will not reduce the number of required lines
        // a minimal assignment to the matrix is also a minimal assignment to the original matrix (see above)
        // thus, with every loop the minimum number of required lines can only increase, until 'size' number of lines are reached, and a minimal assignment can be easily found
        moreZero(matrix, size, rowLines, colLines, iteration);
        iteration++;
        lines = minLines(matrix, size, rowLines, colLines, iteration);
    }
    assert(lines == size);

    // approach to finding a minimal assignment:
    // search rows and columns for single zeroes and assign them, then assign doubles, then triples, etc
    double minDistance = 0.0;
    int lastX, lastY, zeroesFound;
    int found = 0, searchNum, foundChanged;
    for (searchNum = 0; found < size; searchNum++) {
        foundChanged = 0; // flag to ensure that searchNum only increments when it absolutely has to
        // search by row
        for (y = 0; y < size; y++) {
            if (rowLines[y] == iteration + 1) continue;
            zeroesFound = 0;
            for (x = 0; x < size; x++) {
                if (matrix[y][x] == 0 && colLines[x] != iteration + 1) {
                    lastX = x;
                    zeroesFound++;
                }
            }
            // valid entry found, assign it
            if (zeroesFound <= searchNum) {
                minDistance += originalMatrix[y][lastX];
                originalMatrix[y][lastX] = MATCH;
                rowLines[y] = iteration + 1;
                colLines[lastX] = iteration + 1;
                found++;
                foundChanged = 1;
            }
        }
        // search by column
        for (x = 0; x < size; x++) {
            if (colLines[x] == iteration + 1) continue;
            zeroesFound = 0;
            for (y = 0; y < size; y++) {
                if (matrix[y][x] == 0 && rowLines[y] != iteration + 1) {
                    lastY = y;
                    zeroesFound++;
                }
            }
            // valid entry found, assign it
            if (zeroesFound <= searchNum) {
                minDistance += originalMatrix[lastY][x];
                originalMatrix[lastY][x] = MATCH;
                rowLines[lastY] = iteration + 1;
                colLines[x] = iteration + 1;
                found++;
                foundChanged = 1;
            }
        }
        if (foundChanged != 0) searchNum--;
    }
    // freeing data
    for (y = 0; y < size; y++) {
        free(matrix[y]);
    }
    free(matrix);
    free(rowLines);
    free(colLines);
    return minDistance;
}

void reduce(double **matrix, int size) {
    int x, y;
    double minimum;
    // row reductions
    for (y = 0; y < size; y++) {
        minimum = matrix[y][0];
        // finding minimum value for row
        for (x = 0; x < size; x++) {
            if (matrix[y][x] < minimum) {
                minimum = matrix[y][x];
            }
        }
        // subtracting it from entire row
        for (x = 0; x < size; x++) {
            matrix[y][x] -= minimum;
        }
    }
    // column reductions
    for (x = 0; x < size; x++) {
        minimum = matrix[0][x];
        // finding minimum value for row
        for (y = 0; y < size; y++) {
            if (matrix[y][x] < minimum) {
                minimum = matrix[y][x];
            }
        }
        // subtracting it from entire row
        for (y = 0; y < size; y++) {
            matrix[y][x] -= minimum;
        }
    }
}

int minLines(double **matrix, int size, int *rowLines, int *colLines, int iteration) {
    int lineCount = 0;
    int x, y;
    int rowZeroes = 0, colZeroes = 0;

    // simple stack implementation to hold double lines
    int *stackX = malloc(size * sizeof(int));
    assert(stackX != NULL);
    int *stackY = malloc(size * sizeof(int));
    assert(stackX != NULL);
    int stackVal = 0;

    for (y = 0; y < size; y++) {
        if (rowLines[y] == iteration) continue; // dont consider zeroes which have already been covered
        for (x = 0; x < size; x++) {
            if (colLines[x] == iteration || rowLines[y] == iteration) continue;
            // zero found, drawing lines
            if (matrix[y][x] == 0) {
                countZeroes(matrix, size, y, x, &rowZeroes, &colZeroes);
                // draw horizontal line if there are more zeroes in the row
                if (rowZeroes > colZeroes) {
                    rowLines[y] = iteration;
                    lineCount++;
                }
                // vertical line
                else if(rowZeroes < colZeroes) {
                    colLines[x] = iteration;
                    lineCount++;
                }
                // draw both lines for now, then remove after all zeroes have been covered
                else {
                    rowLines[y] = iteration;
                    colLines[x] = iteration;
                    lineCount += 2;
                    stackX[stackVal] = x;
                    stackY[stackVal] = y;
                    stackVal++;
                }
            }
        }
    }
    // removing excess lines
    int removable;
    while (--stackVal >= 0) {
        // checking to see if vertical line is removable
        removable = 1;
        for (y = 0; y < size; y++) {
            if (matrix[y][stackX[stackVal]] == 0 && rowLines[y] != iteration) {
                removable = 0;
                break;
            }
        }
        // if this line is removable, then remove it and move on to the next double-covered zero
        if (removable == 1) {
            colLines[stackX[stackVal]] = 0;
            lineCount--;
            continue;
        }
        // vertical line is not removable, check to see if horizontal line is removable
        removable = 1;
        for (x = 0; x < size; x++) {
            if (matrix[stackY[stackVal]][x] == 0 && colLines[x] != iteration) {
                removable = 0;
                break;
            }
        }
        if (removable == 1) {
            rowLines[stackY[stackVal]] = 0;
            lineCount--;
        }
        // if both lines are not removable, then simply move on to the next double-covered zero
    }
    free(stackX);
    free(stackY);
    return lineCount;
}

void countZeroes(double **matrix, int size, int row, int col, int *rowNum, int *colNum) {
    int x, y;
    // counting number of zeroes in row
    *rowNum = 0;
    for (x = 0; x < size; x++) {
        if (matrix[row][x] == 0) {
            (*rowNum)++;
        }
    }
    // counting number of zeroes in column
    *colNum = 0;
    for (y = 0; y < size; y++) {
        if (matrix[y][col] == 0) {
            (*colNum)++;
        }
    }
}


void moreZero(double **matrix, int size, int *rowLines, int *colLines, int iteration) {
    // finding minimum
    int x, y;
    double minimum = 0.0;
    for (y = 0; y < size; y++) {
        if (rowLines[y] == iteration) continue; // skip rows with lines
        for (x = 0; x < size; x++) {
            if (colLines[x] == iteration) continue;
            if (matrix[y][x] < minimum || minimum == 0.0) {
                minimum = matrix[y][x];
            }
        }
    }
    // editing matrix
    for (y = 0; y < size; y++) {
        if (rowLines[y] == iteration) {
            for (x = 0; x < size; x++) {
                // double-covered element found, add minimum value
                if (colLines[x] == iteration) {
                    matrix[y][x] += minimum;
                }
            }
        }
        else {
            for (x = 0; x < size; x++) {
                if (colLines[x] == iteration) {
                    continue;
                }
                // uncovered element
                matrix[y][x] -= minimum;
            }
        }
    }
}










