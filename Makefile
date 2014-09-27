VPATH = ./src ./include ./objects
CC = g++ -std=c++11 -O3
CFLAGS = -Wall -fPIC -I./include -I./objects -I./src

all: NEAT.o geneticEncoding.o randomFunction.o discreteProbabilities.o connectionGene.o globalInformationOfNEAT.o nodeGene.o connectionGene.o specie.o
	@echo "Ready"

connectionGene.o: connectionGene.cpp
	@mkdir -p objects
	$(CC) $(CFLAGS) -c ./src/connectionGene.cpp -o ./objects/connectionGene.o

nodeGene.o: nodeGene.cpp randomFunction.o
	@mkdir -p objects
	$(CC) $(CFLAGS) -c ./src/nodeGene.cpp -o ./objects/nodeGene.o

NEAT.o: NEAT.cpp
	@mkdir -p objects
	$(CC) $(CFLAGS) -c ./src/NEAT.cpp  -o ./objects/NEAT.o

geneticEncoding.o: geneticEncoding.cpp randomFunction.o nodeGene.o connectionGene.o globalInformationOfNEAT.o
	@mkdir -p objects
	$(CC) $(CFLAGS) -c ./src/geneticEncoding.cpp -o ./objects/geneticEncoding.o

randomFunction.o: randomFunction.cpp
	@mkdir -p objects
	$(CC) $(CFLAGS) -c ./src/randomFunction.cpp  -o ./objects/randomFunction.o

discreteProbabilities.o: discreteProbabilities.cpp discreteProbabilities.hpp
	@mkdir -p objects
	$(CC) $(CFLAGS) -c ./src/discreteProbabilities.cpp   -o ./objects/discreteProbabilities.o

globalInformationOfNEAT.o: globalInformationOfNEAT.cpp
	@mkdir -p objects
	$(CC) $(CFLAGS) -c ./src/globalInformationOfNEAT.cpp   -o ./objects/globalInformationOfNEAT.o

specie.o: specie.cpp
	@mkdir -p objects
	$(CC) $(CFLAGS) -c ./src/specie.cpp   -o ./objects/specie.o

clean:
	rm -f ./objects/*.o ./bin/*
	rm -rf ./Statistics ./objects
	rm -rf ./doc

doc: Doxyfile
	doxygen Doxyfile
	cp Doxygen/HrI4ZJpJ3Fh0wa5ofYMK8fk_vArhqVIZ0nv9q090hN8.woff2 Doxygen/HrI4ZJpJ3Fh0wa5ofYMK8RsxEYwM7FgeyaSgU71cLG0.woff ./doc/html

install:
	make
	g++ -shared -Wl,-soname,libneat.so.1 -o libneat.so.1.0 ./objects/nodeGene.o ./objects/connectionGene.o ./objects/discreteProbabilities.o ./objects/NEAT.o ./objects/globalInformationOfNEAT.o ./objects/geneticEncoding.o ./objects/randomFunction.o ./objects/specie.o 
	ln -sf libneat.so.1.0 libneat.so
	ln -sf libneat.so.1.0 libneat.so.1
	mv libneat.so.1.0 libneat.so libneat.so.1 /usr/lib
	mkdir -p /usr/include/NEAT_include/
	cp ./include/* /usr/include/NEAT_include/
	cp NEAT /usr/include