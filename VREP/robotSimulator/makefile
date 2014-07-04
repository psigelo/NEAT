CC = g++ -O3
VPATH = ./src ./headers 
CFLAGS = -g -Wall -I../remoteApi -I../include -I./src -I./headers -I./objects -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255
LDFLAGS = -lpthread

all: RobotSimulator.o Object.o Joint.o Dummy.o

RobotSimulator.o: RobotSimulator.cpp 
	@mkdir -p objects 
	@echo "Compiling RobotSimulator.cpp to RobotSimulator.o"
	@$(CC) $(CFLAGS) -c ./src/RobotSimulator.cpp -o ./objects/RobotSimulator.o

Object.o: Object.cpp 
	@mkdir -p objects 
	@echo "Compiling Object.cpp to Object.o"
	@$(CC) $(CFLAGS) -c ./src/Object.cpp -o ./objects/Object.o

Joint.o: Joint.cpp 
	@mkdir -p objects 
	@echo "Compiling Joint.cpp to Joint.o"
	@$(CC) $(CFLAGS) -c ./src/Joint.cpp -o ./objects/Joint.o

Dummy.o: Dummy.cpp 
	@mkdir -p objects 
	@echo "Compiling Dummy.cpp to Dummy.o"
	@$(CC) $(CFLAGS) -c ./src/Dummy.cpp -o ./objects/Dummy.o

clean:
	@rm -f ./objects/*.o