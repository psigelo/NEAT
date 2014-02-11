#ifndef NEAT_HPP
#define NEAT_HPP

#include "genetic_encoding.hpp"

namespace ANN_USM{

	class Niche{
		public:
			int id;
			int years;
			vector<int> organism_position;

	};

	class Population{
		public:
			vector<Genetic_Encoding> organisms;
			void init_population();
			void spatiation();
			int total_niches; //number of niches
			vector <int> current_niches;
			Genetic_Encoding * best_organism;

			vector <int> innovation_to_node;
			vector < vector<int> > historical_nodes;
			vector< vector<int> > historical_innovation;

			Genetic_Encoding mutation_node(Genetic_Encoding organism);
			Genetic_Encoding mutation_create_new_node(Genetic_Encoding organism);
			Genetic_Encoding mutation_connection(Genetic_Encoding organism);

			int verification_of_historical_node(connection_gene);
			int last_innovation;
			int last_node;

			int node_innovation_verificator(int innovation);
			int innovation_verificator(int in, int out);
			void add_node_to_historical_nodes(int in, int out, int node);
	};
}

#endif