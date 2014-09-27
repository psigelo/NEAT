#include "globalInformationOfNEAT.hpp"

globalInformationOfNEAT::globalInformationOfNEAT(char pathUserDefinitions[]){
	lastNode = 0;
	lastInnovation = 0;
	load(pathUserDefinitions);
}
int globalInformationOfNEAT::newNodeNumber(){
	return lastNode++;
}
int globalInformationOfNEAT::newInnovation(){
	return lastInnovation++;
}
vector < vector < int > > & globalInformationOfNEAT::getHistoricalNodeNumberReference(){
	return historicalNodeNumber;
}
vector < vector < int > > & globalInformationOfNEAT::getHistoricalInnovationReference(){
	return historicalInnovation;
}
vector <int> & globalInformationOfNEAT::getHistoricalLayerReference(){
	return historicalLayer;
}
vector <int> & globalInformationOfNEAT::getTransformLayerInPositionOfLayerReference(){
	return transformLayerInPositionOfLayer;
}
vector <int> & globalInformationOfNEAT::getGlobalLayerOrdererReference(){
	return globalLayerOrderer;
}
double globalInformationOfNEAT::getOrganismDistanceW(){
	return organismDistanceW;
}
double globalInformationOfNEAT::getOrganismDistanceD(){
	return organismDistanceD;
}
double globalInformationOfNEAT::getOrganismDistanceE(){
	return organismDistanceE;
}
double globalInformationOfNEAT::getExpectativeOfLife(){
	return expectativeOfLife;
}
double globalInformationOfNEAT::getDistanceThreshold(){
	return distanceThreshold;
}
int globalInformationOfNEAT::getGenerations(){
	return generations;
}
void globalInformationOfNEAT::setOrganismDistanceW(double x){
	organismDistanceW=x;
}
void globalInformationOfNEAT::setOrganismDistanceD(double x){
	organismDistanceD=x;
}
void globalInformationOfNEAT::setOrganismDistanceE(double x){
	organismDistanceE=x;
}

double globalInformationOfNEAT::getExpectativeOfPopulation(){
	return expectativeOfPopulation;
}
double globalInformationOfNEAT::getProbabilityOfInterspecieMating(){
	return probabilityOfInterspecieMating;
}

double globalInformationOfNEAT::getProbabilityOfMutateNode(){
	return probabilityOfMutateNode;
}

double globalInformationOfNEAT::getProbabilityOfMutateConnection(){
	return probabilityOfMutateConnection;
}

double globalInformationOfNEAT::getProbabilityOfMutateFunction(){
	return probabilityOfMutateFunction;
}

double globalInformationOfNEAT::getProbabilityOfMatingOnly(){
	return probabilityOfMatingOnly;
}

void globalInformationOfNEAT::load(char pathUserDefinitions[]){
	ifstream file (pathUserDefinitions);
	file.seekg (0, file.end);
	int length = file.tellg();
	file.seekg (0, file.beg);
	char buffer[length]; // In JSON format
	file.read (buffer,length);
	file.close();
	char * pch;
	char delimiters[] = " \n\":\t{},[";
	pch = strtok (buffer,delimiters);
	pch = strtok(NULL,delimiters);
	organismDistanceW = atof(pch);
	pch = strtok(NULL,delimiters);
	pch = strtok(NULL,delimiters);
	organismDistanceD = atof(pch);
	pch = strtok(NULL,delimiters);
	pch = strtok(NULL,delimiters);
	organismDistanceE = atof(pch);
	pch = strtok(NULL,delimiters);
	pch = strtok(NULL,delimiters);
	distanceThreshold = atof(pch);
	pch = strtok(NULL,delimiters);
	pch = strtok(NULL,delimiters);
	expectativeOfPopulation = atof(pch);
	pch = strtok(NULL,delimiters);
	pch = strtok(NULL,delimiters);
	probabilityOfInterspecieMating = atof(pch);
	pch = strtok(NULL,delimiters);
	pch = strtok(NULL,delimiters);
	probabilityOfMutateNode = atof(pch);
	pch = strtok(NULL,delimiters);
	pch = strtok(NULL,delimiters);
	probabilityOfMutateConnection = atof(pch);
	pch = strtok(NULL,delimiters);
	pch = strtok(NULL,delimiters);
	probabilityOfMutateFunction = atof(pch);
	pch = strtok(NULL,delimiters);
	pch = strtok(NULL,delimiters);
	probabilityOfMatingOnly = atof(pch);
	pch = strtok(NULL,delimiters);
	pch = strtok(NULL,delimiters);
	expectativeOfLife = atof(pch);
	pch = strtok(NULL,delimiters);
	pch = strtok(NULL,delimiters);
	expectativeOfSpecies = atof(pch);
	pch = strtok(NULL,delimiters);
	pch = strtok(NULL,delimiters);
	generations = (uint) atoi(pch);
}


