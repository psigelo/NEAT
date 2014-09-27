#ifndef NEAT_HPP
#define NEAT_HPP
#include "geneticEncoding.hpp"
#include <cmath>
#include <unistd.h>
#include <stdexcept>      // std::out_of_range
#include <list>
#include "specie.hpp"
#include "globalInformationOfNEAT.hpp"
#include "discreteProbabilities.hpp"
namespace NEAT_USM{
	class population{
		public:
			population(char pathUserDefinitions[], char pathGeneticEncoding[] );
			//	The order of the functions is the same that the time ocurrence.
			/*! \brief generate the new poblation and their niches
			*
			*
			*/
			void 				epoch();
			void 				reproduce(geneticEncoding * parents[3]);
			void 				speciate();
			void 				assignSpecie(geneticEncoding & organism);
			void 				load_user_definitions(char address[]);
			/**
			\brief 	Imprime toda la generacion a archivo, en la carpeta Stattistics
			\param generation corresponde a el nombre de la generacion para su mas posterior
			*/
			void 				save(char path[]);
			void 				printToFileCurrrentGeneration(char path[]);
			void 				newYear();
			void 				eliminateWorseOrganism();
			void 				computeStatistics();
			void 				setOrganismProbabilities( discreteProbabilities & sackOfProbabilities);
			void 				getOrganismToRerpoduceRandomly(discreteProbabilities & sackOfProbabilities, geneticEncoding * parents[3]);
			void 				printStatistics();
			int 				getGenerations();
			int					getOrganismsAmount();
			geneticEncoding * 	getPointerToNewOrganismAtPosition(int position);

		private:			
			vector <int> 							newOrganismsPosition;
			vector < geneticEncoding *>				newOrganisms;
			vector <specie>							listSpecies;
			geneticEncoding 						champion;
			double 									fitnessChampion;
			char *									name;		
			char *									savePath;	
			int 									current_generation;	
			globalInformationOfNEAT 				informationForGenome;
			int 									specieId;
			int 									currenGeneration;
	};
}
ostream & operator<<(ostream & o, NEAT_USM::population & pop);
#endif