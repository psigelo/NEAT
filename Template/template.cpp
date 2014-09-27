/*
	Se recomienda leer el manual de usario para usar este template.
*/

#include <NEAT>


using namespace ANN_USM;
using namespace std;

double fitness(geneticEncoding organism){
	double result;
	
	//	Este es un ejemplo de 
	
	vector <double> inputs;
	vector <double> outputs;
	
	outputs = organism.eval(inputs);

	return result;	
}


int main(int argc, char** argv){
	if(argc < 3){cerr << "Error of inputs parameters:: Correct parameters are [1] user definition file, [2] genetic encoding file, [3] The NEAT name,  [4] path to save files. \n";exit(1);}
	std::srand(std::time(0)); // use current time as seed for random generator
	Population poblacion(argv[1],argv[2], argv[3] ,argv[4]);

	for (int i = 0; i < poblacion.GENERATIONS; ++i){
		for (int i = 0; i < (int)poblacion.organisms.size(); ++i)
		{
			poblacion.organisms.at(i).fitness = fitness(poblacion.organisms.at(i));
		}
		poblacion.epoch();
		poblacion.print_to_file_currrent_generation();
	}
	return 0;
}

