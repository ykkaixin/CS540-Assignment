CC=g++
CFLAGS=-Wall -Wextra -std=c++11 -ldl -g

all: test-run

%.o: %.cpp 
	$(CC) -c -o $@ $< $(CFLAGS)

test-run: test.o
	$(CC) test.o $(CFLAGS) -o run.exe

clean:
	rm *.o run.exe
