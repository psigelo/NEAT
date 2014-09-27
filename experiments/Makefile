CC = g++ -std=c++11 -O3
CFLAGS = -Wall 

xor: xor.cpp 
	@cd ..; make; make install
	@mkdir -p bin
	@mkdir -p XOR_results_files
	@rm -f XOR_results_files/**
	$(CC) $(CFLAGS) ./xor.cpp  -o ./bin/NEAT -lneat

runXor:
	@mkdir -p ./XOR_results_files
	./bin/NEAT user_definitions/user_def_standart genetics_encodings/xor.genetic_encoding 

clean:
	@cd ..; make clean
	@rm -rf XOR_results_files/ bin