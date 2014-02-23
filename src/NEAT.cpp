#ifndef NEAT_CPP
#define NEAT_CPP

#include "NEAT.hpp"


/*
* 	USER DEFINITIONS
*/
#define POPULATION_MAX 100
#define DISTANCE_CONST_1 1.0
#define DISTANCE_CONST_2 0.4
#define DISTANCE_CONST_3 1.0
#define DISTANCE_CONST_4 1.0
#define DISTANCE_THRESHOLD 3.0

#define PERCENT_MUTATION_CONNECTION 0.25
#define PERCENTAGE_OFFSPRING_WITHOUT_CROSSOVER 25
#define PROBABILITY_INTERSPACIES_MATING 0.01
#define SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE 0.07
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















































void Population::init_population(char path[]){
	
	Genetic_Encoding _organism;
	_organism.load(path);
	_organism.niche=0;

	for (int i = 0; i < (int)_organism.Lnode_genes.size(); ++i)
	{
		if (_organism.Lnode_genes[i].type == INPUT)
		{
			_organism.Lnode_genes[i].row = 0;
		}
		else if(_organism.Lnode_genes[i].type == OUTPUT)
		{
			_organism.Lnode_genes[i].row = 1;
		}
		else{
			cerr << "FATAL ERROR:: Your initial genome(Genetic_Encoding) can not have hiden nodes\n";
			exit(1); 
		}	
	}
	
	row_orderer_list.push_back(0);
	row_orderer_list.push_back(1);
	last_row = 2;

	fitness_champion = 0;
	champion = _organism;

	last_innovation=0;
	for (int i = 0; i < (int)_organism.Lconnection_genes.size(); ++i)
		obtain_innovation(_organism.Lconnection_genes[i].in, _organism.Lconnection_genes[i].out); 
	last_node = (int)_organism.Lnode_genes.size()-1;
	
	

	for (int i = 0; i < POPULATION_MAX; ++i){
		organisms.push_back(  mutation_node(put_randoms_weight(_organism)) ); 
	}
	lenght = POPULATION_MAX;

	prev_organisms.push_back(_organism);
	Niche niche_temp;
	niche_temp.organism_position.push_back(0);
	niche_temp.exist=true;
	niche_temp.niche_champion_position=0;
	current_niches.push_back(niche_temp);
	spatiation();
}









Genetic_Encoding Population::mutation_node(Genetic_Encoding organism){


	int number_of_connections = organism.Lconnection_genes.size();
	int connection_to_mutate;
	int innov1;
	int innov2;
	int node;
	// add node
	int count(0);
	int row;

	do{
		while(true){
			connection_to_mutate = round(rand()%number_of_connections);
			if(organism.Lconnection_genes[connection_to_mutate].exist) break;
		}
		
		
		node = obtain_historical_node(organism.Lconnection_genes[connection_to_mutate].in, organism.Lconnection_genes[connection_to_mutate].out);
		if(node < (int)organism.Lnode_genes.size()){
			if(!organism.Lnode_genes[node].exist){
				break;
			}
		}
		else{
			break;
		}
		if(count++ > 50){cerr << "In function Mutation_node:: in 50 attempts not found an mutation option";break;}
	}while(true);

	row = obtain_row(node, organism.Lnode_genes[organism.Lconnection_genes[connection_to_mutate].in].row, organism.Lnode_genes[ organism.Lconnection_genes[connection_to_mutate].out ].row );
	organism.add_node(node, row ,HIDDEN);
	

	// add connections
	organism.Lconnection_genes[connection_to_mutate].enable=0; // disabling old connection.
	
	innov1 = obtain_innovation(organism.Lconnection_genes[connection_to_mutate].in, node);
	innov2 = obtain_innovation(node, organism.Lconnection_genes[connection_to_mutate].out);
	
	organism.add_connection(innov1, organism.Lconnection_genes[connection_to_mutate].in, node, 1.0);
	organism.add_connection(innov2, node, organism.Lconnection_genes[connection_to_mutate].out, organism.Lconnection_genes[connection_to_mutate].weight);
	
	
	return organism;
}













// Hay que mejorar esto
int Population::obtain_row(int node, int row_node_initial_in, int row_node_initial_out){
	
	
	int row_position_in(-1);
	int row_position_out(-1);
	bool flag_in(false), flag_out(false);
	for (int i = 0; i < (int)row_orderer_list.size(); ++i)
	{
		if(!flag_in) 	if(row_node_initial_in == row_orderer_list[i]) {  row_position_in =  i; flag_in=true; }
		if(!flag_out)	if(row_node_initial_out == row_orderer_list[i]){  row_position_out = i; flag_out=true;}
		if(flag_in && flag_out) break;
	}
	
	if(row_position_in == -1 || row_position_out == -1 ){
		cerr << row_node_initial_in << "\t" << row_position_in << "\t" << row_node_initial_out << "\t" << row_position_out << "\t" << row_orderer_list.size() << "\n" << "Error:: Function obtain_row :: Row_node_in or row_node_out does not exist .\n"; exit(1);
	}

	if( row_position_in > row_position_out){
		cerr << row_node_initial_in << "\t" << row_position_in << "\t" << row_node_initial_out << "\t" << row_position_out << "\t" << row_orderer_list.size() << "\n"; 
		cerr << "Error:: Function obtain_row :: ------------------------.\n"; exit(1);
	}

	while((int)historical_row.size()-1 < node)
	{
		historical_row.push_back(-1);
	}

	if(historical_row[node] >= 0) {
		return historical_row[node];
	}
	
	
	if(row_position_in == row_position_out ){

		return row_node_initial_in;
	}
	else{
		if(row_position_out - row_position_in == 1 ){
			historical_row[node] = last_row;
			row_orderer_list.insert(row_orderer_list.begin() + row_position_in + 1, last_row);
			last_row++;
			return historical_row[node];
		}
		else{
			historical_row[node] = row_orderer_list[row_position_in + 1];

			return historical_row[node];
		}
	}	
}







































// RECORDAR: son inicial in y inicial out, esa es la idea.
int Population::obtain_historical_node(int in, int out){
	while((int)historical_nodes.size()-1 < in)
	{
		vector <int> temp;
		historical_nodes.push_back(temp);
	}
	while((int)historical_nodes[in].size()-1 < out)
	{
		historical_nodes[in].push_back(-1);
	}
	if(historical_nodes[in][out] < 0){
		last_node = last_node + 1;
		historical_nodes[in][out] = last_node;
	}
	return historical_nodes[in][out];
}







































int Population::obtain_innovation(int in, int out){
	while((int)historical_innovation.size()-1 < in)
	{
		vector <int> temp;
		historical_innovation.push_back(temp);
	}
	while((int)historical_innovation[in].size()-1 < out)
	{
		historical_innovation[in].push_back(-1);
	}
	if(historical_innovation[in][out] < 0){
		historical_innovation[in][out] = last_innovation;
		last_innovation++;
	}
	return historical_innovation[in][out];
}




















































































Genetic_Encoding Population::mutation_connection(Genetic_Encoding organism){
	int count(0), counter(0);
	int node_in;
	int node_out;
	int innovation;
	int number_of_nodes = (int)organism.Lnode_genes.size();
	
	while(true){
		counter=0;
		while(true){
			node_in = round(rand()%number_of_nodes);
			node_out = round(rand()%number_of_nodes);
			//asdlfasjdfpoasijfpdoiasj

			int row_position_in(-1);
			int row_position_out(-1);
			bool flag_in(false), flag_out(false);
			
			for (int i = 0; i < (int)row_orderer_list.size(); ++i)
			{
				if(organism.Lnode_genes[node_in].row == row_orderer_list[i]){   row_position_in = i; flag_in=true;}
				if(organism.Lnode_genes[node_out].row == row_orderer_list[i]){ 	row_position_out = i; flag_out=true;}
				if(flag_in && flag_out)break;
			}
	
			if( (organism.Lnode_genes[node_out].exist  && organism.Lnode_genes[node_in].exist) && ( row_position_in <  row_position_out) ) break;
			if (counter > 50)
			{
				
				//cerr << "In function mutation_connection:: counter is greater than 50 \n";
				return organism;
			}
			counter++;
		}

		
		innovation = obtain_innovation(node_in, node_out);
		if((int)organism.Lconnection_genes.size() - 1 < innovation){
			organism.add_connection(innovation,node_in, node_out, 2*(rand()%10000)/10000.0 - 1.0);
			break;
		}
		else if(!organism.Lconnection_genes[innovation].exist){
			organism.add_connection(innovation,node_in, node_out, 2*(rand()%10000)/10000.0 - 1.0);
			break;
		}
		
		if(count>50){
			//cerr << "In function mutation_connection:: in 50 attempts not found an mutation option \n";
			break;
		}

		count++;

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
	int node_size_1 = orgm1.Lnode_genes.size();
	int node_size_2 = orgm2.Lnode_genes.size();

	int conn_limit_sup, conn_limit_inf, node_limit_sup, node_limit_inf;
	bool connection_1_is_larger, node_1_is_larger;

	if(conn_size_1>conn_size_2) {		conn_limit_sup=conn_size_1;		conn_limit_inf=conn_size_2;		connection_1_is_larger=true;}
	else{		conn_limit_sup=conn_size_2;		conn_limit_inf=conn_size_1;		connection_1_is_larger=false;	}
	if(node_size_1>node_size_2) {		node_limit_sup=node_size_1;		node_limit_inf=node_size_2;		node_1_is_larger=true;	}
	else{		node_limit_sup=node_size_2;		node_limit_inf=node_size_1;		node_1_is_larger=false;	}


	int i=0;
	while( i < conn_limit_sup)
	{
		if (i >= conn_limit_inf)
		{
			if(connection_1_is_larger){
				if(orgm1.Lconnection_genes[i].exist==1){
					orgm_resutl.add_connection(orgm1.Lconnection_genes[i]);
				}
			}
			else{
				if(orgm2.Lconnection_genes[i].exist==1){
					orgm_resutl.add_connection(orgm2.Lconnection_genes[i]);
				}
			}
		}

		else{
			if( orgm1.Lconnection_genes[i].exist && orgm2.Lconnection_genes[i].exist )
				(rand()%10 >= 5) ? orgm_resutl.add_connection(orgm1.Lconnection_genes[i]) : orgm_resutl.add_connection(orgm2.Lconnection_genes[i]);
			else if ( orgm1.Lconnection_genes[i].exist || orgm2.Lconnection_genes[i].exist)
				(orgm1.Lconnection_genes[i].exist ) ? orgm_resutl.add_connection(orgm1.Lconnection_genes[i]) : orgm_resutl.add_connection(orgm2.Lconnection_genes[i]);
		}
		
		i++;
	}


	i=0;
	while(i < node_limit_sup){
		if (i >= node_limit_inf)
		{
			if(node_1_is_larger){
				if(orgm1.Lnode_genes[i].exist==1){
					orgm_resutl.add_node(orgm1.Lnode_genes[i]);
				}
			}
			else{
				if(orgm2.Lnode_genes[i].exist==1 ){
					orgm_resutl.add_node(orgm2.Lnode_genes[i]);
				}
			}
		}

		else{
			if( orgm1.Lnode_genes[i].exist && orgm2.Lnode_genes[i].exist )
				(rand()%10 >= 5) ? orgm_resutl.add_node(orgm1.Lnode_genes[i]) : orgm_resutl.add_node(orgm2.Lnode_genes[i]);
			else if ( orgm1.Lnode_genes[i].exist || orgm2.Lnode_genes[i].exist )
				(orgm1.Lnode_genes[i].exist) ? orgm_resutl.add_node(orgm1.Lnode_genes[i]) : orgm_resutl.add_node(orgm2.Lnode_genes[i]);
		}
		i++;
	}

	return orgm_resutl;
}
























































void Population::spatiation(){
	vector<Niche>().swap(prev_niches);
	prev_niches = current_niches;
	vector<Niche>().swap(current_niches);

	Niche aux_niche;
	aux_niche.exist=false;
	for(int i=0; i < (int)prev_niches.size(); i++){
		current_niches.push_back(aux_niche);
	}
	//cout << "Prev_niches.size(): " << (int)prev_niches.size() << endl;


	bool have_niche;
	int amount_of_new_niches(0);
	for(int j=0; j < (int)organisms.size(); j++){
		have_niche = false;

		for (int i = 0; i < (int)prev_niches.size(); ++i)
		{
			if(prev_niches[i].exist){
				if(compatibility(organisms[j], prev_organisms[prev_niches[i].niche_champion_position]) < DISTANCE_THRESHOLD ){
					have_niche = true;
					current_niches[i].exist=true;
					current_niches[i].organism_position.push_back(j);
					current_niches[i].niche_champion_position=j; // this is temporal until in function epoch the real champion is decided respect its fitness
					break;
				}
			}
		}

		if (!have_niche)
		{
			for (int i = 0; i < amount_of_new_niches; ++i)
			{
				if( compatibility( organisms[j], organisms[current_niches[i + (int)prev_niches.size()].niche_champion_position] ) < DISTANCE_THRESHOLD ){
					have_niche = true;
					current_niches[i + (int)prev_niches.size()].exist=true;
					current_niches[i + (int)prev_niches.size()].organism_position.push_back(j);
					current_niches[i + (int)prev_niches.size()].niche_champion_position=j; // this is temporal until in function epoch the real champion is decided respect its fitness
					break;
				}
			}
		}

		if(!have_niche){
			Niche aux2_niche;
			aux2_niche.exist=true;
			aux2_niche.niche_champion_position=j;
			aux2_niche.organism_position.push_back(j);
			current_niches.push_back(aux2_niche);
			amount_of_new_niches++;
		}

	
	}
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

	double fitness_temp;
	double fitness_max(0.0);
	int mutation_amount; 
	int random_organism;
	int random_father;
	int random_mother; // for mating
	int random_niche_father;
	int random_niche_mother;
	Genetic_Encoding organism_temp;
	Genetic_Encoding organism_father;
	Genetic_Encoding organism_mother;// for mating
	double total_shared_fitness_population(0.0);


	vector <Niche> real_niches;
	for (int i = 0; i < (int)current_niches.size(); ++i)
	{
		if(current_niches[i].exist){
			real_niches.push_back(current_niches[i]);
		}
	}


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
			total_shared_fitness_population+= current_niches[i].total_fitness/current_niches[i].organism_position.size();
		}
	}
	vector < Genetic_Encoding >().swap(prev_organisms);
	prev_organisms = organisms;
	vector < Genetic_Encoding >().swap(organisms);



	// CAMBIARLO Y USAR REAL_NICHES.
	for (int i = 0; i < (int)current_niches.size(); ++i)
	{
		if(current_niches[i].exist){
			current_niches[i].amount_of_offspring=round( POPULATION_MAX*(current_niches[i].total_fitness/current_niches[i].organism_position.size())/total_shared_fitness_population   + 1.0);
			for (int j = 0; j < current_niches[i].amount_of_offspring; ++j)
			{
				if(j==0){ // all niche champions pass throgh generations.
					organisms.push_back(prev_organisms[current_niches[i].niche_champion_position]);
				}
				if(rand()%100 < PERCENTAGE_OFFSPRING_WITHOUT_CROSSOVER){
					random_organism = rand()%current_niches[i].organism_position.size();
					organism_temp = prev_organisms[random_organism];
					mutation_amount = rand()%(int)round(prev_organisms[random_organism].Lconnection_genes.size()*PERCENT_MUTATION_CONNECTION) + 1; 
					for(int k = 0; k < mutation_amount; k++)
						organism_temp = mutation_change_weight(organism_temp);
				}
				else{
					if( (rand()%1000)/1000.0 < PROBABILITY_INTERSPACIES_MATING){
						while(true){							
							random_niche_father = rand()%real_niches.size();
							random_niche_mother = rand()%real_niches.size();
							if(random_niche_mother != random_niche_father)break;
							if(real_niches.size() == 1 ){
								cerr << "Warning:: In function Epoch:: Exist only one niche\n";
								break;
							}
						}


						
						
						random_father = real_niches[random_niche_father].organism_position[rand()%real_niches[random_niche_father].organism_position.size()];
						organism_father = prev_organisms[random_father];
						

						random_mother = real_niches[random_niche_mother].organism_position[rand()%real_niches[random_niche_mother].organism_position.size()];
						organism_mother = prev_organisms[random_mother];
						
						organism_temp = crossover(organism_father, organism_mother);
						
					}
					else{
						while(true){
							random_niche_father = rand()%current_niches.size();
							if(current_niches[random_niche_father].exist && (int)current_niches[random_niche_father].organism_position.size() > 1)break;
						}
						random_father = current_niches[random_niche_father].organism_position[rand()%current_niches[random_niche_father].organism_position.size()];
						organism_father = prev_organisms[random_father];
						
						while(true){
							random_mother = rand()%current_niches[random_niche_father].organism_position.size();
							if(random_mother != random_father)break;
						}
						
						organism_mother = prev_organisms[random_mother];
						random_mother = current_niches[random_mother].organism_position[random_mother];
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
	spatiation();
}


#endif