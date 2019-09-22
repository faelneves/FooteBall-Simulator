OPCOES_COMPILADOR=-lGL -lglut -lGLEW -lGLU -lSOIL -lm -lSDL2 -lSDL2_mixer

all: main.o
	g++ -o main main.o $(OPCOES_COMPILADOR)

main.o: main.cpp
	gcc main.cpp -lGL -lglut -lGLEW -lGLU -lSOIL -lm -lSDL2 -lSDL2_mixer -o main
run: all
	./main

clean:
	rm *.o main