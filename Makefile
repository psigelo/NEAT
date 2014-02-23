EXECUTABLE = prueba_NEAT
VPATH = ./src ./headers ./objects
CC = g++ -O3
CFLAGS = -g -Wall -I./headers -I./objects -I./src

test_genetic_encoding: genetic_encoding.o test_genetic_encoding.cpp 
	@$(CC) $(CFLAGS)  ./src/test_genetic_encoding.cpp ./objects/genetic_encoding.o  -o ./executables/$@

test_NEAT: test_NEAT.cpp NEAT.o  genetic_encoding.o fitness.o 
	@$(CC) $(CFLAGS) ./src/test_NEAT.cpp ./objects/NEAT.o ./objects/genetic_encoding.o ./objects/fitness.o -o ./executables/NEAT_test

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
