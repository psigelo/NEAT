#ifndef NEAT_HPP
#define NEAT_HPP

#include "genetic_encoding.hpp"
#include <cmath>
namespace ANN_USM{

	class Niche{
		public:
			int id;
			int years;
			vector<int> organism_position;

	};

	class Population{
		public:
			int lenght;
			vector<Genetic_Encoding> organisms;
			void init_population(char path[]);
			void spatiation();
			int total_niches; //number of niches
			vector <int> current_niches;
			Genetic_Encoding * best_organism;

			vector < vector<int> > historical_nodes;
			vector< vector<int> > historical_innovation;

			Genetic_Encoding mutation_node(Genetic_Encoding organism);
			Genetic_Encoding mutation_create_new_node(Genetic_Encoding organism);
			Genetic_Encoding mutation_connection(Genetic_Encoding organism);
			Genetic_Encoding mutation_change_weight(Genetic_Encoding organism);
			Genetic_Encoding put_randoms_weight(Genetic_Encoding organism);

			int last_innovation;
			int last_node;

			int obtain_historical_node(int initial_in, int initial_out);
			int obtain_innovation(int in, int out);
	};
}

ostream & operator<<(ostream & o, ANN_USM::Population & pop);

#endif