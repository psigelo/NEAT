#ifndef NEAT_CPP
#define NEAT_CPP

#include "NEAT.hpp"


/*
* 	USER DEFINITIONS
*/
#define POPULATION_MAX 150
#define DISTANCE_CONST_1 1.0
#define DISTANCE_CONST_2 0.4
#define DISTANCE_CONST_3 3.0
#define DISTANCE_CONST_4 1.0
#define DISTANCE_THRESHOLD 3.0

#define PERCENT_SMALLER_MUTATION_CONNECTION 0.25
#define PERCENTAGE_OFFSPRING_WITHOUT_CROSSOVER 25
#define PROBABILITY_INTERSPACIES_MATING 0.01
#define SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE 0.002
#define SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION 0.05
#define LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE 0.01
#define LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION 0.3
//falta la del disable
#define PROBABILITY_CONNECTION_WEIGHT_MUTATING 80
#define LARGE_POPULATION_DISCRIMINATOR (POPULATION_MAX/10.0)


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




























double Population::compatibility(Genetic_Encoding orgm1, Genetic_Encoding orgm2){
	double distance;
	int E(0);
	int D(0);
	double W(0);
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










































//Esta funcion se tiene que mejorar, además se tiene que agregar la opción de utulizar la distancia respecto a los mejores de las generaciones antereiores.
void Population::spatiation(){ // Hay que optimizar esta función
	prev_niches = current_niches;
	Niche aux_niche;

	current_niches.clear();
	aux_niche.exist=false;
	for(int i=0; i < (int)prev_niches.size(); i++){
		current_niches.push_back(aux_niche);
	}

	// ABSOLUTAMENTE POCO EFICIENTE ESTO !!!!!!
	bool have_niche;
	int amount_of_new_niches(0);
	for (int j = 0; j < POPULATION_MAX; ++j)
	{
		have_niche=false;
		for (int i = 0; i < (int)prev_niches.size() + amount_of_new_niches; ++i)
		{ 
			if (i < (int)prev_niches.size())
			{
				if(prev_niches[i].exist){				
					if ( compatibility(organisms[j],prev_organisms[prev_niches[i].niche_champion_position ]) <  DISTANCE_THRESHOLD)
					{
						have_niche=true;
						if (current_niches[i].exist)
						{
							current_niches[i].organism_position.push_back(j);
						}
						else{
							current_niches[i].exist=true;
							current_niches[i].organism_position.push_back(j);
						}
						break;
					}
				}
			}
			else{
				if ( compatibility(organisms[j],organisms[current_niches[i].niche_champion_position]) <  DISTANCE_THRESHOLD ){
					current_niches[i].organism_position.push_back(j);
					have_niche=true;
					break;
				}
			}
		}
		if(!have_niche){
			aux_niche.organism_position.clear();
			aux_niche.exist=true;
			aux_niche.niche_champion_position=(int)prev_niches.size() + amount_of_new_niches;
			aux_niche.organism_position.push_back((int)prev_niches.size() + amount_of_new_niches);
			amount_of_new_niches++;
			current_niches.push_back(aux_niche);
		}
	}
}












































void Population::init_population(char path[]){
	
	Genetic_Encoding _organism;
	_organism.load(path);
	_organism.niche=0;

	fitness_champion = 0;
	champion = _organism;

	for (int i = 0; i < (int)_organism.Lconnection_genes.size(); ++i)
		obtain_innovation(_organism.Lconnection_genes[i].in, _organism.Lconnection_genes[i].out); 
	last_node = (int)_organism.Lnode_genes.size()-1;
	
	for (int i = 0; i < POPULATION_MAX; ++i)
		organisms.push_back(put_randoms_weight(_organism)); //
	lenght = POPULATION_MAX;



	prev_organisms.push_back(_organism);
	Niche niche_temp;
	niche_temp.organism_position.push_back(0);
	niche_temp.exist=true;
	niche_temp.niche_champion_position=0;
	current_niches.push_back(niche_temp);
	spatiation();




}
































void Population::print_niches(){
	for (int i = 0; i < (int)current_niches.size(); ++i)
	{
		if(current_niches[i].exist){
			cout << "Niche " << i << ":\t\t\t" <<  current_niches[i].organism_position.size() << endl;
		}
		else{
			cout << "Niche " << i << ":\t\t\t0\n";
		}
	}
}


























void Population::epoch(){

	double total_shared_fitness_population(0.0);
	double fitness_temp;
	double fitness_max(0);
	int mutation_amount; 
	int random_organism;
	int random_niche;
	int random_father;
	int random_mother; // for mating
	Genetic_Encoding organism_temp;
	Genetic_Encoding organism_father;
	Genetic_Encoding organism_mother;// for mating
	// Falta eliminar a los más malos de cada generación
	for (int i = 0; i < (int)current_niches.size(); ++i)
	{
		if(current_niches[i].exist){
			current_niches[i].total_fitness=0;
			for (int j = 0; j < (int)current_niches[i].organism_position.size(); ++j)
			{
				fitness_temp=fitness(organisms[current_niches[i].organism_position[j]]);
				if(j==0 || fitness_temp > fitness_max)
				{
					fitness_max=fitness_temp;
					current_niches[i].niche_champion_position = j;
				}
				current_niches[i].total_fitness+=fitness_temp;

				if(fitness_temp > fitness_champion){ // Champion over all generations.
					champion = organisms[current_niches[i].organism_position[j]];
					fitness_champion=fitness_temp;
				}
			}
			total_shared_fitness_population += current_niches[i].total_fitness/(double)current_niches[i].organism_position.size(); 
		}
	}



	prev_organisms = organisms;
	organisms.clear();

	
	// Generate the offspring, then mutate them.
	for (int i = 0; i < (int)current_niches.size(); ++i)
	{
		if(current_niches[i].exist){
			current_niches[i].amount_of_offspring=round(POPULATION_MAX*total_shared_fitness_population/(current_niches[i].total_fitness/current_niches[i].organism_position.size())  + 1.0);
			for (int j = 0; j < current_niches[i].amount_of_offspring; ++j)
			{
				if(j==0){ // all niche champions pass throght generations.
					organisms.push_back(prev_organisms[current_niches[i].niche_champion_position]);
				}

				if(rand()%100 < PERCENTAGE_OFFSPRING_WITHOUT_CROSSOVER){
					random_organism = rand()%current_niches[i].organism_position.size();
					mutation_amount = rand()%(int)round(prev_organisms[random_organism].Lconnection_genes.size()*PERCENT_SMALLER_MUTATION_CONNECTION) + 1; 
					organism_temp = prev_organisms[random_organism];
					for(int k = 0; k < mutation_amount; k++)
						organism_temp = mutation_change_weight(organism_temp);

				}
				else{
					if( (rand()%1000)/1000.0 < PROBABILITY_INTERSPACIES_MATING){
						while(1){
							random_niche = rand()%prev_niches.size();
							if(prev_niches[random_niche].exist)break;
						}
						random_father = rand()%current_niches[i].organism_position.size();
						organism_father = prev_organisms[random_father];
						random_mother = rand()%current_niches[random_niche].organism_position.size();
						organism_mother = prev_organisms[random_mother];
						organism_temp = crossover(organism_father, organism_mother);
					}
					else{
						random_father = rand()%current_niches[i].organism_position.size();
						organism_father = prev_organisms[random_father];
						while (1){
							random_mother = rand()%current_niches[i].organism_position.size();
							if(random_mother!=random_father)break;
						}
						organism_mother = prev_organisms[random_mother];
						organism_temp = crossover(organism_father, organism_mother);
					}
				}




				if(rand()%100 < LARGE_POPULATION_DISCRIMINATOR ){ // enter if is a small niche
					if((rand()%1000)/1000.0 < SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE){
						organism_temp = mutation_node(organism_temp);
					}
					if((rand()%1000)/1000.0 < SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION) {
						organism_temp = mutation_connection(organism_temp);
					}
				}

				else{// enter if is a large niche
					if((rand()%1000)/1000.0 < LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE){
						organism_temp = mutation_node(organism_temp);
					}
					if((rand()%1000)/1000.0 < LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION) {
						organism_temp = mutation_connection(organism_temp);
					}
				}

				organisms.push_back(organism_temp);

			}
		}
	}


}






#endif