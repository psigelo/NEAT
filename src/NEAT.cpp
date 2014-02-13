#ifndef NEAT_CPP
#define NEAT_CPP

#include "NEAT.hpp"

#define POPULATION_MAX 150

using namespace ANN_USM;

Genetic_Encoding Population::put_randoms_weight(Genetic_Encoding organism){
	for (int i = 0; i < (int)organism.Lconnection_genes.size(); ++i)
	{
		organism.Lconnection_genes[i].weight=2.0*(rand()%10000)/10000.0-1.0;
	}
	return organism;
}

Genetic_Encoding Population::mutation_change_weight(Genetic_Encoding organism){
	int number_of_connections = organism.Lconnection_genes.size();
	int connection_to_mutate = round(rand()%number_of_connections);
	double delta = 2*(rand()%10000)/10000.0-1;
	organism.Lconnection_genes[connection_to_mutate].weight=delta*0.3 + 0.7*organism.Lconnection_genes[connection_to_mutate].weight;
	return organism;
}

Genetic_Encoding Population::mutation_node(Genetic_Encoding organism){
	int number_of_connections = organism.Lconnection_genes.size();
	int connection_to_mutate = round(rand()%number_of_connections);
	
	int innov1;
	int innov2;

	// add node
	int node = obtain_historical_node(organism.Lconnection_genes[connection_to_mutate].in, organism.Lconnection_genes[connection_to_mutate].out);
	organism.add_node(node,HIDDEN);

	// add connections
	organism.Lconnection_genes[connection_to_mutate].enable=0; // disabling old connection.

	innov1 = obtain_innovation(organism.Lconnection_genes[connection_to_mutate].in, node);
	innov2 = obtain_innovation(node, organism.Lconnection_genes[connection_to_mutate].out);

	organism.add_connection(innov1, organism.Lconnection_genes[connection_to_mutate].in, node, 1.0);
	organism.add_connection(innov2, node, organism.Lconnection_genes[connection_to_mutate].out, organism.Lconnection_genes[connection_to_mutate].weight);
	
	return organism;
}

// RECORDAR: son inicial in y inicial out, esa es la idea.
int Population::obtain_historical_node(int in, int out){
	if((int)historical_nodes.size() <= in){
		for (int i = 0; i < in - (int)historical_nodes.size() + 1; ++i)		//REVIZAR EL +1
		{
			vector <int> temp;
			temp.push_back(-1);
			historical_nodes.push_back(temp);
		}
		for (int i = 0; i < out; ++i)
		{
			historical_nodes[in].push_back(-1);
		}
		last_node= last_node+1;
		historical_nodes[in][out] = last_node;
		return last_node;
	}
	else{
		if((int)historical_nodes[in].size() < out){
			for (int i = 0; i < out-(int)historical_nodes[in].size() +1; ++i)		//REVIZAR EL +1
			{
				historical_nodes[in].push_back(-1);
			}
			last_node = last_node +1;
			historical_nodes[in][out] = last_node;
			return last_node;
		}
		else{
			if(historical_nodes[in][out] < 0){
				last_node = last_node +1;
				historical_nodes[in][out] = last_node;
				return last_node;
			}
			else{
				return historical_nodes[in][out];
			}
		}
	}
}





int Population::obtain_innovation(int in, int out){
	

	if((int)historical_innovation.size() <= in){
		for (int i = 0; i < in - (int)historical_innovation.size() + 1; ++i)    //REVIZAR EL +1
		{
			vector <int> temp;
			temp.push_back(-1);
			historical_innovation.push_back(temp);
		}
		
		for (int i = 0; i < out; ++i)
		{
			historical_innovation[in].push_back(-1);
		}
		
		last_innovation= last_innovation+1;
		historical_innovation[in][out] = last_innovation;
		return last_innovation;
		
	}
	else{
		cout << "\n\n" << historical_innovation[0].size();
		if((int)historical_innovation[in].size() < out){
			for (int i = 0; i < out-(int)historical_innovation[in].size() + 1 ; ++i)		//REVIZAR EL +1
			{
				historical_innovation[in].push_back(-1);
			}
			last_innovation = last_innovation + 1;
			historical_innovation[in][out] = last_innovation;
			return last_innovation;
		}
		else{
			if(historical_innovation[in][out] < 0){
				last_innovation = last_innovation +1;
				historical_innovation[in][out] = last_innovation;
				return last_innovation;
			}
			else{
				return historical_innovation[in][out];
			}
		}
		
	}
	
	return -100;

}






Genetic_Encoding Population::mutation_create_new_node(Genetic_Encoding organism){
	int number_of_nodes = (int)organism.Lnode_genes.size();
	int count(0);
	int node;
	int node_in;
	int node_out;
	int last_node_temp;
	int innov1;
	int innov2;
	while(true){
		node_in = round(rand()%number_of_nodes);
		while(true){
			node_out = round(rand()%number_of_nodes);
			if(organism.Lnode_genes[node_out].type != INPUT && node_out != node_in) break;
		}
		last_node_temp = last_node;
		node = obtain_historical_node(node_in, node_out);
		if( node < last_node_temp){
			if(number_of_nodes < node){
				organism.add_node(node,HIDDEN);
				innov1 = obtain_innovation(node_in, node);
				innov2 = obtain_innovation(node, node_out);

				organism.add_connection(innov1, node_in, node, 2*(rand()%10000)/10000.0 - 1.0);
				organism.add_connection(innov2, node, node_out, 2*(rand()%10000)/10000.0 - 1.0);
				break;				
			}
			else{
				if(!organism.Lnode_genes[node].exist){
					organism.add_node(node,HIDDEN);
					innov1 = obtain_innovation(node_in, node);
					innov2 = obtain_innovation(node, node_out);

					organism.add_connection(innov1, node_in, node, 2*(rand()%10000)/10000.0 - 1.0);
					organism.add_connection(innov2, node, node_out, 2*(rand()%10000)/10000.0 - 1.0);
					break;
				}
			}
		}
		else{
			organism.add_node(node,HIDDEN);
			innov1 = obtain_innovation(node_in, node);
			innov2 = obtain_innovation(node, node_out);

			organism.add_connection(innov1, node_in, node, 2*(rand()%10000)/10000.0 - 1.0);
			organism.add_connection(innov2, node, node_out, 2*(rand()%10000)/10000.0 - 1.0);
			break;
		}

		count++;
		if(count>30){cerr << "ERROR:: In function mutation_create_new_node \n";break;}
	}
	return organism;
}


Genetic_Encoding Population::mutation_connection(Genetic_Encoding organism){
	int count(0);
	int node_in;
	int node_out;
	int last_innovation_temp;
	int innovation;
	int number_of_connections = (int)organism.Lconnection_genes.size();
	while(true){
		node_in = round(rand()%number_of_connections);
		while(true){
			node_out = round(rand()%number_of_connections);
			if(organism.Lnode_genes[node_out].type != INPUT && node_out != node_in) break;
		}

		
		last_innovation_temp = last_innovation;
		innovation = obtain_innovation(node_in, node_out);

		if( innovation < last_innovation_temp){
			if(number_of_connections < innovation){
				organism.add_connection(innovation,node_in, node_out, 2*(rand()%10000)/10000.0 - 1.0);
				break;				
			}
			else{
				if(!organism.Lconnection_genes[innovation].exist){
					organism.add_connection(innovation,node_in, node_out, 2*(rand()%10000)/10000.0 - 1.0);
					break;
				}
			}
		}
		else{
			organism.add_connection(innovation,node_in, node_out, 2*(rand()%10000)/10000.0 - 1.0);
			break;
		}		


		count++;
		if(count>30){cerr << "ERROR:: In function mutation_connection \n";break;}
	}
	return organism;
}

void Population::init_population(char path[]){
	Genetic_Encoding _organism;
	_organism.load(path);
	
	for (int i = 0; i < (int)_organism.Lconnection_genes.size(); ++i)
		obtain_innovation(_organism.Lconnection_genes[i].in, _organism.Lconnection_genes[i].out); 
	last_node = (int)_organism.Lnode_genes.size()-1;
	
	for (int i = 0; i < POPULATION_MAX; ++i)
		organisms.push_back(put_randoms_weight(_organism)); //
	lenght = POPULATION_MAX;
	
}


ostream & operator<<(ostream & o, ANN_USM::Population & pop) { 
	for (int i = 0; i < pop.lenght; ++i)
		o << pop.organisms[i];
		
	return o;
}

#endif