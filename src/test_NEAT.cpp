#include "NEAT.hpp"
#include "genetic_encoding.hpp"

using namespace std;
using namespace ANN_USM;

#include <ctime>
enum user_definitions{
	GENERATIONS = 10
};


int main(int argc, char** argv){
	std::srand(std::time(0)); // use current time as seed for random generator
	
	if(argc < 2){
		cerr << "Faltan argumentos, se debe ingresar la ruta relativa hasta algun genetic_encoding\n";
		exit(1);
	}

	Population poblacion;
	
	poblacion.init_population(argv[1]);
	
	/*
	poblacion.print_niches();
	poblacion.epoch();
	poblacion.print_niches();
	poblacion.epoch();
	poblacion.print_niches();
	poblacion.epoch();
	poblacion.print_niches();
	poblacion.epoch();
	poblacion.print_niches();
	poblacion.epoch();
	poblacion.print_niches();
	poblacion.epoch();
	poblacion.print_niches();
	
	*/
	for (int i = 0; i < 100; ++i)
	{
		poblacion.epoch();

	}
	//poblacion.print_niches();
	//char path[]="./files/test_population";
	//poblacion.save(path);
	//poblacion.print_niches();
	//cout << poblacion.organisms[poblacion.current_niches[(int)poblacion.current_niches.size()-1].organism_position[0]] << endl;
	cout << "\n";
	return 0;

}