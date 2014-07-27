CC = g++ -O3
CFLAGS = -Wall -I../headers -I../objects -I../src 


xor: xor.cpp 
	@cd ..; make 
	@mkdir -p bin
	@mkdir -p XOR_results_files
	@rm -f XOR_results_files/**
	$(CC) $(CFLAGS) ./xor.cpp ../objects/NEAT.o ../objects/genetic_encoding.o  ../objects/random_function.o ../objects/discrete_probabilities.o -o ./bin/NEAT


runXor:
	@mkdir -p ./XOR_results_files
	./bin/NEAT user_definitions/user_def_standart genetics_encodings/test_encoding ./XOR_results_files/


clean:
	@cd ..; make clean
	@rm -rf XOR_results_files/ bin