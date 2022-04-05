# Makefile inside 'soln/test/'

CC = g++
CC_FLAGS = -std=c++11 -Wall

all:
	make reader
	make writer

reader: reader.o
	$(CC) reader.o -o reader

writer: writer.o
	$(CC) writer.o -o writer

reader.o: reader.cpp
	$(CC) $(CC_FLAGS) -c reader.cpp

writer.o: writer.cpp
	$(CC) $(CC_FLAGS) -c writer.cpp

reset:
	rm -f inpipe outpipe

clean:
	rm -f *.o reader writer
	make reset