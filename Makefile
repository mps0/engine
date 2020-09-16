CXX = g++

CFLAGS = -w

LFLAGS = -lSDL2


rast: main.o drawLine.o vector.o image.o camera.o pipeline.o matrix.o drawTriangle.o assets.o plane.o vertex.o clip.o OBJ_Loader.o obj.o shader.o
	$(CXX) $(CFLAGS) -v -o rast main.o $(LFLAGS)

main.o: main.cpp camera.hpp drawLine.hpp image.hpp vector.hpp pipeline.hpp matrix.hpp drawTriangle.hpp assets.hpp plane.hpp vertex.hpp clip.hpp OBJ_Loader.h obj.hpp shader.hpp
	$(CXX) $(CFLAGS) -c main.cpp -o main.o

drawLine.o: drawLine.hpp vector.hpp image.hpp
	$(CCXX) $(CFLAGS) -c drawLine.hpp -o drawLine.o

vector.o: vector.hpp
	$(CCXX) $(CFLAGS) -c vector.hpp -o vector.o

image.o: image.hpp
	$(CCXX) $(CFLAGS) -c image.hpp -o image.o

camera.o: camera.hpp vector.hpp
	$(CCXX) $(CFLAGS) -c camera.hpp -o camera.o

pipeline.o: pipeline.hpp camera.hpp vertex.hpp plane.hpp
	$(CCXX) $(CFLAGS) -c pipeline.hpp -o pipeline.o

matrix.o: matrix.hpp vector.hpp
	$(CCXX) $(CFLAGS) -c matrix.hpp -o matrix.o

drawTriangle.o: drawTriangle.hpp
	$(CCXX) $(CFLAGS) -c drawTriangle.hpp -o drawTriangle.o

assets.o: assets.hpp vector.hpp
	$(CCXX) $(CFLAGS) -c assets.hpp -o assets.o

plane.o: plane.hpp vector.hpp
	$(CCXX) $(CFLAGS) -c plane.hpp -o plane.o

vertex.o: vertex.hpp vector.hpp
	$(CCXX) $(CFLAGS) -c vertex.hpp -o vertex.o

clip.o: clip.hpp vector.hpp
	$(CCXX) $(CFLAGS) -c clip.hpp -o clip.o

obj.o: obj.hpp vector.hpp
	$(CCXX) $(CFLAGS) -c obj.hpp -o obj.o


OBJ_Loader.o: OBJ_Loader.h
	$(CCXX) $(CFLAGS) -c OBJ_Loader.h -o OBJ_Loader.o

shader.o: shader.hpp vector.hpp
	$(CCXX) $(CFLAGS) -c shader.hpp -o shader.o
