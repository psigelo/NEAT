EXECUTABLE = prueba_NEAT
VPATH = ./src ./headers ./objects
CC = g++ -O3
CFLAGS = -g -Wall -I./headers -I./objects -I./src



neat: NEAT.o 
	$(CC) $(CFLAGS) $(LIBS) example1.o NEAT.o  -o $(EXECUTABLE)

test_genetic_encoding: genetic_encoding.o test_genetic_encoding.cpp 
	$(CC) $(CFLAGS)  ./src/test_genetic_encoding.cpp ./objects/genetic_encoding.o -o ./executables/$@

NEAT.o: NEAT.cpp genetic_encoding.o
	$(CC) $(CFLAGS) -c ./src/NEAT.cpp ./objects/genetic_encoding.o -o ./objects/NEAT.o

example1.o: example1.cpp example1.hpp 
	$(CC) $(CFLAGS) -c example1.cpp -o ./objects/example1.o	

genetic_encoding.o: genetic_encoding.cpp 
	$(CC) $(CFLAGS) -c $^ -o ./objects/genetic_encoding.o
	
clean:
	rm -f ./objects/*.o ./executables/$(EXECUTABLE)
