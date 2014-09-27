#ifndef GLOBALINFORMATIONOFNEAT_HPP
#define GLOBALINFORMATIONOFNEAT_HPP

#include <vector>
#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

class globalInformationOfNEAT{
	public:
		void load(char pathUserDefinitions[]);
		double getOrganismDistanceW();
		double getOrganismDistanceD();
		double getOrganismDistanceE();
		double getDistanceThreshold();
		double getExpectativeOfPopulation();
		double getProbabilityOfInterspecieMating();
		double getProbabilityOfMutateNode();
		double getProbabilityOfMutateConnection();
		double getProbabilityOfMutateFunction();
		double getProbabilityOfMatingOnly();
		double getExpectativeOfLife();
		int getGenerations();
		void setOrganismDistanceW(double x);
		void setOrganismDistanceD(double x);
		void setOrganismDistanceE(double x);
		void setDistanceThreshold(double x);
		int newNodeNumber();
		int newInnovation();
		globalInformationOfNEAT(char pathUserDefinitions[]);
		vector < vector < int > > & getHistoricalNodeNumberReference();
		vector < vector < int > > & getHistoricalInnovationReference();
		vector <int>			  & getHistoricalLayerReference();
		vector <int> 			  & getTransformLayerInPositionOfLayerReference();	
		vector <int>			  & getGlobalLayerOrdererReference();
	private:
		int lastNode;
		vector < vector <int> > historicalNodeNumber;
		vector < int > globalLayerOrderer;
		vector < int > historicalLayer;
		vector < int > transformLayerInPositionOfLayer;
		vector < vector <int> >  historicalInnovation;
		int  lastInnovation;
		double 	organismDistanceW;
		double 	organismDistanceD;
		double 	organismDistanceE;
		double distanceThreshold;
		double expectativeOfPopulation;
		double probabilityOfInterspecieMating;
		double probabilityOfMutateNode;
		double probabilityOfMutateConnection;
		double probabilityOfMutateFunction;
		double probabilityOfMatingOnly;
		double expectativeOfLife;
		double expectativeOfSpecies;
		uint   generations; 
};
#endif	