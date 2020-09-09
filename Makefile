CXX = g++

CFLAGS = -w

LFLAGS = -lSDL2


rast: main.o drawLines.o vector.o image.o camera.o pipeline.o matrix.o drawTriangle.o assets.o
	$(CXX) $(CFLAGS) -v -o rast main.o $(LFLAGS)

main.o: main.cpp camera.hpp drawLines.hpp image.hpp vector.hpp pipeline.hpp matrix.hpp drawTriangle.hpp assets.hpp
	$(CXX) $(CFLAGS) -c main.cpp -o main.o

drawLines.o: drawLines.hpp vector.hpp image.hpp
	$(CCXX) $(CFLAGS) -c drawLines.hpp -o drawLines.o

vector.o: vector.hpp
	$(CCXX) $(CFLAGS) -c vector.hpp -o vector.o

image.o: image.hpp
	$(CCXX) $(CFLAGS) -c image.hpp -o image.o

camera.o: camera.hpp vector.hpp
	$(CCXX) $(CFLAGS) -c camera.hpp -o camera.o

pipeline.o: pipeline.hpp camera.hpp
	$(CCXX) $(CFLAGS) -c pipeline.hpp -o pipeline.o

matrix.o: matrix.hpp vector.hpp
	$(CCXX) $(CFLAGS) -c matrix.hpp -o matrix.o

drawTriangle.o: drawTriangle.hpp
	$(CCXX) $(CFLAGS) -c drawTriangle.hpp -o drawTriangle.o

assets.o: assets.hpp vector.hpp
	$(CCXX) $(CFLAGS) -c assets.hpp -o assets.o
