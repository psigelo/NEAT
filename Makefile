EXECUTABLE = prueba_NEAT
VPATH = ./src ./headers ./objects ./experiments ./VREP/include ./VREP/remoteApi ./VREP/robotSimulator/headers ./VREP/robotSimulator/src ./VREP/robotSimulator/objects
CC = g++ -O3
CFLAGS = -Wall -fPIC -I./headers -I./objects -I./src -I./VREP/include -I./VREP/remoteApi -I./VREP/robotSimulator/src -I./VREP/robotSimulator/headers -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255



all: NEAT.o genetics_encodings.o random_function.o discrete_probabilities.o

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
	



Quadratot2GnomeExec:  quadratot2_gnome_exec.cpp NEAT.o  genetic_encoding.o random_function.o discrete_probabilities.o 
	@mkdir -p bin
	@mkdir -p Statistics
	@cd VREP/; make
	$(CC) $(CFLAGS) ./experiments/quadratot2_gnome_exec.cpp ./objects/NEAT.o ./objects/genetic_encoding.o  ./objects/random_function.o ./objects/discrete_probabilities.o  ./VREP/remoteApi/extApiPlatform.o ./VREP/remoteApi/extApi.o -o ./bin/Quadratot2GenomeExec -lpthread



QuadratotHem: quadratot_hemisferios.cpp NEAT.o  genetic_encoding.o random_function.o discrete_probabilities.o 
	@mkdir -p bin
	@mkdir -p Statistics
	@cd VREP/; make
	$(CC) $(CFLAGS) ./experiments/quadratot_hemisferios.cpp ./objects/NEAT.o ./objects/genetic_encoding.o  ./objects/random_function.o ./objects/discrete_probabilities.o  ./VREP/remoteApi/extApiPlatform.o ./VREP/remoteApi/extApi.o -o ./bin/QuadratotHem -lpthread



Quadratot4: quadratot4.cpp NEAT.o  genetic_encoding.o random_function.o discrete_probabilities.o 
	@mkdir -p bin
	@mkdir -p Statistics
	@cd VREP/; make
	$(CC) $(CFLAGS) ./experiments/quadratot4.cpp ./objects/NEAT.o ./objects/genetic_encoding.o  ./objects/random_function.o ./objects/discrete_probabilities.o  ./VREP/remoteApi/extApiPlatform.o ./VREP/remoteApi/extApi.o -o ./bin/Quadratot4 -lpthread


Quadratot3: quadratot3.cpp NEAT.o  genetic_encoding.o random_function.o discrete_probabilities.o 
	@mkdir -p bin
	@mkdir -p Statistics
	@cd VREP/; make
	$(CC) $(CFLAGS) ./experiments/quadratot3.cpp ./objects/NEAT.o ./objects/genetic_encoding.o  ./objects/random_function.o ./objects/discrete_probabilities.o  ./VREP/remoteApi/extApiPlatform.o ./VREP/remoteApi/extApi.o -o ./bin/Quadratot3 -lpthread


Quadratot2: quadratot2.cpp NEAT.o  genetic_encoding.o random_function.o discrete_probabilities.o 
	@mkdir -p bin
	@mkdir -p Statistics
	@cd VREP/; make
	$(CC) $(CFLAGS) ./experiments/quadratot2.cpp ./objects/NEAT.o ./objects/genetic_encoding.o  ./objects/random_function.o ./objects/discrete_probabilities.o  ./VREP/remoteApi/extApiPlatform.o ./VREP/remoteApi/extApi.o -o ./bin/Quadratot2 -lpthread


Quadratot: quadratot.cpp NEAT.o  genetic_encoding.o random_function.o discrete_probabilities.o 
	@mkdir -p bin
	@mkdir -p Statistics
	@cd VREP/; make
	$(CC) $(CFLAGS) ./experiments/quadratot.cpp ./objects/NEAT.o ./objects/genetic_encoding.o  ./objects/random_function.o ./objects/discrete_probabilities.o  ./VREP/remoteApi/extApiPlatform.o ./VREP/remoteApi/extApi.o -o ./bin/Quadratot -lpthread

xor: xor.cpp NEAT.o  genetic_encoding.o random_function.o discrete_probabilities.o
	@mkdir -p bin
	@mkdir -p Statistics
	$(CC) $(CFLAGS) ./experiments/xor.cpp ./objects/NEAT.o ./objects/genetic_encoding.o  ./objects/random_function.o ./objects/discrete_probabilities.o -o ./bin/NEAT



runQuadratot:
	./bin/Quadratot ./user_definitions/quadratot_def ./genetics_encodings/quadratot_gen

runQuadratot2:
	./bin/Quadratot2 ./user_definitions/quadratot_def ./genetics_encodings/quadratot_gen2

runQuadratot3:
	./bin/Quadratot3 ./user_definitions/quadratot_def ./genetics_encodings/quadratot_gen3	

runQuadratot4:
	./bin/Quadratot4 ./user_definitions/quadratot_def ./genetics_encodings/quadratot_gen4

runQuadratotHem:
	./bin/QuadratotHem ./user_definitions/quadratot_def ./genetics_encodings/hem_der ./genetics_encodings/hem_izq ./genetics_encodings/hem_cen