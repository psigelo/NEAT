#ifndef NEAT_HPP
#define NEAT_HPP

#include "genetic_encoding.hpp"
#include <cmath>
#include <unistd.h>
#include <stdexcept>      // std::out_of_range
#include <list>
#include <algorithm>
#include "discrete_probabilities.hpp"


namespace ANN_USM{

	class Niche{
		public:
			bool 		    exist;
			int 		        years;
			double		 total_fitness;
			double 		total_shared_fitness;
			int 		niche_champion_position;
			int 		amount_of_offspring;
			vector <int> organism_position;

	};

	class Population{
		public:

			Population(char user_definitions[],char genetic_encoder[]);
			//	The order of the functions is the same that the time ocurrence.


			/*! \brief generate the new poblation and their niches
			*
			*
			*/
			void 				epoch();
			Genetic_Encoding 	epoch_reproduce(
									Genetic_Encoding 	organism,
									int 				poblation_place
									);
			void 				spatiation();
			Genetic_Encoding 	mutation_node 			(Genetic_Encoding organism);
			Genetic_Encoding 	mutation_create_new_node(Genetic_Encoding organism);
			Genetic_Encoding 	mutation_connection 	(Genetic_Encoding organism);
			Genetic_Encoding 	mutation_change_weight 	(Genetic_Encoding organism);
			/*!\brief Changes all weights of an genome randomly
			* \param organism is the genome that will change and will be returned.
			*/
			Genetic_Encoding 	put_randoms_weight 		(Genetic_Encoding organism);
			int 				obtain_historical_node(
									int initial_in,
									int initial_out
									);
			int 				obtain_innovation(
									int in,
									int out
									);
			int 				obtain_row(
									int node,
									int node_initial_in,
									int node_initial_out
									);
			double 				compatibility(
									Genetic_Encoding orgm1,
									Genetic_Encoding orgm2
									); // Distance between two ANNs
			Genetic_Encoding 	crossover(
									Genetic_Encoding orgm1,
									Genetic_Encoding orgm2
									);
			void 				save(char path[]);
			void 				print_niches();
			void 				load_user_definitions(char address[]);

			//============================ User definitions ================================//
			/**
				These variables are introduced through the user_definition file.
				These variables represent the probabilities, amount of genomes and amount of
				generations of NEAT is important to know that are absolutely relevant.
			*/
			int 	POPULATION_MAX;


			double 	ORGANISM_DISTANCE_1;
			double 	ORGANISM_DISTANCE_2;
			double 	ORGANISM_DISTANCE_3;



			double 	DISTANCE_THRESHOLD;
			double 	PERCENTAGE_OFFSPRING_WITHOUT_CROSSOVER;
			double 	PROBABILITY_INTERSPACIES_MATING;
			double 	SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE;
			double 	SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION;
			double 	LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE;
			double 	LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION;
			double 	PROB_ENABLE_AN_DISABLE_CONNECTION;
			double 	LARGE_POPULATION_DISCRIMINATOR;
			int 	GENERATIONS;
			double 	PROBABILITY_CHANGE_WEIGHT;
			double 	PROBABILITY_CHANGE_NODE_FUNCTION_PER_NODE;
			//==============================================================================//

			int 						lenght;
			vector<Genetic_Encoding> 	organisms;
			vector<Genetic_Encoding> 	prev_organisms;
			vector <Niche> 				current_niches;
			vector <Niche> 				prev_niches;
			int 						last_innovation;
			int 						last_node;
			int 						last_row;
			vector < vector<int> > 		historical_nodes;
			vector< vector<int> >  		historical_innovation;
			vector <int> 				historical_row;
			vector <int> 				row_orderer_list;
			Genetic_Encoding 			champion;
			double 						fitness_champion;
			list <int> organims_position_orderer_by_fitness;
	};
}

ostream & operator<<(ostream & o, ANN_USM::Population & pop);





#endif
