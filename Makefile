VPATH = ./src ./headers ./objects ./experiments ./VREP/include ./VREP/remoteApi ./VREP/robotSimulator/headers ./VREP/robotSimulator/src ./VREP/robotSimulator/objects
CC = g++ -O3
CFLAGS = -Wall -fPIC -I./headers -I./objects -I./src -I./VREP/include -I./VREP/remoteApi -I./VREP/robotSimulator/src -I./VREP/robotSimulator/headers -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255



all: NEAT.o genetic_encoding.o random_function.o discrete_probabilities.o

NEAT.o: NEAT.cpp discrete_probabilities.o genetic_encoding.o
	@mkdir -p objects
	$(CC) $(CFLAGS) -c ./src/NEAT.cpp  -o ./objects/NEAT.o

genetic_encoding.o: genetic_encoding.cpp random_function.o
	@mkdir -p objects
	$(CC) $(CFLAGS) -c ./src/genetic_encoding.cpp -o ./objects/genetic_encoding.o

random_function.o: random_function.cpp
	@mkdir -p objects
	$(CC) $(CFLAGS) -c ./src/random_function.cpp  -o ./objects/random_function.o

discrete_probabilities.o: discrete_probabilities.cpp
	@mkdir -p objects
	$(CC) $(CFLAGS) -c ./src/discrete_probabilities.cpp   -o ./objects/discrete_probabilities.o

clean:
	rm -f ./objects/*.o ./bin/*
	rm -rf ./Statistics ./objects

run:
	@mkdir -p ./Statistics
	./bin/NEAT user_definitions/user_def_standart genetics_encodings/test_encoding.genetic_encoding ./objects/random_function.o ./objects/genetic_encoding.o 

doc:
	doxygen Doxyfile

install:
	g++ -shared -Wl,-soname,libneat.so.1 -o libneat.so.1.0 ./objects/discrete_probabilities.o ./objects/NEAT.o  ./objects/genetic_encoding.o ./objects/random_function.o 
	ln -sf libneat.so.1.0 libneat.so
	ln -sf libneat.so.1.0 libneat.so.1
	mv libneat.so.1.0 libneat.so libneat.so.1 /usr/lib
	mkdir -p /usr/include/NEAT_headers/
	cp ./headers/* /usr/include/NEAT_headers/
	cp NEAT /usr/include