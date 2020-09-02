CXX = g++

CFLAGS = -w

LFLAGS = -lSDL2


rast: main.o drawLines.o vector.o image.o
	$(CXX) $(CFLAGS) -v -o rast main.o $(LFLAGS)

main.o: main.cpp drawLines.hpp vector.hpp image.hpp
	$(CXX) $(CFLAGS) -c main.cpp -o main.o

drawLines.o: drawLines.hpp vector.hpp image.hpp
	$(CCXX) $(CFLAGS) -c drawLines.hpp -o drawLines.o

vector.o: vector.hpp
	$(CCXX) $(CFLAGS) -c vector.hpp -o vector.o

image.o: image.hpp
	$(CCXX) $(CFLAGS) -c image.hpp -o image.o

