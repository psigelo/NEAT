VPATH = ../src ../include ../objects
CC = g++ -std=c++11 -O3
CFLAGS = -Wall -fPIC -I../include -I../objects -I../src

PruebaCrossoverYMutations: PruebaCrossoverYMutations.cpp
	mkdir -p bin
	cd ..; make
	$(CC) $(CFLAGS) ./PruebaCrossoverYMutations.cpp ../objects/nodeGene.o ../objects/connectionGene.o ../objects/discreteProbabilities.o ../objects/NEAT.o ../objects/globalInformationOfNEAT.o ../objects/geneticEncoding.o ../objects/randomFunction.o ../objects/specie.o  -o ./bin/PruebaCrossoverYMutations

RunPruebaCrossoverYMutations:
	./bin/PruebaCrossoverYMutations


clean:
	rm -Rf ./bin/