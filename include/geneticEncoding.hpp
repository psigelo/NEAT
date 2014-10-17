#ifndef geneticEncoding_HPP
#define geneticEncoding_HPP


#include <fstream>
#include <cmath>
#include <cstring>
#include <vector>
#include <iostream>
#include "randomFunction.hpp"
#include "connectionGene.hpp"
#include "nodeGene.hpp"
#include "globalInformationOfNEAT.hpp"

using namespace std;

namespace NEAT_USM{
	class geneticEncoding{
		public:
			geneticEncoding();
			/**
			 \brief Copy constructor
			*/
			geneticEncoding( const geneticEncoding & genetic_encoding  );
			/**
			*	\brief load  a genome from file and set this organism variables.
			*	\param path is where the file is stored.
			*/
			geneticEncoding(char path[], globalInformationOfNEAT * informationOfPopulation);
			/**
			* \brief add a node to the genome.
			* \param node: an node that already exist.
			*/
			void 				addNode(nodeGene node);
			/**
			* \brief add a node to the genome.
			* \param node: an node that already exist.
			*/
			void 				addConnection(connectionGene connection);
			/**
			* \brief store the genome in file in json format.
			* \param path: Is where the file is stored.
			*/
			void 				save(char path[]); // save to a file
			/**
			* \brief load  a genome from file and set this organism variables.
			* \param path: Is where the file is stored.
			*/
			void 				load(char path[]); // Load an geneticEncoding from a file.
			/**
			*	\brief Evals the organism.
			*	\param vector <double> inputs are the inputs to the neural network 
			*/
			vector <double> 	eval(vector <double> inputs); 
			/**
			*	\brief Put all weights randomly
			*/
			void putRandomsWeight();
			/**
			*	\brief Mutate weight
			*/
			void mutationChangeWeight(double probabilityChangeWeight);
			/*
			* 	\brief Can be created a node with a probability.
			*/
			void mutateNode();
			void mutateConnection();
			void mutateFunction();
			int  obtainHistoricalNodeNumber(int initial_in, int initial_out) ;
			int  obtainInnovation(int initial_in, int initial_out);
			int  obtainLayer(int historicalNodeNumber, int layerNodeInitialIn, int layerNodeInitialOut);
			void changeConnection(connectionGene &);
			void changeNode(nodeGene &);
			void setSpecie(int niche);
			int  getSpecie();
			void printHumanReadable();
			void printLayersOnly();
			double getFitness();
			void   setFitness(double fitness);

			double getSharedFitness();
			void   setSharedFitness(double x);

			int 	getFatherSpecie();
			void 	setFatherSpecie(int fatherSpecie);
			int 	getMotherSpecie();
			void 	setMotherSpecie(int motherSpecie);
			int 	getYear();
			void 	setYear(int year);
			void 	incrementYear();

			friend ostream & operator<<(ostream & o, NEAT_USM::geneticEncoding & encoding);
			/**
				\brief Is the crossover function bewteen two organisms.
			*/
			friend geneticEncoding * operator * (NEAT_USM::geneticEncoding & encoding1, NEAT_USM::geneticEncoding & encoding2);
			/**
				\brief Is the compatibility function bewteen two organisms.
			*/
			friend double operator % (NEAT_USM::geneticEncoding & encoding1, NEAT_USM::geneticEncoding & encoding2);

		private:
			//vector < int >							globalLayerOrderer;
			vector < int >							presentLayers; // presentLayer.at(layer) = 1 if is present and 0 if is not present	
			vector < int >							input_pointer;
			vector < int >							output_pointer;
			vector < int >							transformHistoricalNodeToListNodeGenes;// Is used in Crossover functions
																							// trasnsformHistoricalNodeToListNodeGenes.at( historicalNode ) = position in listNodeGenes
			vector < int >							transformInnovationToListConnectionGenes; 	// Is used in Crossover functions
																								// transformInnovationToListConnectionGenes.at( innovation ) = position in listConnectionGenes
			vector <  connectionGene  > 			listConnectionGenes; //List of connections genes
			vector <  nodeGene  > 					listNodeGenes;
			vector < vector < int > >				listNodesAtLayer;
			vector < vector < int > >				listConnectionTowardSpecificLayer; // listConnectionTowardSpecificLayer.at(  ) = 

			double 									fitness;
			double 									sharedFitness;
			int 									specie;
			int 									fatherSpecie;
			int 									motherSpecie;
			vector < int > 							layerOrdererList; // only with layers that are present in gnome.
			int 									amountInputs;
			int 									amountOutputs;
			int 									amountHidden;
			int 									ExpectativeOfAmountOfConnections;
			int 									ExpectativeOfAmountOfNodes;
			vector < int >							connectionsThatCanMutateNodes;// Is used in mutateNode
																				  // connectionsThatCanMutateNodes.at(position) = positionInListConnections	
			vector < vector < int > >				connectionsThatCanBeMutated;// Is used in mutateConnection
																				//  connectionsThatCanBeMutated.at(position) = vector that have node in, node out. 			bool									isCppn;
			bool									ableDisableConnections;
			globalInformationOfNEAT *				NEATinformation;
			int 									year;

	};
}


#endif
