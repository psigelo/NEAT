#include "NEAT.hpp"
#include "genetic_encoding.hpp"
#include <ctime>

using namespace ANN_USM;
using namespace std;

double fitness(Genetic_Encoding organism){

	vector<double> output;
	vector<double> input;

	double error_sum = 0;

	//======================
	//	XOR
	//======================

	//cout << organism << endl;

	// XOR(0,0) -> 0
	input.push_back(0);
	input.push_back(0);

	output = organism.eval(input);

	error_sum += abs(output.at(0));
	input.clear();

	// XOR(0,1) -> 1
	input.push_back(0);
	input.push_back(1);
	output = organism.eval(input);
	error_sum += abs(1 - output.at(0));
	input.clear();

	// XOR(1,0) -> 1
	input.push_back(1);
	input.push_back(0);
	output = organism.eval(input);
	error_sum += abs(1 - output.at(0));
	input.clear();

	// XOR(1,1) -> 0
	input.push_back(1);
	input.push_back(1);
	output = organism.eval(input);
	error_sum += abs(output.at(0));
	input.clear();

	return pow(4 - error_sum, 2);
}


int main(int argc, char** argv){
	if(argc < 3){
		cerr << "Arguments missing, The relative path to the user definitions and genetic encoding files (.genetic_encoding files) must be defined!. \n";
		exit(1);
	}
	
	std::srand(std::time(0)); // use current time as seed for random generator
	Population poblacion(argv[1],argv[2]);
	cerr << poblacion.GENERATIONS << "\t" << poblacion.POPULATION_MAX << "\t" << poblacion.organisms.size() << endl;
	for (int i = 0; i < poblacion.GENERATIONS; ++i){
		for (int i = 0; i < (int)poblacion.organisms.size(); ++i)
		{
			poblacion.organisms.at(i).fitness = fitness(poblacion.organisms.at(i));
		}
		poblacion.epoch();
	}

	cout << "Fitness champion: " << poblacion.fitness_champion << "\n\n"<< endl;
	cout << poblacion.champion.ANN_function() << endl;
	cout << poblacion.champion << endl;
	return 0;
}