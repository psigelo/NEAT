#ifndef SPECIE_HPP
#define SPECIE_HPP
#include "globalInformationOfNEAT.hpp"
#include "geneticEncoding.hpp"

namespace NEAT_USM{
	class specie{
		public:
			specie(int id, globalInformationOfNEAT * information);
			void	organismPushBack( geneticEncoding * organism);
			geneticEncoding * getOrganismAt( int position);
			void	setRepresentant( geneticEncoding * organism);
			geneticEncoding * getRepresentant();
			uint	getAmountOfOrganism();
			void	deleteWorseOrganisms();
			double	getfitnessOfOrganismAt(int position);
			int 	getId();
			geneticEncoding * getPointerToOrganismAt(int organismPosition);
			void	newYear();
			double	getTotalFitness();
			void	computeStatistics();
			void	printStatistics();
		private:
			int				id;
			int				age;
			double			totalFitness;
			double			totalSharedFitness;
			int				nicheChampionPosition;
			int				amountOfOffspring;
			double			fitnessAverage;
			vector < geneticEncoding *>	organisms;
			geneticEncoding * representant;
			globalInformationOfNEAT * globalInformation;
	};
}
#endif