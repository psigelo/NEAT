#include "NEAT.hpp"
#include "genetic_encoding.hpp"
#include <ctime>
#include "user_definitions.hpp"

using namespace std;
using namespace ANN_USM;




int main(int argc, char** argv){
	
	
	if(argc < 2){
		cerr << "Arguments missing, The relative path to the genetic encoding files (.genetic_encoding files) must be defined!. \n";
		exit(1);
	}
	
	std::srand(std::time(0)); // use current time as seed for random generator
	Population poblacion;
	poblacion.init_population(argv[1]);
	for (int i = 0; i < GENERATIONS; ++i)
		poblacion.epoch();
	cout << "Fitness champion: " << poblacion.fitness_champion << endl;
	cout << poblacion.champion << endl;
	return 0;
}