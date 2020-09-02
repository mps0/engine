OBJS = main.cpp

CC = g++

COMPILER_FLAGS = -w

LINKER_FLAGS = -lSDL2

OBJ_NAME = SDLtest

all : $(OBJS)
		g++ $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -w -lSDL2 -o $(OBJ_NAME)

