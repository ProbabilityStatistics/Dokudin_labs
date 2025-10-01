CC=g++
CFLAGS=-Wextra -Werror -Wall
all: main

main: pipe.o compressor_station.o process.o main.cpp
	$(CC) $(CFLAGS) -o $@ $^

pipe.o: pipe.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

cpmpressor_station.o: compressor_station.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

process.o: process.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o

rebuild: all clean