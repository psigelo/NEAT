/** \mainpage Neural Evolution of Augmenting Topologies (NEAT)
 *
 * \section neat_intro What is NEAT?
 *
 * NEAT stands for NeuroEvolution of Augmenting Topologies. It is a method for evolving artificial neural networks with a genetic algorithm. NEAT implements the idea that it is most effective to start evolution with small, simple networks and allow them to become increasingly complex over generations. That way, just as organisms in nature increased in complexity since the first cell, so do neural networks in NEAT. This process of continual elaboration allows finding highly sophisticated and complex neural networks. 
 *
 * For more information please visit the following <a href="http://www.cs.ucf.edu/~kstanley/neat.html">site</a>.
 * 
 * \section goals Our Goals
 *
 * We seek for an intuitive and easy-reading code in order to facilitate the understanding and usage of NEAT among experienced and/or unexperimented users.
 *
 * \section general_info General Information
 *  \author María José Escobar - <a href="mailto:mariajose.escobar@usm.cl?Subject=Dear%20Pascal" target="_top">mariajose.escobar@usm.cl</a>
 *  \author Pascl Sigel - <a href="mailto:pascal.sigel@alumnos.usm.cl?Subject=Dear%20Pascal" target="_top">pascal.sigel@alumnos.usm.cl</a>
 *  \author Oscar Silva - <a href="mailto:oscar.silva@alunos.usm.cl?Subject=Dear%20Oscar" target="_top">oscar.silva@alunos.usm.cl</a>
 *  \author Warren Eaton - <a href="mailto:warren.eaton@alumnos.usm.cl?Subject=Dear%20Oscar" target="_top">warren.eaton@alumnos.usm.cl</a>
 * \version 3.0
 * \date 2014
 */
#include "NEAT.hpp"
using namespace NEAT_USM;
// IDEA:
// At this moment exist a new population of organisms without species 
// and exist the previus organisms that have species
// for decide the specie of new organisms each of they have to be compared 
// with species of previus generation and if one of they have no specie then a new specie is created this one
// with this one like the species representant.
// Our added idea is that in the moment to compare the specie is compared firts with the father specie and then 
// randomly with other species (with no order). 
population::population(char pathUserDefinitions[], char pathGeneticEncoding[] )
	:informationForGenome(pathUserDefinitions)
	{
	geneticEncoding _organism(pathGeneticEncoding, &informationForGenome);
	int expectativeOfPopulation = (int)informationForGenome.getExpectativeOfPopulation(); // the first generation have deterministic number of childs.
	for (int i = 0; i < expectativeOfPopulation ; ++i){
		_organism.putRandomsWeight();
		geneticEncoding * newOrganismTmp = new geneticEncoding(_organism);
		newOrganisms.push_back(newOrganismTmp);
	}
	listSpecies.push_back(specie(specieId++, &informationForGenome));
	listSpecies.at(0).setRepresentant(newOrganisms.at(0));
	speciate();
	currenGeneration=0;
}
void population::speciate(){
	int amountOfOrganisms = (int)newOrganisms.size();
	for (int i = 0; i < amountOfOrganisms; ++i){
		if(newOrganisms.at(i)->getSpecie() == -1){ // Is not -1 if have an specie because can be of parents species.
		 	assignSpecie(*newOrganisms.at(i));
		 }
	}
	// If an specie have no organism is in extintion and is erased of the species in code.
	for(int i = 0; i < (int)listSpecies.size(); ++i){
		//listSpecies.at(i).printStatistics();
		if( listSpecies.at(i).getAmountOfOrganism() == 0){
			listSpecies.erase(listSpecies.begin() + i);
			i--;
		}
	}
}
void population::assignSpecie(geneticEncoding & organism){
	uint size = listSpecies.size();
	vector <uint> randomValue;

	for (uint i = 0; i < size; ++i)
		randomValue.push_back(i);

	while(randomValue.size()!=0){
		uint random = rand()%randomValue.size();
		if( organism % *listSpecies.at(randomValue.at(random)).getRepresentant() < informationForGenome.getDistanceThreshold() ){
			organism.setSpecie(listSpecies.at(randomValue.at(random)).getRepresentant()->getSpecie() );
			listSpecies.at(randomValue.at(random)).organismPushBack(&organism);
			return ;
		}
		randomValue.erase(randomValue.begin() + random);
	}
	//If not have any specie at then a new specie is created for
	listSpecies.push_back(specie(specieId++, &informationForGenome));
	listSpecies.back().setRepresentant(&organism);
	listSpecies.back().organismPushBack(&organism);
	
}
/*
	Because is the most important method, we try to write as simplified as we can do
	only ideas and method to do this ideas.

	steps:
		1) Eliminate by fitness.

*/
void population::epoch(){
	
	double expectativeOfPopulation;
	discreteProbabilities sackOfProbabilities; 
	geneticEncoding  * organismsRandomlyElected[3];
	newOrganisms.clear();
	newYear(); //Old organism can die with an probability
	computeStatistics();
	cerr << "1) listSpecies.at(0).getAmountOfOrganism(): " << listSpecies.at(0).getAmountOfOrganism() << endl;
	eliminateWorseOrganism(); 
	cerr << "2) listSpecies.at(0).getAmountOfOrganism(): " << listSpecies.at(0).getAmountOfOrganism() << endl;
	setOrganismProbabilities(sackOfProbabilities); 
	//Note that  organismRandomlyElecter :
		// [0] -> father
		// [1] -> mother
		// [2] -> mother if exist interspace mating
	expectativeOfPopulation = informationForGenome.getExpectativeOfPopulation();
	
	cerr <<  "Epoch 2" << endl;
	//while( rand()/(double)RAND_MAX < expectativeOfPopulation/(1 + expectativeOfPopulation) ){
	for(uint k=0; k < expectativeOfPopulation; k++){
		cerr << "Epoch 3 " << listSpecies.at(0).getAmountOfOrganism() << endl;
		getOrganismToRerpoduceRandomly(sackOfProbabilities, organismsRandomlyElected); 
	cerr << "Epoch 4" << endl;
		reproduce(organismsRandomlyElected);
	cerr << "Epoch 5" << endl;
	}
	speciate();
	cerr << "Epoch 6" << endl;
	currenGeneration++;
} 
void population::eliminateWorseOrganism(){ 
	int speciesAmount = listSpecies.size();
	for (int i = 0; i < speciesAmount; ++i)
		listSpecies.at(i).deleteWorseOrganisms();
}
void population::setOrganismProbabilities(discreteProbabilities & sackOfProbabilities){
	uint speciesAmount = listSpecies.size();
	double totalFitnessOfPopulation(0.0);
	for (uint i = 0; i < speciesAmount; ++i)
		totalFitnessOfPopulation += listSpecies.at(i).getTotalFitness();
	for (uint i = 0; i < speciesAmount; ++i){
		uint amountOfOrganisms = listSpecies.at(i).getAmountOfOrganism();
		for (uint j = 0; j < amountOfOrganisms; ++j){
			sackOfProbabilities.addOrganismProbability(listSpecies.at(i).getfitnessOfOrganismAt(j),i,j);
		}
	}
}
void population::getOrganismToRerpoduceRandomly(discreteProbabilities & sackOfProbabilities, geneticEncoding * parents[3]){
	organismProbabilities father = sackOfProbabilities.getFatherRandomly();
	parents[0] = listSpecies.at(father.specie).getPointerToOrganismAt(father.positionInSpecie);//father
	if( listSpecies.at(father.specie).getAmountOfOrganism() == 0 ){
		sackOfProbabilities.repositioningOrganism(father);
	}
	organismProbabilities mother =  sackOfProbabilities.getMotherRandomly(father.specie );
	parents[1] = listSpecies.at(mother.specie ).getPointerToOrganismAt(mother.positionInSpecie );//mother of the same specie
	if( listSpecies.at(father.specie).getAmountOfOrganism() == 0 ){
		sackOfProbabilities.repositioningOrganism(father);
	}
	organismProbabilities motherInterspecie =  sackOfProbabilities.getMotherInterSpecieRandomly(father.specie);
	parents[2] = listSpecies.at(motherInterspecie.specie).getPointerToOrganismAt(motherInterspecie.positionInSpecie);//mother of the same specie

	// Thereby the next time have all organisms 
	sackOfProbabilities.repositioningOrganism(father);
	sackOfProbabilities.repositioningOrganism(mother);
	sackOfProbabilities.repositioningOrganism(motherInterspecie);
}
void population::reproduce(geneticEncoding * parents[3]){
	geneticEncoding * child;
	if( rand()/(double)RAND_MAX < informationForGenome.getProbabilityOfInterspecieMating() )
		child = (*parents[0])*(*parents[2]); // where * is for crossover
	else
		child = (*parents[0])*(*parents[1]);
	if(rand()/(double)RAND_MAX < informationForGenome.getProbabilityOfMatingOnly()){ 
		newOrganisms.push_back(child);
		return;
	}
	else{
		if( ((double)rand()/RAND_MAX) < informationForGenome.getProbabilityOfMutateNode()){
			child->mutateNode();	
		}
		if( ((double)rand()/RAND_MAX) < informationForGenome.getProbabilityOfMutateConnection()) {
			child->mutateConnection();
		}
		if( ((double)rand()/RAND_MAX) < informationForGenome.getProbabilityOfMutateFunction()) {
			child->mutateFunction();
		}
	}

	newOrganisms.push_back(child);
}
void population::newYear(){
	int amountSpecies=listSpecies.size();
	for (int i = 0; i < amountSpecies; ++i)
		listSpecies.at(i).newYear();
}
void population::computeStatistics(){
	int amountSpecies=listSpecies.size();
	for (int i = 0; i < amountSpecies; ++i)
		listSpecies.at(i).computeStatistics();
}

void population::printStatistics(){
	int amountSpecies=listSpecies.size();
	cout << "===================================\nGeneration " << currenGeneration << endl;
	for (int i = 0; i < amountSpecies; ++i){
		listSpecies.at(i).printStatistics();
	}

}
int population::getGenerations(){
	return informationForGenome.getGenerations();
}
int population::getOrganismsAmount(){
	return (int)newOrganisms.size();
}

geneticEncoding * population::getPointerToNewOrganismAtPosition(int position){
	return newOrganisms.at(position);
}