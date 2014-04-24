#ifndef NEAT_HPP
#define NEAT_HPP

#include "genetic_encoding.hpp"
#include "fitness.hpp"
#include <cmath>
#include <unistd.h>


namespace ANN_USM{

	class Niche{
		public:
			bool 		exist;
			int 		years;
			double		total_fitness;
			double 		total_shared_fitness;
			int 		niche_champion_position;
			int 		amount_of_offspring;
			vector<int> organism_position;

	};

	class Population{
		public:
			

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
			void 				init_population(char path[]);
			void 				spatiation();
			Genetic_Encoding 	mutation_node 			(Genetic_Encoding organism);
			Genetic_Encoding 	mutation_create_new_node(Genetic_Encoding organism);
			Genetic_Encoding 	mutation_connection 	(Genetic_Encoding organism);
			Genetic_Encoding 	mutation_change_weight 	(Genetic_Encoding organism);
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
			double 	DISTANCE_CONST_1;
			double 	DISTANCE_CONST_2;
			double 	DISTANCE_CONST_3;
			double 	DISTANCE_CONST_4;
			double 	DISTANCE_THRESHOLD;
			double 	PERCENT_MUTATION_CONNECTION;
			double 	PERCENTAGE_OFFSPRING_WITHOUT_CROSSOVER;
			double 	PROBABILITY_INTERSPACIES_MATING;
			double 	SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE;
			double 	SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION;
			double 	LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE;
			double 	LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION;
			double 	PROB_ENABLE_AN_DISABLE_CONNECTION;
			double 	LARGE_POPULATION_DISCRIMINATOR;
			int 	GENERATIONS;
			double 	SIGMOID_CONSTANT;
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
	};
}

ostream & operator<<(ostream & o, ANN_USM::Population & pop);

#endif