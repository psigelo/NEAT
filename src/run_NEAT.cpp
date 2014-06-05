#include "NEAT.hpp"
#include "genetic_encoding.hpp"
#include <ctime>

using namespace std;
using namespace ANN_USM;




int main(int argc, char** argv){
	if(argc < 3){
		cerr << "Arguments missing, The relative path to the user definitions and genetic encoding files (.genetic_encoding files) must be defined!. \n";
		exit(1);
	}

	std::srand(std::time(0)); // use current time as seed for random generator
	Population poblacion(argv[1],argv[2]);
	for (int i = 0; i < poblacion.GENERATIONS; ++i)
		poblacion.epoch();
	cout << "Fitness champion: " << poblacion.fitness_champion << "\n\n"<< endl;
	cout << poblacion.champion.ANN_function() << endl;
	cout << poblacion.champion << endl;
	return 0;
}
