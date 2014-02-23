#include "NEAT.hpp"
#include "genetic_encoding.hpp"

using namespace std;
using namespace ANN_USM;

#include <ctime>
enum user_definitions{
	GENERATIONS = 100
};


int main(int argc, char** argv){
	std::srand(std::time(0)); // use current time as seed for random generator
	
	if(argc < 2){
		cerr << "Faltan argumentos, se debe ingresar la ruta relativa hasta algun genetic_encoding\n";
		exit(1);
	}

	Population poblacion;
	
	poblacion.init_population(argv[1]);
	
	for (int i = 0; i < GENERATIONS; ++i)
	{
		poblacion.epoch();
	}

	/*
	for (int i = 0; i < (int)poblacion.row_orderer_list.size(); ++i)
	{
		cerr << "\t\t" <<  poblacion.row_orderer_list[i];
	}
	cerr << endl;


	
	char path[]="./files/test_population";
	poblacion.save(path);
	*/
	
	cout << "\n";
	return 0;
}