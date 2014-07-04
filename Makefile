EXECUTABLE = prueba_NEAT
VPATH = ./src ./headers ./objects ./experiments ./VREP/include ./VREP/remoteApi ./VREP/robotSimulator/headers ./VREP/robotSimulator/src ./VREP/robotSimulator/objects
CC = g++ -O3
CFLAGS = -g -Wall -I./headers -I./objects -I./src -I./VREP/include -I./VREP/remoteApi -I./VREP/robotSimulator/src -I./VREP/robotSimulator/headers -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255


Quadratot: quadratot.cpp NEAT.o  genetic_encoding.o random_function.o discrete_probabilities.o 
	@mkdir -p bin
	@cd VREP/robotSimulator/; make
	@cd VREP/; make
	$(CC) $(CFLAGS) ./experiments/quadratot.cpp ./objects/NEAT.o ./objects/genetic_encoding.o  ./objects/random_function.o ./objects/discrete_probabilities.o ./VREP/robotSimulator/objects/RobotSimulator.o ./VREP/robotSimulator/objects/Object.o ./VREP/robotSimulator/objects/Joint.o ./VREP/robotSimulator/objects/Dummy.o ./VREP/remoteApi/extApiPlatform.o ./VREP/remoteApi/extApi.o -o ./bin/Quadratot -lpthread

xor: xor.cpp NEAT.o  genetic_encoding.o random_function.o discrete_probabilities.o
	@mkdir -p bin
	$(CC) $(CFLAGS) ./experiments/xor.cpp ./objects/NEAT.o ./objects/genetic_encoding.o  ./objects/random_function.o ./objects/discrete_probabilities.o -o ./bin/NEAT



NEAT.o: NEAT.cpp discrete_probabilities.o genetic_encoding.o
	@mkdir -p objects
	$(CC) $(CFLAGS) -c ./src/NEAT.cpp  -o ./objects/NEAT.o

example1.o: example1.cpp example1.hpp
	@mkdir -p objects
	$(CC) $(CFLAGS) -c example1.cpp -o ./objects/example1.o

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

run:
	@mkdir -p ./Statistics
	./bin/NEAT user_definitions/user_def_standart genetics_encodings/test_encoding.genetic_encoding

doc:
	doxygen Doxyfile
