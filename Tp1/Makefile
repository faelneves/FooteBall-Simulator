OPCOES_COMPILADOR=-I. -lglut -lGLU -lGLEW -lGL -lSOIL -lSDL -lSDL_mixer

all: main.o
	g++ -o main main.o $^ $(OPCOES_COMPILADOR)

main.o: main.cpp
	g++ -c main.cpp
run: all
	./main

clean:
	rm *.o main