#include "NEAT.hpp"
#include "genetic_encoding.hpp"

using namespace std;
using namespace ANN_USM;
#include <ctime>

int main(int argc, char** argv){
	std::srand(std::time(0)); // use current time as seed for random generator
	char path[]="./files/test_population";
	if(argc < 2){
		cerr << "Faltan argumentos, se debe ingresar la ruta relativa hasta algun genetic_encoding\n";
		exit(1);
	}

	Population poblacion;
	poblacion.init_population(argv[1]);
	poblacion.epoch();
	poblacion.save(path);
	//poblacion.print_niches();
	//cout << poblacion.organisms[poblacion.current_niches[(int)poblacion.current_niches.size()-1].organism_position[0]] << endl;
	cout << "\n";
	return 0;

}