EXECUTABLE = prueba_NEAT
VPATH = ./src ./headers ./objects
CC = g++ -O3
CFLAGS = -g -Wall -I./headers -I./objects -I./src


NEAT: run_NEAT.cpp NEAT.o  genetic_encoding.o fitness.o
	@mkdir -p bin
	$(CC) $(CFLAGS) ./src/run_NEAT.cpp ./objects/NEAT.o ./objects/genetic_encoding.o ./objects/fitness.o -o ./bin/NEAT 

NEAT.o: NEAT.cpp
	@mkdir -p objects
	$(CC) $(CFLAGS) -c ./src/NEAT.cpp  -o ./objects/NEAT.o

example1.o: example1.cpp example1.hpp 
	@mkdir -p objects
	$(CC) $(CFLAGS) -c example1.cpp -o ./objects/example1.o	

genetic_encoding.o: genetic_encoding.cpp 
	@mkdir -p objects
	$(CC) $(CFLAGS) -c $^ -o ./objects/genetic_encoding.o 

fitness.o: fitness.cpp 
	@mkdir -p objects
	$(CC) $(CFLAGS) -c ./src/fitness.cpp  -o ./objects/fitness.o

prueba: prueba_random.cpp random_function.o
	$(CC) $(CFLAGS) ./src/prueba_random.cpp ./objects/random_function.o -o ./bin/prueba -fpermissive

random_function.o: random_function.cpp
	@mkdir -p objects
	$(CC) $(CFLAGS) -c ./src/random_function.cpp  -o ./objects/random_function.o

clean:
	rm -f ./objects/*.o ./bin/*
