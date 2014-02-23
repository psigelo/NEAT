EXECUTABLE = prueba_NEAT
VPATH = ./src ./headers ./objects
CC = g++ -O3
CFLAGS = -g -Wall -I./headers -I./objects -I./src


NEAT: run_NEAT.cpp NEAT.o  genetic_encoding.o fitness.o 
	@$(CC) $(CFLAGS) ./src/run_NEAT.cpp ./objects/NEAT.o ./objects/genetic_encoding.o ./objects/fitness.o -o ./executables/NEAT

NEAT.o: NEAT.cpp
	@$(CC) $(CFLAGS) -c ./src/NEAT.cpp  -o ./objects/NEAT.o

example1.o: example1.cpp example1.hpp 
	@$(CC) $(CFLAGS) -c example1.cpp -o ./objects/example1.o	

genetic_encoding.o: genetic_encoding.cpp 
	@$(CC) $(CFLAGS) -c $^ -o ./objects/genetic_encoding.o

fitness.o: fitness.cpp 
	@$(CC) $(CFLAGS) -c ./src/fitness.cpp  -o ./objects/fitness.o

clean:
	@rm -f ./objects/*.o ./executables/*
