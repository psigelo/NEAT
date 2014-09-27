#include "specie.hpp"
using namespace NEAT_USM;

specie::specie(int id, globalInformationOfNEAT * information){
	this->id=id;
	age = 1;
	globalInformation = information;
	representant = new geneticEncoding();
}
void specie::organismPushBack( geneticEncoding * organism){
	organisms.push_back(organism);
}
geneticEncoding * specie::getOrganismAt( int position){
	return organisms.at(position);
}
void specie::setRepresentant(geneticEncoding * organism){
	delete(representant);
	representant = new geneticEncoding(*organism);
}
geneticEncoding * specie::getRepresentant(){
	return representant;
}
uint specie::getAmountOfOrganism(){
	return organisms.size();
}
void specie::computeStatistics(){
	int amountOfOrganism = organisms.size();
 	totalFitness = 0.0;
	for (int i = 0; i < amountOfOrganism; ++i){
		totalFitness += organisms.at(i)->getFitness();
		cerr << "fitness" << organisms.at(i)->getFitness() << endl;
	}
	fitnessAverage = totalFitness/(double)amountOfOrganism;

	cerr << fitnessAverage << endl;
}
void specie::deleteWorseOrganisms(){ // this is after of compute statistics, in other way is probably that deleteWorseOrganism() will fail
	totalFitness=0.0;
	for (uint i = 0; i < organisms.size(); ++i){
		if(organisms.at(i)->getFitness() < fitnessAverage){
			delete( organisms.at(i) );
			organisms.erase(organisms.begin() + i);
			i--;
		}
		else{
			totalFitness += organisms.at(i)->getFitness();
		}
	}
	fitnessAverage = totalFitness/(double)organisms.size();
}
double specie::getfitnessOfOrganismAt(int position){
	return organisms.at(position)->getFitness();
}
geneticEncoding * specie::getPointerToOrganismAt(int organismPosition){
	return organisms.at(organismPosition);
}
void specie::newYear(){
	for (uint i = 0; i < organisms.size(); ++i){
		if(organisms.at(i)->getYear() > 2 ){
			double lifeExpectative = globalInformation->getExpectativeOfLife();
			if(rand()/(double)RAND_MAX > lifeExpectative/(1.0 + lifeExpectative)) {
				organisms.erase(organisms.begin() + i);
				i--;
			}
			else
				organisms.at(i)->incrementYear();
		}
		else
			organisms.at(i)->incrementYear();
	}
}

double specie::getTotalFitness(){
	return totalFitness;
}
int specie::getId(){
	return id;
}
void specie::printStatistics(){
	cout << "-------------------------------" 	<< endl;
	cout << "Specie id: " 			 			<< id 				<< endl;
	cout << "Specie fitnessAverage " 			<< fitnessAverage 	<< endl;
	cout << "Specie amount of organisms" 		<< organisms.size() << endl;
	cout << "-------------------------------" 	<< endl;
}