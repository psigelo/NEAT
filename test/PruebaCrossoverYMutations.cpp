#include "geneticEncoding.hpp"
#include "globalInformationOfNEAT.hpp"
#include <ctime>
using namespace NEAT_USM;

int main(){
	srand(time(0)); // use current time as seed for random generator
	char rutaUserDef[] = "./testUserdef/PruebaCrossoverYMutations";
	char rutaGenome[]  = "./testGenomes/PruebaCrossoverYMutations";


	globalInformationOfNEAT  informationForGenome(rutaUserDef);
	geneticEncoding organismoPrueba (rutaGenome, &informationForGenome);
	geneticEncoding organismoPrueba2 (organismoPrueba);
	
	organismoPrueba.mutateNode();
	organismoPrueba.mutateNode();
	organismoPrueba.mutateConnection();
	organismoPrueba.mutateConnection();
	organismoPrueba.mutateNode();
	organismoPrueba.mutateConnection();
	organismoPrueba.mutateConnection();
	organismoPrueba.mutateNode();
	organismoPrueba.mutateConnection();
	organismoPrueba.mutateNode();
	organismoPrueba.mutateNode();

	organismoPrueba2.mutateNode();
	organismoPrueba2.mutateNode();
	organismoPrueba2.mutateConnection();
	organismoPrueba2.mutateConnection();
	organismoPrueba2.mutateNode();
	organismoPrueba2.mutateConnection();
	organismoPrueba2.mutateConnection();
	organismoPrueba2.mutateNode();
	organismoPrueba2.mutateConnection();
	organismoPrueba2.mutateNode();
	organismoPrueba2.mutateNode();

	organismoPrueba.printHumanReadable(); 
	organismoPrueba2.printHumanReadable(); 


	geneticEncoding * resultado = organismoPrueba * organismoPrueba2;

	resultado->printHumanReadable();

	return 0;
}