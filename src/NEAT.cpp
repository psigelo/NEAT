#ifndef NEAT_CPP
#define NEAT_CPP

#include "NEAT.hpp"

#define POPULATION_MAX 150
#define DISTANCE_CONST_1 1.0
#define DISTANCE_CONST_2 0.4
#define DISTANCE_CONST_3 3.0
#define DISTANCE_CONST_4 1.0
#define DISTANCE_THRESHOLD 3.0

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
	_organism.niche=0;
	
	



	for (int i = 0; i < (int)_organism.Lconnection_genes.size(); ++i)
		obtain_innovation(_organism.Lconnection_genes[i].in, _organism.Lconnection_genes[i].out); 
	last_node = (int)_organism.Lnode_genes.size()-1;
	
	for (int i = 0; i < POPULATION_MAX; ++i)
		organisms.push_back(put_randoms_weight(_organism)); //
	lenght = POPULATION_MAX;




/*
	int last_niche_id = 0;
	Niche p_niche;

	for (int i = 0; i < POPULATION_MAX; ++i)
	{
		for(int j; j< (int)current_niche.size() + (int)prev_niche.size();j++){
			if(compatibility(organisms[i],_organism)){
				if(j < prev_niche.size()){
					
				}
				else{

				}
			}
		}
	}
*/




}




ostream & operator<<(ostream & o, ANN_USM::Population & pop) { 
	for (int i = 0; i < pop.lenght; ++i){
		o << pop.organisms[i];
		o << "\n";
	}
	return o;
}




void Population::save(char path[]){
	ofstream file;
	file.open (path);
	file << *this;
	file.close();
}



int Population::compatibility(Genetic_Encoding orgm1, Genetic_Encoding orgm2){
	double distance;
	int E(0);
	int D(0);
	int W(0);
	int size_1 = orgm1.Lconnection_genes.size();
	int size_2 = orgm2.Lconnection_genes.size();
	int limit_sup;
	int limit_inf;

	limit_sup = (size_1 > size_2) ? size_2 : size_1;
	limit_inf = (size_1 < size_2) ? size_1 : size_2;

	for (int i = 0; i < limit_inf; ++i)
		if(orgm1.Lconnection_genes[i].exist && orgm2.Lconnection_genes[i].exist)
			W += abs(orgm1.Lconnection_genes[i].weight - orgm2.Lconnection_genes[i].weight); 
		else if(orgm1.Lconnection_genes[i].exist || orgm2.Lconnection_genes[i].exist)
			D+=1;
		
	for (int i = limit_inf; i < limit_sup; ++i)
		if( (size_1<size_2) ? orgm2.Lconnection_genes[i].exist : orgm1.Lconnection_genes[i].exist)
			E+=1;

	distance = DISTANCE_CONST_1*E/DISTANCE_CONST_4 + DISTANCE_CONST_2*D/DISTANCE_CONST_4 + DISTANCE_CONST_3*W;
	return distance;
}



Genetic_Encoding Population::crossover(Genetic_Encoding orgm1, Genetic_Encoding orgm2){
	Genetic_Encoding orgm_resutl;

	int conn_size_1 = orgm1.Lconnection_genes.size();
	int conn_size_2 = orgm2.Lconnection_genes.size();
	int conn_limit_sup = (conn_size_1 > conn_size_2) ? conn_size_2 : conn_size_1;
	int conn_limit_inf = (conn_size_1 < conn_size_2) ? conn_size_1 : conn_size_2;

	int node_size_1 = orgm1.Lnode_genes.size();
	int node_size_2 = orgm2.Lnode_genes.size();
	int node_limit_sup = (node_size_1 > node_size_2) ? node_size_2 : node_size_1;
	int node_limit_inf = (node_size_1 < node_size_2) ? node_size_1 : node_size_2;
	



	for (int i = 0; i < node_limit_inf; ++i)
		if(orgm1.Lnode_genes[i].exist && orgm2.Lnode_genes[i].exist)
			(rand()%10 >= 5) ? orgm_resutl.add_node(orgm1.Lnode_genes[i]) : orgm_resutl.add_node(orgm2.Lnode_genes[i]);
		else if(orgm1.Lnode_genes[i].exist || orgm2.Lnode_genes[i].exist)
			(orgm1.Lnode_genes[i].exist) ? orgm_resutl.add_node(orgm1.Lnode_genes[i]) : orgm_resutl.add_node(orgm2.Lnode_genes[i]);
		
	for (int i = node_limit_inf; i < node_limit_sup; ++i)
		if( (node_size_1 < node_size_2) ? orgm2.Lnode_genes[i].exist : orgm1.Lnode_genes[i].exist)
			(orgm1.Lnode_genes[i].exist) ? orgm_resutl.add_node(orgm1.Lnode_genes[i]) : orgm_resutl.add_node(orgm2.Lnode_genes[i]);




	for (int i = 0; i < conn_limit_inf; ++i)
		if(orgm1.Lconnection_genes[i].exist && orgm2.Lconnection_genes[i].exist)
			(rand()%10 >= 5) ? orgm_resutl.add_connection(orgm1.Lconnection_genes[i]) : orgm_resutl.add_connection(orgm2.Lconnection_genes[i]);
		else if(orgm1.Lconnection_genes[i].exist || orgm2.Lconnection_genes[i].exist)
			(orgm1.Lconnection_genes[i].exist) ? orgm_resutl.add_connection(orgm1.Lconnection_genes[i]) : orgm_resutl.add_connection(orgm2.Lconnection_genes[i]);
		
	for (int i = conn_limit_inf; i < conn_limit_sup; ++i)
		if( (conn_size_1<conn_size_2) ? orgm2.Lconnection_genes[i].exist : orgm1.Lconnection_genes[i].exist)
			(orgm1.Lconnection_genes[i].exist) ? orgm_resutl.add_connection(orgm1.Lconnection_genes[i]) : orgm_resutl.add_connection(orgm2.Lconnection_genes[i]);

	return orgm_resutl;
}


void Population::spatiation(){

}



#endif