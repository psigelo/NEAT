#ifndef NEAT_CPP
#define NEAT_CPP

#include "NEAT.hpp"

using namespace ANN_USM;

Genetic_Encoding Population::mutation_node(Genetic_Encoding organism){
	int number_of_connections = organism.Lconnection_genes.size();
	int connection_to_mutate = round(rand()%number_of_connections);
	
	int verif = node_innovation_verificator(organism.Lconnection_genes[connection_to_mutate]);
	int verif2;

	organism.add_node(verif,HIDDEN);
	organism.Lconnection_genes[connection_to_mutate].enable=0;

	verif2 = innovation_verificator(organism.Lconnection_genes[connection_to_mutate].in, verif);
	organism.add_connection(verif2, organism.Lconnection_genes[connection_to_mutate].in, verif, 1.0);
	

	verif2 = innovation_verificator(verif, organism.Lconnection_genes[connection_to_mutate].out);
	organism.add_connection(verif2, verif, organism.Lconnection_genes[connection_to_mutate].out, organism.Lconnection_genes[connection_to_mutate].weight);
	
	
	return organism;
}

/*
int Population::node_innovation_verificator(connection_gene conn_gen){
	int size = innovation_to_node.size();
	if(innovation < size){
		if(innovation_to_node[conn_gen.innovation]<0){
			last_node = last_node + 1;
			innovation_to_node[conn_gen.innovation] = last_node;
			add_node_to_historical_nodes(conn_gen.in, conn_gen.out, last_node);
			return last_node;
		}
		else{
			return innovation_to_node[conn_gen.innovation];
		}
	}
	else{
		for(int i; i < conn_gen.innovation - size; i++){
			innovation_to_node.push_back(-1);
		}
		last_node = last_node + 1;
		innovation_to_node[conn_gen.innovation] = last_node;
		add_node_to_historical_nodes(conn_gen.in, conn_gen.out, last_node);
		return last_node;
	}

	

}
void Population::add_node_to_historical_nodes(int in, int out, int node){
	if(historical_nodes.size < in){
		for (int i = 0; i < in - historical_nodes.size ; ++i)
		{
			historical_nodes.push_back(-1);
		}
		for (int i = 0; i < out; ++i)
		{
			historical_nodes[in].push_back(-1);
		}
		historical_nodes[in][out] = node;
	}
	else{
		if(historical_nodes[in].size() < out){
			for (int i = 0; i < out-historical_nodes[in].size(); ++i)
			{
				historical_nodes[in].push_back(-1);
			}
			historical_nodes[in][out] = node;
		}
		else{
			if(historical_nodes[in][out] < 0){
				historical_nodes[in][out] = node;
			}
			else{
				cerr << "You cannot add an node, historical node already exist\n";
			}
		}
	}
}


int Population::innovation_verificator(int in, int out){
	if(historical_nodes.size < in){
		last_node
		return last_node;
	}
}

TODO MALO!!.

*/


#endif