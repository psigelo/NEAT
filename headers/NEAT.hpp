#ifndef NEAT_HPP
#define NEAT_HPP

#include "genetic_encoding.hpp"
#include "fitness.hpp"
#include <cmath>
#include <unistd.h>

namespace ANN_USM{

	class Niche{
		public:
			bool exist;
			int years;
			double	total_fitness;
			int niche_champion_position;
			int amount_of_offspring;
			vector<int> organism_position;

	};

	class Population{
		public:
			int lenght;
			vector<Genetic_Encoding> organisms;
			vector<Genetic_Encoding> prev_organisms;
			void init_population(char path[]);
			void spatiation();
			int total_niches; //number of niches


			vector <Niche> current_niches;
			vector <Niche> prev_niches;

			int last_niche_id;

			Genetic_Encoding * best_organism;

			vector < vector<int> > historical_nodes;
			vector< vector<int> > historical_innovation;
			vector <int> historical_row;

			vector <int> row_orderer_list;

			Genetic_Encoding mutation_node(Genetic_Encoding organism);
			Genetic_Encoding mutation_create_new_node(Genetic_Encoding organism);
			Genetic_Encoding mutation_connection(Genetic_Encoding organism);
			Genetic_Encoding mutation_change_weight(Genetic_Encoding organism);
			Genetic_Encoding put_randoms_weight(Genetic_Encoding organism);


			int last_innovation;
			int last_node;
			int last_row;

			int obtain_historical_node(int initial_in, int initial_out);
			int obtain_innovation(int in, int out);
			int obtain_row(int node, int node_initial_in, int node_initial_out);

			double compatibility(Genetic_Encoding orgm1, Genetic_Encoding orgm2); // Distance between two ANNs
			Genetic_Encoding crossover(Genetic_Encoding orgm1, Genetic_Encoding orgm2);

			void save(char path[]);
			void print_niches();
			void epoch();

			Genetic_Encoding champion;
			double fitness_champion;
	};
}

ostream & operator<<(ostream & o, ANN_USM::Population & pop);

#endif