#ifndef NEAT_CPP
#define NEAT_CPP

#include "NEAT.hpp"




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
	organism.Lconnection_genes[connection_to_mutate].weight=delta*0.2 + 0.8*organism.Lconnection_genes[connection_to_mutate].weight;
	return organism;
}















































void Population::init_population(char path[]){
	
	Genetic_Encoding _organism;
	_organism.load(path);
	_organism.niche=0;
	_organism.SIGMOID_CONSTANT = SIGMOID_CONSTANT;

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

	_organism.row_orderer_list = row_orderer_list;

	fitness_champion = 0;
	champion = _organism;

	last_innovation=0;
	for (int i = 0; i < (int)_organism.Lconnection_genes.size(); ++i)
		obtain_innovation(_organism.Lconnection_genes[i].in, _organism.Lconnection_genes[i].out); 
	last_node = (int)_organism.Lnode_genes.size()-1;
	
	

	for (int i = 0; i < POPULATION_MAX; ++i){
		organisms.push_back( put_randoms_weight(_organism) ); 
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
		if(count++ > 50){
			cerr << "In function Mutation_node:: in 50 attempts not found an mutation option";
			return organism;
		}
	}while(true);

	row = obtain_row(node, organism.Lnode_genes[organism.Lconnection_genes[connection_to_mutate].in].row, organism.Lnode_genes[ organism.Lconnection_genes[connection_to_mutate].out ].row );
	organism.add_node(node, row ,HIDDEN);
	organism.row_orderer_list = row_orderer_list;

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
	for (int i = 0; i < (int)pop.organisms.size(); ++i){
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
			if( orgm1.Lconnection_genes[i].exist && orgm2.Lconnection_genes[i].exist ){
				if(orgm1.Lconnection_genes[i].enable && orgm2.Lconnection_genes[i].enable){
					(rand()%10 >= 5) ? orgm_resutl.add_connection(orgm1.Lconnection_genes[i]) : orgm_resutl.add_connection(orgm2.Lconnection_genes[i]);
				}
				else{
					//(!orgm1.Lconnection_genes[i].enable)? orgm_resutl.add_connection(orgm1.Lconnection_genes[i]) : orgm_resutl.add_connection(orgm2.Lconnection_genes[i]); 
					if(!orgm1.Lconnection_genes[i].enable){
						if( ((double)rand())/RAND_MAX < PROB_ENABLE_AN_DISABLE_CONNECTION ){
							orgm1.Lconnection_genes[i].enable = true;
							orgm_resutl.add_connection(orgm1.Lconnection_genes[i]);
						}
						else{
							orgm_resutl.add_connection(orgm1.Lconnection_genes[i]);
						}
					}
					else{
						if( ((double)rand())/RAND_MAX < PROB_ENABLE_AN_DISABLE_CONNECTION ){
							orgm2.Lconnection_genes[i].enable = true;
							orgm_resutl.add_connection(orgm2.Lconnection_genes[i]);
						}
						else{
							orgm_resutl.add_connection(orgm2.Lconnection_genes[i]);
						}
					}
				}
			}
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
	orgm_resutl.row_orderer_list = row_orderer_list;
	orgm_resutl.SIGMOID_CONSTANT = orgm1.SIGMOID_CONSTANT;
	return orgm_resutl;
}

























































void Population::spatiation(){
	vector<Niche>().swap(prev_niches);
	prev_niches = current_niches;
	vector<Niche>().swap(current_niches);

	
	//cout << "Prev_niches.size(): " << (int)prev_niches.size() << endl;

	vector <Niche> real_niches;
	vector <Niche> current_niches_temp;
	for (int i = 0; i < (int)prev_niches.size(); ++i)
	{
		if (prev_niches[i].exist)
		{
			real_niches.push_back(prev_niches[i]);
		}
	}

	Niche aux_niche;
	aux_niche.exist=false;
	for(int i=0; i < (int)real_niches.size(); i++){
		current_niches_temp.push_back(aux_niche);
	}


	bool have_niche;
	int amount_of_new_niches(0);
	for(int j=0; j < (int)organisms.size(); j++){
		have_niche = false;

		for (int i = 0; i < (int)real_niches.size(); ++i)
		{
			
			if(compatibility(organisms[j], prev_organisms[real_niches[i].niche_champion_position]) < DISTANCE_THRESHOLD ){
				have_niche = true;
				current_niches_temp[i].exist=true;
				current_niches_temp[i].organism_position.push_back(j);
				current_niches_temp[i].niche_champion_position=j; // this is temporal until in function epoch the real champion is decided respect its fitness
				break;
			}
			
		}

		if (!have_niche)
		{
			for (int i = 0; i < amount_of_new_niches; ++i)
			{
				if( compatibility( organisms[j], organisms[current_niches_temp[i + (int)real_niches.size()].niche_champion_position] ) < DISTANCE_THRESHOLD ){
					have_niche = true;
					current_niches_temp[i + (int)real_niches.size()].exist=true;
					current_niches_temp[i + (int)real_niches.size()].organism_position.push_back(j);
					current_niches_temp[i + (int)real_niches.size()].niche_champion_position=j; // this is temporal until in function epoch the real champion is decided respect its fitness
					break;
				}
			}
		}

		if(!have_niche){
			Niche aux2_niche;
			aux2_niche.exist=true;
			aux2_niche.niche_champion_position=j;
			aux2_niche.organism_position.push_back(j);
			current_niches_temp.push_back(aux2_niche);
			amount_of_new_niches++;
		}
	}


	for (int i = 0; i < (int)current_niches_temp.size(); ++i)
		if( current_niches_temp[i].exist ) current_niches.push_back(current_niches_temp[i]);

	for (int i = 0; i < (int)current_niches.size(); ++i)
	{
			for (int j = 0; j < (int)current_niches[i].organism_position.size(); ++j)
			{
				organisms[current_niches[i].organism_position[j]].niche = i;
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





















































/*
//VOLVER A HACER
void Population::epoch(){

	double temp_max_current_fitness(0.0);
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



	cerr << "P1" << endl;

	for (int i = 0; i < (int)organisms.size(); ++i)
		organisms[i].fitness = fitness(organisms[i]);
	
	cerr << "P2" << endl;
	
	for (int i = 0; i < (int)current_niches.size(); ++i)
	{
		current_niches[i].total_fitness=0;
		for (int j = 0; j < (int)current_niches[i].organism_position.size(); ++j)
		{
			current_niches[i].total_fitness += organisms[current_niches[i].organism_position[j]].fitness/current_niches[i].organism_position.size();

			if(j == 0 || temp_max_current_fitness < organisms[current_niches[i].organism_position[j]].fitness){
				temp_max_current_fitness = organisms[current_niches[i].organism_position[j]].fitness;
				current_niches[i].niche_champion_position=current_niches[i].organism_position[j];
				if(temp_max_current_fitness > fitness_champion){
					fitness_champion = temp_max_current_fitness;
					champion = organisms[current_niches[i].organism_position[j]];
				}
			}
		}
	}

	cerr << "P3" << endl;

	for (int i = 0; i < (int)current_niches.size(); ++i)
	{
		total_shared_fitness_population += current_niches[i].total_fitness;
	}

	cerr << "P4" << endl;
	vector < Genetic_Encoding >().swap(prev_organisms);
	prev_organisms = organisms;
	vector < Genetic_Encoding >().swap(organisms);

	int sum(0);
	//int corrector = ((int)prev_organisms.size() > POPULATION_MAX ) ? (int)prev_organisms.size(): POPULATION_MAX;
	for (int i = 0; i < (int)current_niches.size(); ++i)
	{
		current_niches[i].amount_of_offspring = round(POPULATION_MAX*(current_niches[i].total_fitness/total_shared_fitness_population));
		sum += current_niches[i].amount_of_offspring;
		cerr << current_niches.size() << "\t" << current_niches[i].amount_of_offspring << "\t" << current_niches[i].organism_position.size() << "\t" << current_niches[i].total_fitness << "\t" << total_shared_fitness_population << endl; 
	}
	cerr << "P5" << endl;
	cerr << sum << endl;

	for (int i = 0; i < (int)current_niches.size(); ++i)
	{
		for (int j = 0; j < (int)current_niches[i].organism_position.size(); ++j)
		{
			if(prev_organisms[current_niches[i].organism_position[j]].fitness < (current_niches[i].total_fitness/current_niches[i].organism_position.size()) ){
				prev_organisms[current_niches[i].organism_position[j]] = prev_organisms[current_niches[i].niche_champion_position];
			}
		}
	}


	for (int i = 0; i < (int)current_niches.size(); ++i)
	{
		for (int j = 0; j < current_niches[i].amount_of_offspring; ++j)
		{
			if(j==0){ // all niche champions pass throgh generations.
				organisms.push_back(prev_organisms[current_niches[i].niche_champion_position]);
			}
			if(rand()%100 < PERCENTAGE_OFFSPRING_WITHOUT_CROSSOVER){
				random_organism = rand()%current_niches[i].organism_position.size();
				organism_temp = prev_organisms[random_organism];

				while( (double)rand()/RAND_MAX < PERCENT_MUTATION_CONNECTION )
					organism_temp = mutation_change_weight(organism_temp);
			}
			else{
				if( (rand()%1000)/1000.0 < PROBABILITY_INTERSPACIES_MATING){
					random_niche_father = i; // Is not really random
					while(true){							
						random_niche_mother = rand()%current_niches.size();
						if(random_niche_mother != random_niche_father)break;
						if(current_niches.size() == 1 ){
							cerr << "Warning:: In function Epoch:: Exist only one niche\n";
							break;
						}
					}


					
					random_father = current_niches[random_niche_father].organism_position[rand()%current_niches[random_niche_father].organism_position.size()];
					organism_father = prev_organisms[random_father];
					

					random_mother = current_niches[random_niche_mother].organism_position[rand()%current_niches[random_niche_mother].organism_position.size()];
					organism_mother = prev_organisms[random_mother];
					
					organism_temp = crossover(organism_father, organism_mother);
				}	
				else{
					random_niche_father = i;
						
					random_father = current_niches[random_niche_father].organism_position[rand()%current_niches[random_niche_father].organism_position.size()];
					organism_father = prev_organisms[random_father];
					
					while(true){
						random_mother = rand()%current_niches[random_niche_father].organism_position.size();
						if(random_mother != random_father)break;
						if(current_niches[random_niche_father].organism_position.size()==1)break;
					}
					organism_mother = prev_organisms[random_mother];
					random_mother = current_niches[random_mother].organism_position[random_mother];
					organism_temp = crossover(organism_father, organism_mother);
				}

			}
			




			if( (int)organism_temp.Lnode_genes.size() <  LARGE_POPULATION_DISCRIMINATOR ){ // enter if is a small organism
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
	spatiation();
}

*/
/* 

void Population::epoch(){

	//double temp_max_current_fitness(0.0);
	int mutation_amount; 
	int random_organism;
	int random_father;
	int random_mother; // for mating
	int random_niche_father;
	int random_niche_mother;
	double random;
	Genetic_Encoding organism_temp;
	Genetic_Encoding organism_father;
	Genetic_Encoding organism_mother;// for mating
	double total_shared_fitness_population(0.0);


	// Se calculan todos los fitness de todos los organismos.
	for (int i = 0; i < (int)organisms.size(); ++i){
		organisms[i].fitness = fitness(organisms[i]);
	}


	for (int i = 0; i < (int)current_niches.size(); ++i)
	{

		current_niches[i].total_shared_fitness = 0;
		for (int j = 0; j < (int)current_niches[i].organism_position.size(); ++j)
		{
			//current_niches[i].total_fitness += organisms[current_niches[i].organism_position[j]].fitness/current_niches[i].organism_position.size();

			if(j == 0 || organisms[current_niches[i].organism_position[j]].fitness < organisms[current_niches[i].niche_champion_position].fitness){
				current_niches[i].niche_champion_position=current_niches[i].organism_position[j];
				if( organisms[current_niches[i].organism_position[j]].fitness > fitness_champion ){
					//cerr <<  "comparacion" <<  fitness_champion << "\t" << organisms[current_niches[i].organism_position[j]].fitness << endl;
					fitness_champion	= organisms[ current_niches[i].organism_position[j] ].fitness;
					champion			= organisms[ current_niches[i].organism_position[j] ];

				}

			}

			organisms[current_niches[i].organism_position[j]].shared_fitness = organisms[current_niches[i].organism_position[j]].fitness/(double)current_niches[i].organism_position.size();
			current_niches[i].total_shared_fitness += organisms[current_niches[i].organism_position[j]].shared_fitness;
		}
	}



	for (int i = 0; i < (int)current_niches.size(); ++i)
	{
		total_shared_fitness_population += current_niches[i].total_shared_fitness;
	}



	// *Se calculan la cantidad de hijos por cada nicho.
	// *Se trunca la cantidad de hijos
	// *Se reparten los restantes uno a uno, 
	//	Para repartir primero se calcula la probabilidad que corresponde a cada nicho
	//	Luego se obtiene un número random, dependiendo del random se elije el nicho
	//  Obviamente nichos con mayor fitness tienen más probabilidades de tener más hijos.
	vector <double> random_niche;
	double probability_acum;
	int sum(0);
	for (int i = 0; i < (int)current_niches.size(); ++i)
	{
		random_niche.push_back((double)(current_niches[i].total_shared_fitness)/total_shared_fitness_population);
	}
	for (int i = 0; i < (int)current_niches.size(); ++i)
	{
		current_niches[i].amount_of_offspring = trunc(POPULATION_MAX*(current_niches[i].total_shared_fitness/total_shared_fitness_population));
		sum += current_niches[i].amount_of_offspring;
	}
	while(sum < POPULATION_MAX){
		random = ((double)rand())/RAND_MAX;
		probability_acum = 0.0;
		for(int i=0; i < (int)current_niches.size(); i++){
			probability_acum += random_niche[i];
			if(random < probability_acum){
				current_niches[i].amount_of_offspring++;
				break;
			}
		}
		sum++;
	}







	// Según mi opinión debería cambiarse hacia el final antes de spatiation()
	vector < Genetic_Encoding >().swap(prev_organisms); // Se limpia prev_organisms
	prev_organisms = organisms;
	vector < Genetic_Encoding >().swap(organisms); // Se limpia organisms

	// Por cada nicho:
	// Elegir un organismo
	// 		La probabilidad de elegir un organismo debe ser relativa a su fitness
	// ó se crusa ó muta segun probabilidades
	for (int i = 0; i < (int)current_niches.size(); ++i)
	{
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
					random_niche_father = i; // Is not really random
					while(true){							
						random_niche_mother = rand()%current_niches.size();
						if(random_niche_mother != random_niche_father)break;
						if(current_niches.size() == 1 ){
							cerr << "Warning:: In function Epoch:: Exist only one niche\n";
							break;
						}
					}


					
					
					random_father = current_niches[random_niche_father].organism_position[rand()%current_niches[random_niche_father].organism_position.size()];
					organism_father = prev_organisms[random_father];
					

					random_mother = current_niches[random_niche_mother].organism_position[rand()%current_niches[random_niche_mother].organism_position.size()];
					organism_mother = prev_organisms[random_mother];
					
					organism_temp = crossover(organism_father, organism_mother);
					
				}
				else{
					
					random_niche_father = i;
						
					random_father = current_niches[random_niche_father].organism_position[rand()%current_niches[random_niche_father].organism_position.size()];
					organism_father = prev_organisms[random_father];
					
					while(true){
						random_mother = rand()%current_niches[random_niche_father].organism_position.size();
						if(random_mother != random_father)break;
						if(current_niches[random_niche_father].organism_position.size()==1)break;
					}
					
					organism_mother = prev_organisms[random_mother];
					random_mother = current_niches[random_mother].organism_position[random_mother];
					organism_temp = crossover(organism_father, organism_mother);
				}

			}
			





			if( (int)organism_temp.Lnode_genes.size() <  LARGE_POPULATION_DISCRIMINATOR ){ // enter if is a small organism
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
	
	spatiation();
}
*/



void Population::epoch(){

	double random;
	double total_shared_fitness_population(0.0);
	vector <double> random_organism;
	double probability_acum;
	int organism_acum(0);

	// Se calculan todos los fitness de todos los organismos.
	for (int i = 0; i < (int)organisms.size(); ++i){
		organisms[i].fitness = fitness(organisms[i]);
	}

	for (int i = 0; i < (int)current_niches.size(); ++i)
	{

		current_niches[i].total_shared_fitness = 0;
		for (int j = 0; j < (int)current_niches[i].organism_position.size(); ++j)
		{
			//current_niches[i].total_fitness += organisms[current_niches[i].organism_position[j]].fitness/current_niches[i].organism_position.size();

			if(j == 0 || organisms[current_niches[i].organism_position[j]].fitness > organisms[current_niches[i].niche_champion_position].fitness){
				current_niches[i].niche_champion_position = current_niches[i].organism_position[j];
				if( organisms[current_niches[i].organism_position[j]].fitness > fitness_champion ){
					fitness_champion	= organisms[ current_niches[i].organism_position[j] ].fitness;
					champion			= organisms[ current_niches[i].organism_position[j] ];
				}

			}

			organisms[current_niches[i].organism_position[j]].shared_fitness = organisms[current_niches[i].organism_position[j]].fitness/(double)current_niches[i].organism_position.size();
			total_shared_fitness_population += organisms[current_niches[i].organism_position[j]].shared_fitness;
			//organisms[current_niches[i].organism_position[j]].niche = i; //idealmente que esto se haga en spatiation()
		}
	}

	// *Se calculan la cantidad de hijos por cada nicho.
	// *Se trunca la cantidad de hijos
	// *Se reparten los restantes uno a uno, 
	//	Para repartir primero se calcula la probabilidad que corresponde a cada nicho
	//	Luego se obtiene un número random, dependiendo del random se elije el nicho
	//  Obviamente nichos con mayor fitness tienen más probabilidades de tener más hijos.
	
	for (int i = 0; i < (int)organisms.size(); ++i)
		random_organism.push_back((double)(organisms[i].shared_fitness)/total_shared_fitness_population);
	
	vector < Genetic_Encoding >().swap(prev_organisms); // Se limpia prev_organisms
	prev_organisms = organisms;
	vector < Genetic_Encoding >().swap(organisms); // Se limpia organisms

	while(organism_acum < POPULATION_MAX){
		random = ((double)rand())/RAND_MAX;
		probability_acum = 0.0;
		for(int i=0; i < (int)prev_organisms.size(); i++){
			probability_acum += random_organism[i];
			if(random <= probability_acum){
				organisms.push_back( epoch_reproduce(prev_organisms[i] , i) );
				break;
			}
		}
		organism_acum++;
	}
	spatiation();

}



Genetic_Encoding Population::epoch_reproduce(Genetic_Encoding organism, int poblation_place){
	int random_mother; // for mating
	int random_niche_mother;
	Genetic_Encoding organism_father;
	Genetic_Encoding organism_mother;// for mating

	if(100 *((double)rand())/RAND_MAX < PERCENTAGE_OFFSPRING_WITHOUT_CROSSOVER){
		while( (double)rand()/RAND_MAX < PERCENT_MUTATION_CONNECTION )
			organism = mutation_change_weight(organism);
	}
	else{
		if( ((double)rand())/RAND_MAX < PROBABILITY_INTERSPACIES_MATING){
			while(true){							
				random_niche_mother = rand()%current_niches.size();
				if(random_niche_mother != organism.niche)break;
				if(current_niches.size() == 1 ){
					cerr << "Warning:: In function Epoch:: Exist only one niche\n";
					for(int k = 0; k < (int)organism.Lconnection_genes.size(); k++)
							organism = mutation_change_weight(organism);
					return(organism);
				}
			}

			random_mother = current_niches[random_niche_mother].organism_position[rand()%current_niches[random_niche_mother].organism_position.size()];
			organism_mother = prev_organisms[random_mother];
			organism = crossover(organism, organism_mother);
		}
		else{
			
			while(true){
				random_mother = rand()%current_niches[organism.niche].organism_position.size();
				if(current_niches[organism.niche].organism_position[random_mother] != poblation_place )break;
				if(current_niches[organism.niche].organism_position.size()==1)break;
			}
			
			organism_mother = prev_organisms[current_niches[organism.niche].organism_position[random_mother]];
			organism = crossover(organism, organism_mother);
		}

	}
	

	if( (int)organism.Lnode_genes.size() <  LARGE_POPULATION_DISCRIMINATOR ){ // enter if is a small organism
		if( ((double)rand()/RAND_MAX) < SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE){
			organism = mutation_node(organism);
		}
		if( ((double)rand()/RAND_MAX) < SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION) {
			organism = mutation_connection(organism);
		}
	}
	else{// enter if is a large niche
		if( ((double)rand()/RAND_MAX) < LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE){
			organism = mutation_node(organism);
		}
		if( ((double)rand()/RAND_MAX) < LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION) {
			organism = mutation_connection(organism);
		}
	}

	return organism;
}

void Population::load_user_definitions(char address[]){
	ifstream file (address);
	file.seekg (0, file.end);
    int length = file.tellg();
    file.seekg (0, file.beg);
	char buffer[length]; // In JSON format
	file.read (buffer,length);
	file.close();
	char * pch;

	char delimiters[] = " \n\":\t{},[";
	pch = strtok (buffer,delimiters);
	pch = strtok (NULL,delimiters);
	POPULATION_MAX =  atoi(pch);
	pch = strtok (NULL,delimiters);
	pch = strtok (NULL,delimiters);
	DISTANCE_CONST_1 = atof(pch);
	pch = strtok (NULL,delimiters);
	pch = strtok (NULL,delimiters);
	DISTANCE_CONST_2 = atof(pch);
	pch = strtok (NULL,delimiters);
	pch = strtok (NULL,delimiters);
	DISTANCE_CONST_3 = atof(pch);
	pch = strtok (NULL,delimiters);
	pch = strtok (NULL,delimiters);
	DISTANCE_CONST_4 = atof(pch);
	pch = strtok (NULL,delimiters);
	pch = strtok (NULL,delimiters);
	DISTANCE_THRESHOLD = atof(pch);
	pch = strtok (NULL,delimiters);
	pch = strtok (NULL,delimiters);
	PERCENT_MUTATION_CONNECTION = atof(pch);
	pch = strtok (NULL,delimiters);
	pch = strtok (NULL,delimiters);
	PERCENTAGE_OFFSPRING_WITHOUT_CROSSOVER = atof(pch);
	pch = strtok (NULL,delimiters);
	pch = strtok (NULL,delimiters);
	PROBABILITY_INTERSPACIES_MATING = atof(pch);
	pch = strtok (NULL,delimiters);
	pch = strtok (NULL,delimiters);
	SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE = atof(pch);
	pch = strtok (NULL,delimiters);
	pch = strtok (NULL,delimiters);
	SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION = atof(pch);
	pch = strtok (NULL,delimiters);
	pch = strtok (NULL,delimiters);
	LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE = atof(pch);
	pch = strtok (NULL,delimiters);
	pch = strtok (NULL,delimiters);
	LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION = atof(pch);
	pch = strtok (NULL,delimiters);
	pch = strtok (NULL,delimiters);
	PROB_ENABLE_AN_DISABLE_CONNECTION = atof(pch);
	/* Lnodes.size() comparation */
	pch = strtok (NULL,delimiters);
	pch = strtok (NULL,delimiters);
	LARGE_POPULATION_DISCRIMINATOR = atof(pch);
	pch = strtok (NULL,delimiters);
	pch = strtok (NULL,delimiters);
	GENERATIONS = atoi(pch);
	pch = strtok (NULL,delimiters);
	pch = strtok (NULL,delimiters);
	SIGMOID_CONSTANT = atof(pch);


	//cerr << "POPULATION_MAX" << POPULATION_MAX << endl;
	//cerr << "PERCENT_MUTATION_CONNECTION: " << PERCENT_MUTATION_CONNECTION << endl;
	//cerr << "PROBABILITY_INTERSPACIES_MATING: " << PROBABILITY_INTERSPACIES_MATING << endl;
	//cerr << "SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE: " <<SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE << endl;
	//cerr << "SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION: " << SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION<< endl;
	//cerr << "LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE: " <<LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE << endl;
	//cerr << "LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION: " <<LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION << endl;
	//cerr << "PROB_ENABLE_AN_DISABLE_CONNECTION: " << PROB_ENABLE_AN_DISABLE_CONNECTION << endl;
	//cerr << "GENERATIONS" << GENERATIONS << endl;
	//cerr << "SIGMOID_CONSTANT" << SIGMOID_CONSTANT << endl;

}



#endif