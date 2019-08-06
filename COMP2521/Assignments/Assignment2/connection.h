// Written by Kevin Luong z5164800 and Gordon Xie z5160618
// Group name googzilla_firechrome

// external functions

// given a collection of URLs
// returns a matrix representing the links between the URLs
// connections[a][b] == 1 represents a link from page a to page b
// connections[0..(size-1)][size] represents the number of outlinks from page a (first index)
// connections[size][0..(size-1)] represents the number of inlinks to page b (second index)
// connections[size][size] should always be 0 and does not represent anything
int **getConnections(Collection collection);

// frees the given connections matrix
void freeConnections(int **connections, int size);

// given the indices of two pages and a connections matrix
// returns the weight of the link from v to u based on inlinks
double weightIn(int v, int u, int **connections, int size);

// given the indices of two pages and a connections matrix
// returns the weight of the link from v to u based on outlinks
double weightOut(int v, int u, int **connections, int size);
