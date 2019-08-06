CC=gcc $(CFLAGS)
CFLAGS=-Wall -lm -std=c11 -g
OBJS=collection.o connection.o
ALLOBJS=collection.o connection.o pagerank.o inverted.o searchPagerank.o searchTfIdf.o invertedInput.o

all : pagerank inverted searchPagerank searchTfIdf

pagerank : $(OBJS) pagerank.o
	$(CC) -o pagerank $(OBJS) pagerank.o

inverted : $(OBJS) inverted.o
	$(CC) -o inverted $(OBJS) inverted.o

searchPagerank : searchPagerank.o collection.o invertedInput.o
	$(CC) -o searchPagerank searchPagerank.o invertedInput.o collection.o

searchTfIdf : searchTfIdf.o collection.o invertedInput.o
	$(CC) -o searchTfIdf searchTfIdf.o collection.o invertedInput.o

collection.o : collection.c collection.h
pagerank.o : pagerank.c pagerank.h connection.h collection.h
connection.o : connection.c collection.h
inverted.o : inverted.c inverted.h collection.h
searchPagerank.o : searchPagerank.c collection.h
invertedInput.o : invertedInput.c collection.o invertedInput.h collection.h
searchTfIdf.o : searchTfIdf.c collection.h

clean :
	rm -f pagerank inverted searchPagerank searchTfIdf $(ALLOBJS) core *.dSYM invertedIndex.txt pagerankList.txt
