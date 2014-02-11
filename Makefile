EXECUTABLE = prueba_NEAT
VPATH = ./src ./headers ./objects
CC = g++ -O3
CFLAGS = -g -Wall -I./headers -I./objects -I./src



neat: NEAT.o 
	$(CC) $(CFLAGS) $(LIBS) -larmadillo example1.o NEAT.o  -o $(EXECUTABLE)

test_genetic_encoding: genetic_encoding.o test_genetic_encoding.cpp 
	$(CC) $(CFLAGS)  $^ -o $@

NEAT.o: NEAT.cpp NEAT.hpp
	$(CC) $(CFLAGS) -c NEAT.cpp -o ./objects/NEAT.o

example1.o: example1.cpp example1.hpp 
	$(CC) $(CFLAGS) -c example1.cpp -o ./objects/example1.o	

genetic_encoding.o: genetic_encoding.cpp 
	$(CC) $(CFLAGS) -c $^ -o ./objects/genetic_encoding.o
	

clean:
	rm -f ./objects/*.o $(EXECUTABLE)
