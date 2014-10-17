//======================
//	XOR
//======================


#include <NEAT>
#include <ctime>

using namespace NEAT_USM;
using namespace std;

double fitness(geneticEncoding & organism){
	vector<double> output;
	vector<double> input;

	double error_sum = 0;
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
	error_sum += abs(1 - abs(output.at(0)) );
	input.clear();
	// XOR(1,0) -> 1
	input.push_back(1);
	input.push_back(0);
	output = organism.eval(input);
	error_sum += abs(1 - abs(output.at(0)) );
	input.clear();
	// XOR(1,1) -> 0
	input.push_back(1);
	input.push_back(1);
	output = organism.eval(input);
	error_sum += abs(output.at(0));
	input.clear();
	//cerr << "Fitness: " << pow(4 - error_sum, 2) << endl;
	return pow(4 - error_sum, 2);
}


int main(int argc, char** argv){
	if(argc < 3){
		cerr << "Error of inputs parameters:: Correct parameters are [1] user definition file, [2] genetic encoding file, [3] path to save files. \n";
		exit(1);
	}
	srand(time(0)); // use current time as seed for random generator
	population poblacion(argv[1],argv[2]);
	int generations = poblacion.getGenerations();
	for (int i = 0; i < generations; ++i){
		int cantidadDeOrganismos = poblacion.getOrganismsAmount();
		for (int j = 0; j < cantidadDeOrganismos; ++j)
		{
			geneticEncoding * organism = poblacion.getPointerToNewOrganismAtPosition(j);
			organism->setFitness( fitness( *organism ) );
		}
		poblacion.epoch();
		//poblacion.printStatistics();
	}
	return 0;
}