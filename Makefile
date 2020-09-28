CC=g++
CFLAGS=-std=c++17
LDFLAGS=-lSDL2

src=$(wildcard *.cpp)
obj=$(src:.cpp=.o)

rast: $(obj)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) myprog
