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
	//char path[]="./files/last_run_population_result";
	//poblacion.save(path);
	poblacion.print_niches();
	cout << "\n";

	cout << "champion: " << poblacion.fitness_champion << endl;
	cout << poblacion.champion << endl;
	
	/*
	cerr << "P0\n";
	Genetic_Encoding organism;
	organism.load(argv[1]);
	vector <double> inputs;
	vector <double> outputs;
	cerr << "P1\n";
	inputs.push_back(0.2);
	inputs.push_back(0.8);
	inputs.push_back(0.1);
	
	organism.row_orderer_list.push_back(0);
	organism.row_orderer_list.push_back(1);

	outputs=organism.eval(inputs);
	cerr << "resultado:\t" << outputs[0]<< endl;
	*/
	return 0;
}