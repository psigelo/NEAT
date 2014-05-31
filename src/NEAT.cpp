#ifndef NEAT_CPP
#define NEAT_CPP

#include "NEAT.hpp"




using namespace ANN_USM;


#define PROBABILITY_CHANGE_WEIGHT 0.7


/*
	IDEAS PARA OPTIMIZAR EL C�DIGO:
		1) SACAR TODOS LOS ALGORITMOS PROBABILISTICOS QUE SE PUEDAN POR ALGORITMOS NORMALES.
			-> EN OBTAIN_ROW();
		2) INTEGRAR LISTAS DE NODOS Y DE CONNECCIONES CON LOS NODOS Y CONECCIONES ACTUALES (ESO SE DEBE IMPLEMENTAR DESDE GENETIC ENCODING TAMBI�N
		POR EJEMPLO EN EL MOMENTO EN QUE SE CARGA UN GENOMA).


	OBSERVAR QUE NO DAN LOS MISMOS RESULTADOS CON LAS DOS VERSIONES DE CROSSOVER!!
*/












Genetic_Encoding Population::put_randoms_weight(Genetic_Encoding organism){
	for (int i = 0; i < (int)organism.Lconnection_genes.size(); ++i)
		organism.Lconnection_genes[i].weight=2.0*(rand()%10000)/10000.0-1.0;
	return organism;
}





































/* as� no fue definida en el paper
Genetic_Encoding Population::mutation_change_weight(Genetic_Encoding organism){
	int number_of_connections = organism.Lconnection_genes.size();
	int connection_to_mutate = round(rand()%number_of_connections);
	double delta = 2*(rand()%10000)/10000.0-1;
	organism.Lconnection_genes[connection_to_mutate].weight=delta*0.2 + 0.8*organism.Lconnection_genes[connection_to_mutate].weight;
	return organism;
}
*/


Genetic_Encoding Population::mutation_change_weight(Genetic_Encoding organism){
	double delta;
	for(int i = 0; i < (int)organism.Lconnection_genes.size(); i++ ){
		if( rand()/(double)RAND_MAX <= PROBABILITY_CHANGE_WEIGHT ){
			delta = 2*(rand()%RAND_MAX)/(double)RAND_MAX-1;
			organism.Lconnection_genes.at(i).weight = delta*0.2 + 0.8*organism.Lconnection_genes.at(i).weight;
		}
	}
	return organism;
}












































Population::Population(char path1[],char path2[]){
	load_user_definitions(path1);

	lenght = POPULATION_MAX;
	Genetic_Encoding _organism;
	_organism.load(path2);
	_organism.niche=0;
	fitness_champion = 0;
	last_innovation=0;
	row_orderer_list = _organism.row_orderer_list;
	last_row = _organism.row_orderer_list.size();
	champion = _organism;
	Niche niche_temp;

	// ================FOR REDUNDANCE ============
	for (int i = 0; i < (int)_organism.Lconnection_genes.size(); ++i){
		int in = _organism.Lconnection_genes.at(i).in;
		int out = _organism.Lconnection_genes.at(i).out;
		while((int)historical_innovation.size()-1 < in)
		{
			vector <int> temp;
			historical_innovation.push_back(temp);
		}
		while((int)historical_innovation.at(in).size()-1 < out)
		{
			historical_innovation.at(in).push_back(-1);
		}
		if(historical_innovation.at(in).at(out) < 0){
			historical_innovation.at(in).at(out) = _organism.Lconnection_genes.at(i).innovation;
			if(last_innovation < _organism.Lconnection_genes.at(i).innovation)
				last_innovation = _organism.Lconnection_genes.at(i).innovation;
		}
	}
	last_innovation++;
	//============================================

	last_node = (int)_organism.Lnode_genes.size()-1;

	for (int i = 0; i < POPULATION_MAX; ++i){}
		organisms.push_back( put_randoms_weight(_organism) );



	prev_organisms.push_back(_organism);
	niche_temp.organism_position.push_back(0);
	niche_temp.exist=true;
	niche_temp.niche_champion_position=0;
	current_niches.push_back(niche_temp);
	spatiation();
}









Genetic_Encoding Population::mutation_node(Genetic_Encoding organism){
	int number_of_connections = organism.Lconnection_genes.size();
	int connection_to_mutate;
	int innov1,innov2, node, row;
	Discrete_Probabilities sack_of_nodes_to_take;
	vector <int> inverse_to_connections;

	/*
	// add node
	do{
		do{
			connection_to_mutate = rand()%number_of_connections;
			if(organism.Lconnection_genes.at(connection_to_mutate).exist && organism.Lconnection_genes.at(connection_to_mutate).enable) break;
		}while(true);
		node = obtain_historical_node(organism.Lconnection_genes.at(connection_to_mutate).in, organism.Lconnection_genes.at(connection_to_mutate).out);
		if(node < (int)organism.Lnode_genes.size()){
			if(!organism.Lnode_genes.at(node).exist){
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
	*/


	// Se obtienen todos los candidatos que cumplen con el requisito;
	for(int i=0; i < number_of_connections; i++ ){
		if(  organism.Lconnection_genes.at(i).exist && organism.Lconnection_genes.at(i).enable ){
			node = obtain_historical_node(organism.Lconnection_genes.at(i).in, organism.Lconnection_genes.at(i).out);
			if(node < (int)organism.Lnode_genes.size()){
				if(!organism.Lnode_genes.at(node).exist) {
					sack_of_nodes_to_take.add_element(0.0);
					inverse_to_connections.push_back(i);
				}
			}else{
				sack_of_nodes_to_take.add_element(0.0);
				inverse_to_connections.push_back(i);
			}
		}
	}

	if(inverse_to_connections.size() == 0){
		cerr << "Error::Mutation_node()::Is imposible that no exist node to mutate";
		return organism;
	}

	connection_to_mutate = inverse_to_connections.at(sack_of_nodes_to_take.take_one_randomly());
	node = obtain_historical_node(organism.Lconnection_genes.at(connection_to_mutate).in, organism.Lconnection_genes.at(connection_to_mutate).out);

	row = obtain_row(node, organism.Lnode_genes.at(organism.Lconnection_genes.at(connection_to_mutate).in).row, organism.Lnode_genes.at( organism.Lconnection_genes.at(connection_to_mutate).out ).row );
	organism.add_node(node, row ,HIDDEN);
	organism.row_orderer_list = row_orderer_list;

	// add connections
	organism.Lconnection_genes.at(connection_to_mutate).enable=0; // disabling old connection.

	innov1 = obtain_innovation(organism.Lconnection_genes.at(connection_to_mutate).in, node);
	innov2 = obtain_innovation(node, organism.Lconnection_genes.at(connection_to_mutate).out);

	organism.add_connection(innov1, organism.Lconnection_genes.at(connection_to_mutate).in, node, 1.0);
	organism.add_connection(innov2, node, organism.Lconnection_genes.at(connection_to_mutate).out, organism.Lconnection_genes.at(connection_to_mutate).weight);
	return organism;
}














int Population::obtain_row(int node, int row_node_initial_in, int row_node_initial_out){

	try{
		if(historical_row.at(node) >= 0) {
			return historical_row.at(node);
		}
	}
	catch (const std::out_of_range& oor){}

	int row_position_in(-1);
	int row_position_out(-1);
	bool flag_in(false), flag_out(false);
	for (int i = 0; i < (int)row_orderer_list.size(); ++i)
	{
		if(!flag_in) 	if(row_node_initial_in == row_orderer_list.at(i)) {  row_position_in =  i; flag_in=true; }
		if(!flag_out)	if(row_node_initial_out == row_orderer_list.at(i)){  row_position_out = i; flag_out=true;}
		if(flag_in && flag_out) break;
	}

	if(row_position_in == -1 || row_position_out == -1 ){
		cerr << row_node_initial_in << "\t" << row_position_in << "\t" << row_node_initial_out << "\t" << row_position_out << "\t" << row_orderer_list.size() << "\n" << "Error:: Function obtain_row :: Row_node_in or row_node_out does not exist .\n";
		exit(1);
	}

	if( row_position_in > row_position_out){
	//if( row_position_in == row_position_out)
		cerr << row_node_initial_in << "\t" << row_position_in << "\t" << row_node_initial_out << "\t" << row_position_out << "\t" << row_orderer_list.size() << "\n";
		cerr << "Error:: Function obtain_row :: ------------------------.\n";
		exit(1);
	}


	while((int)historical_row.size()-1 < node)
	{
		historical_row.push_back(-1);
	}
	if(historical_row.at(node) >= 0) {
		return historical_row.at(node);
	}



	if(row_position_in == row_position_out ){
		return row_node_initial_in;
	}


		if(row_position_out - row_position_in == 1 ){
			historical_row.at(node) = last_row;
			row_orderer_list.insert(row_orderer_list.begin() + row_position_in + 1, last_row);
			last_row++;
			return historical_row.at(node);
		}
		else{
			historical_row[node] = row_orderer_list.at(row_position_in + 1);
			return historical_row.at(node);
		}




}







































// RECORDAR: son inicial in y inicial out, esa es la idea.
int Population::obtain_historical_node(int in, int out){
	while((int)historical_nodes.size()-1 < in)
	{
		vector <int> temp;
		historical_nodes.push_back(temp);
	}
	while((int)historical_nodes.at(in).size()-1 < out)
	{
		historical_nodes.at(in).push_back(-1);
	}
	if(historical_nodes.at(in).at(out) < 0){
		last_node = last_node + 1;
		historical_nodes.at(in).at(out) = last_node;
	}
	return historical_nodes.at(in).at(out);
}







































int Population::obtain_innovation(int in, int out){
	while((int)historical_innovation.size()-1 < in)
	{
		vector <int> temp;
		historical_innovation.push_back(temp);
	}
	while((int)historical_innovation.at(in).size()-1 < out)
	{
		historical_innovation.at(in).push_back(-1);
	}
	if(historical_innovation.at(in).at(out) < 0){
		historical_innovation.at(in).at(out) = last_innovation;
		last_innovation++;
	}
	return historical_innovation.at(in).at(out);
}




















































































Genetic_Encoding Population::mutation_connection(Genetic_Encoding organism){
	
	int innovation(-1);
	int number_of_nodes = (int)organism.Lnode_genes.size();
	Discrete_Probabilities sack_of_nodes_to_take;
	vector < vector < int > > inverse_to_nodes;


	/*
	while(true){ // esto parece muy costoso
		counter=0;

		while(true){
			node_in = round(rand()%number_of_nodes);
			node_out = round(rand()%number_of_nodes);
			if(organism.Lnode_genes.at(node_out).exist  && organism.Lnode_genes.at(node_in).exist){
				int row_position_in(-1);
				int row_position_out(-1);
				bool flag_in(false), flag_out(false);

				for (int i = 0; i < (int)row_orderer_list.size(); ++i)
				{
					if(organism.Lnode_genes.at(node_in).row == row_orderer_list.at(i)){   row_position_in = i; flag_in=true;}
					if(organism.Lnode_genes.at(node_out).row == row_orderer_list.at(i)){ 	row_position_out = i; flag_out=true;}
					if(flag_in && flag_out)break;
				}

				if(   row_position_in <  row_position_out ) break;
			}

			if (counter++ > 30)
			{
				cerr << "WARNING::In function mutation_connection:: counter is greater than 30 \n";
				return organism;
			}
		}



		innovation = obtain_innovation(node_in, node_out);
		if((int)organism.Lconnection_genes.size() - 1 < innovation){
			organism.add_connection(innovation,node_in, node_out, 2*(rand()%RAND_MAX)/(double)RAND_MAX - 1.0);
			break;
		}
		else if(!organism.Lconnection_genes[innovation].exist){
			organism.add_connection(innovation,node_in, node_out, 2*(rand()%RAND_MAX)/(double)RAND_MAX - 1.0);
			break;
		}

		if(count++>30){
			cerr << "WARNING::In function mutation_connection:: in 30 attempts not found an mutation option \n";
			break;
		}


	}
	*/


	
	//generando la lista inverstida para hacer más fácil el procedimiento posterior.
	vector <int> inverted_orderer_list; // input: row output:place
	for(int i=0; i < (int)row_orderer_list.size(); i++ ){
		for(int j=0; j < (int)row_orderer_list.size(); j++ ){
			if(i == row_orderer_list.at(j)) inverted_orderer_list.push_back(j);
		}
	}
	for(int in = 0; in < number_of_nodes; in++){
		for(int out=0; out < number_of_nodes; out++){
			if(in != out){
				if(organism.Lnode_genes.at(in).exist  && organism.Lnode_genes.at(out).exist){
					if( inverted_orderer_list.at(organism.Lnode_genes.at(in).row) < inverted_orderer_list.at(organism.Lnode_genes.at(out).row) ){
						innovation = obtain_innovation(in, out);
						if((int)organism.Lconnection_genes.size() - 1 < innovation){
							sack_of_nodes_to_take.add_element(0.0);
							vector <int> temp_vect;
							temp_vect.push_back(in);
							temp_vect.push_back(out);
							inverse_to_nodes.push_back(temp_vect);
						}
						else if(!organism.Lconnection_genes[innovation].exist){
							sack_of_nodes_to_take.add_element(0.0);
							vector <int> temp_vect;
							temp_vect.push_back(in);
							temp_vect.push_back(out);
							inverse_to_nodes.push_back(temp_vect);
						}
					}
				}
			}
		}
	}
	if(inverse_to_nodes.size() == 0){ // No existe ningún par de nodos que no estén conectados anteriormente
		return organism;
	}
	int randoms_nodes = sack_of_nodes_to_take.take_one_randomly();
	innovation = obtain_innovation(inverse_to_nodes.at(randoms_nodes).at(0), inverse_to_nodes.at(randoms_nodes).at(1));
	organism.add_connection(innovation,inverse_to_nodes.at(randoms_nodes).at(0), inverse_to_nodes.at(randoms_nodes).at(1), 2*(rand()%RAND_MAX)/(double)RAND_MAX - 1.0);
	
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
	int limit_sup;

	bool flag_orgm1_is_greater(false);
	if(orgm1.Lconnection_genes.size() > orgm2.Lconnection_genes.size()) flag_orgm1_is_greater=true;
	if(flag_orgm1_is_greater) limit_sup = (int)orgm1.Lconnection_genes.size();
	else limit_sup = (int)orgm2.Lconnection_genes.size();

	if(flag_orgm1_is_greater){
		for(int i=0; i < (int)orgm2.Lconnection_genes.size(); i++){
			if(orgm1.Lconnection_genes.at(i).exist && orgm2.Lconnection_genes.at(i).exist && orgm1.Lconnection_genes.at(i).enable && orgm2.Lconnection_genes.at(i).enable ){
				W += abs(orgm1.Lconnection_genes.at(i).weight - orgm2.Lconnection_genes.at(i).weight);
			}
			else if( (orgm1.Lconnection_genes.at(i).exist && orgm1.Lconnection_genes.at(i).enable) || (orgm2.Lconnection_genes.at(i).exist && orgm2.Lconnection_genes.at(i).enable)) D++;
		}
		for(int i = (int)orgm2.Lconnection_genes.size(); i < (int)orgm1.Lconnection_genes.size(); i++ )
			if(orgm1.Lconnection_genes.at(i).exist && orgm1.Lconnection_genes.at(i).enable)E++;
	}
	else{
		for(int i=0; i < (int)orgm1.Lconnection_genes.size(); i++){
			if( orgm1.Lconnection_genes.at(i).exist && orgm2.Lconnection_genes.at(i).exist && orgm1.Lconnection_genes.at(i).enable && orgm2.Lconnection_genes.at(i).enable){
			W += abs(orgm1.Lconnection_genes.at(i).weight - orgm2.Lconnection_genes.at(i).weight);
			}
			else if((orgm1.Lconnection_genes.at(i).exist && orgm1.Lconnection_genes.at(i).enable) || (orgm2.Lconnection_genes.at(i).exist && orgm2.Lconnection_genes.at(i).enable)) D++;
	}
	for(int i = (int)orgm1.Lconnection_genes.size(); i < (int)orgm2.Lconnection_genes.size(); i++ )
		if(orgm2.Lconnection_genes.at(i).exist && orgm2.Lconnection_genes.at(i).enable )E++;
	}

	distance = ORGANISM_DISTANCE_1*E/(double)limit_sup + ORGANISM_DISTANCE_2*D/(double)limit_sup + ORGANISM_DISTANCE_3*W;
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
	return orgm_resutl;
}




/*
Genetic_Encoding Population::crossover(Genetic_Encoding orgm1, Genetic_Encoding orgm2){
	Genetic_Encoding orgm_resutl;
	Genetic_Encoding * orgm_greater_connections;
	Genetic_Encoding * orgm_lower_connections;
	Genetic_Encoding * orgm_greater_nodes;
	Genetic_Encoding * orgm_lower_nodes;

	if( orgm1.Lconnection_genes.size() > orgm2.Lconnection_genes.size() ){
		orgm_greater_connections = &orgm1;
		orgm_lower_connections = &orgm2;
	}
	else{
		orgm_greater_connections = &orgm2;
		orgm_lower_connections = &orgm1;
	}
	if(orgm1.Lnode_genes.size() > orgm2.Lnode_genes.size()){
		orgm_greater_nodes = &orgm1;
		orgm_lower_nodes = &orgm2;
	}
	else{
		orgm_greater_nodes = &orgm2;
		orgm_lower_nodes = &orgm1;
	}
	//==========================CONNECTION=================
	for(int i = 0; i < (int)orgm_lower_connections->Lconnection_genes.size();i++){
		if(orgm1.Lconnection_genes.at(i).exist && orgm2.Lconnection_genes.at(i).exist ){
			if(rand()%10 < 5 ){
				orgm_resutl.add_connection(orgm1.Lconnection_genes.at(i));
			}
			else{
				orgm_resutl.add_connection(orgm2.Lconnection_genes.at(i));
			}
		}
		else{
			if( orgm1.Lconnection_genes.at(i).exist ){
				orgm_resutl.add_connection(orgm1.Lconnection_genes.at(i));
			}
			else if(orgm2.Lconnection_genes.at(i).exist){
				orgm_resutl.add_connection(orgm2.Lconnection_genes.at(i));
			}
		}
	}
	for(int i =  (int)orgm_lower_connections->Lconnection_genes.size(); i < (int)orgm_greater_connections->Lconnection_genes.size();i++){
		if(orgm_greater_connections->Lconnection_genes.at(i).exist)
			orgm_resutl.add_connection(orgm_greater_connections->Lconnection_genes.at(i));
	}
	//===================================================

	//==========================NODES=====================
	for(int i = 0; i < (int)orgm_lower_nodes->Lnode_genes.size();i++){
			if( orgm1.Lnode_genes.at(i).exist ){
				orgm_resutl.add_node(orgm1.Lnode_genes.at(i));
			}
			else if(orgm2.Lnode_genes.at(i).exist){
				orgm_resutl.add_node(orgm2.Lnode_genes.at(i));
			}
	}
	for(int i =  (int)orgm_lower_nodes->Lnode_genes.size(); i < (int)orgm_greater_nodes->Lnode_genes.size();i++){
		if(orgm_greater_nodes->Lnode_genes.at(i).exist)
			orgm_resutl.add_node(orgm_greater_nodes->Lnode_genes.at(i) );
	}
	//===================================================

	orgm_resutl.row_orderer_list = row_orderer_list;
	return orgm_resutl;
}







*/















































/*
void Population::spatiation(){
	vector<Niche>().swap(prev_niches);
	prev_niches = current_niches;
	vector<Niche>().swap(current_niches);


	vector <Niche> real_niches;
	vector <Niche> current_niches_temp;
	for (int i = 0; i < (int)prev_niches.size(); ++i)
	{
		if (prev_niches.at(i).exist)
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

			if(compatibility(organisms.at(j), prev_organisms[real_niches[i].niche_champion_position]) < DISTANCE_THRESHOLD ){
				have_niche = true;
				current_niches_temp.at(i).exist=true;
				current_niches_temp.at(i).organism_position.push_back(j);
				current_niches_temp.at(i).niche_champion_position=j; // this is temporal until in function epoch the real champion is decided respect its fitness
				break;
			}

		}

		if (!have_niche)
		{
			for (int i = 0; i < amount_of_new_niches; ++i)
			{
				if( compatibility( organisms[j], organisms[current_niches_temp[i + (int)real_niches.size()].niche_champion_position] ) < DISTANCE_THRESHOLD ){
					have_niche = true;
					current_niches_temp.at(i + (int)real_niches.size()).exist=true;
					current_niches_temp.at(i + (int)real_niches.size()).organism_position.push_back(j);
					current_niches_temp.at(i + (int)real_niches.size()).niche_champion_position=j; // this is temporal until in function epoch the real champion is decided respect its fitness
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
			for (int j = 0; j < (int)current_niches.at(i).organism_position.size(); ++j)
			{
				organisms.at(current_niches.at(i).organism_position.at(j)).niche = i;
			}
	}

}

*/

//		PAPER VERSION.



void Population::spatiation(){
	//Se hace el traspaso generacional, ahora los nichos actuales pasan a ser nichos previos.
	vector<Niche>().swap(prev_niches);
	prev_niches = current_niches;
	vector<Niche>().swap(current_niches);


	vector <Niche> real_niches;
	vector <Niche> current_niches_temp;

	for (int i = 0; i < (int)prev_niches.size(); ++i)
	{
		if (prev_niches.at(i).exist )
		{
			if(prev_niches.at(i).organism_position.size() >0)
				real_niches.push_back(prev_niches[i]);
		}
	}

	Niche aux_niche;
	aux_niche.exist=false;
	for(int i=0; i < (int)real_niches.size(); i++){
		current_niches_temp.push_back(aux_niche);
	}
	// Obtain an random representant
	vector <int> representant_of_niche;
	for(int i = 0; i < (int)real_niches.size(); i++){
		representant_of_niche.push_back( real_niches.at(i).organism_position.at(rand()%real_niches.at(i).organism_position.size()) );
	}

	for(int i=0; i < (int)organisms.size(); i++ ){
			bool have_niche(false);
			//cerr << "ENTRO41" << endl;
			for(int j=0; j < (int)real_niches.size(); j++){
				if( compatibility(organisms.at(i), prev_organisms.at( representant_of_niche.at(j) ) )  < DISTANCE_THRESHOLD ){
					have_niche=true;
					current_niches_temp.at(j).exist=true;
					current_niches_temp.at(j).organism_position.push_back(i);
					break;
				}
			}
			if(!have_niche){
				for(int j= (int)real_niches.size(); j <   (int)current_niches_temp.size() ; j++){
					if( compatibility(organisms.at(i), organisms.at( representant_of_niche.at(j) ) )  < DISTANCE_THRESHOLD ){
						have_niche=true;
						current_niches_temp.at(j).organism_position.push_back(i);
						break;
					}
				}
			}
			if(!have_niche){
				Niche temp_niche;
				temp_niche.exist=true;
				current_niches_temp.push_back(temp_niche);
				representant_of_niche.push_back(i);
				current_niches_temp.at(current_niches_temp.size()-1).organism_position.push_back(i);
			}
	}
	for (int i = 0; i < (int)current_niches_temp.size(); ++i)
		if( current_niches_temp.at(i).exist ) current_niches.push_back(current_niches_temp.at(i));
	for (int i = 0; i < (int)current_niches.size(); ++i)
	{
			for (int j = 0; j < (int)current_niches.at(i).organism_position.size(); ++j)
			{
				organisms.at(current_niches.at(i).organism_position.at(j)).niche = i;
			}
	}
}

































































































void Population::print_niches(){
	for (int i = 0; i < (int)current_niches.size(); ++i)
		if(current_niches[i].exist)
			cout << "Niche " << i << ":\t\t\t" <<  current_niches[i].organism_position.size() << endl;
		else
			cout << "Niche " << i << ":\t\t\t0\n";
}













































/*

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
	//	Luego se obtiene un n�mero random, dependiendo del random se elije el nicho
	//  Obviamente nichos con mayor fitness tienen m�s probabilidades de tener m�s hijos.

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


*/



void Population::epoch(){
	double total_shared_fitness_population(0.0);
	// Se calculan todos los fitness de todos los organismos.
	for (int i = 0; i < (int)organisms.size(); ++i){
		organisms.at(i).fitness = fitness(organisms[i]);
	}

	//Se eliminan a los peores de cada nicho.
	vector < Genetic_Encoding > temp_current_organisms;
	vector <Niche> temp_current_niches;

	for(int i=0; i < (int)current_niches.size(); i++){
		Niche temp_niche;
		double media_nicho = 0;
		double max_fitness = 0;
		int count=0;
		vector <int> temp_orgm_pos;

		// Para hacer calculos estadísticos se encontrarán el máximo, la media, y la desviación.
		for(int j=0; j < (int)current_niches.at(i).organism_position.size(); j++){
			if(organisms.at(current_niches.at(i).organism_position.at(j)).fitness > max_fitness) max_fitness =organisms.at(current_niches.at(i).organism_position.at(j)).fitness ;
			media_nicho += organisms.at(current_niches.at(i).organism_position.at(j)).fitness;
		}
		media_nicho = media_nicho/(double)current_niches.at(i).organism_position.size();
		double desv_estandar = 0.0;
		for(int j=0; j<(int)current_niches.at(i).organism_position.size();j++){
			desv_estandar += pow(media_nicho - organisms.at(current_niches.at(i).organism_position.at(j)).fitness,2);
		}
		desv_estandar=sqrt(desv_estandar/(double)current_niches.at(i).organism_position.size());




		//Se eligen los mejores para pasar a pertenecer a organisms y los peores dejarán de pertenecer, para eso se usan los temporales.
		for(int j=0; j<(int)current_niches.at(i).organism_position.size();j++){
			if(  organisms.at(current_niches.at(i).organism_position.at(j)).fitness  >= media_nicho  /*+ desv_estandar/4 */ ){
				temp_current_organisms.push_back(organisms.at(current_niches.at(i).organism_position.at(j)));
				temp_niche.organism_position.push_back(temp_current_organisms.size() -1);
				count++;
			}
		}
		temp_current_niches.push_back(temp_niche);
		//current_niches.swap(temp_current_niches);
		//organisms.swap(temp_current_organisms);



		cerr << "media: " << media_nicho << "\tdesv: " << desv_estandar << "\t maximo: "  <<  max_fitness  << "\t corte: " << media_nicho  << endl;


	}


	current_niches = temp_current_niches;
	organisms = temp_current_organisms;


	// Se verifica si el campeón de todos los tiempos pertenece a esta población.
	for (int i = 0; i < (int)current_niches.size(); ++i)
	{

		current_niches.at(i).total_shared_fitness = 0;
		for (int j = 0; j < (int)current_niches.at(i).organism_position.size(); ++j)
		{
			if( organisms.at(current_niches.at(i).organism_position.at(j)).fitness > fitness_champion ){
				fitness_champion	= organisms.at( current_niches.at(i).organism_position.at(j) ).fitness;
				champion			= organisms.at( current_niches.at(i).organism_position.at(j) );
			}

			organisms.at(current_niches.at(i).organism_position.at(j)).shared_fitness = organisms.at(current_niches.at(i).organism_position.at(j)).fitness/(double)current_niches.at(i).organism_position.size();
			total_shared_fitness_population += organisms.at(current_niches.at(i).organism_position.at(j)).shared_fitness;
		}
	}



	// Se comienza el traspaso generacional, ahora los organismos que eras actuales pasarán a ser los previos.
	vector < Genetic_Encoding >().swap(prev_organisms); // Se limpia prev_organisms
	prev_organisms = organisms;
	vector < Genetic_Encoding >().swap(organisms); // Se limpia organisms


	// Se agregan los organismos en un objeto que representa las probabilidades, para luego obtener organismos aleatoreamente.
	Discrete_Probabilities organisms_probabilities;
	for(int i=0; i < (int)prev_organisms.size(); i++)
	{
		organisms_probabilities.add_element(prev_organisms.at(i).shared_fitness);
	}


	// La probabilidad de que un organismo tenga un hijo es proporcional a su shared fitness.
	for(int i=0; i < POPULATION_MAX; i++ ){
		int random_organism = organisms_probabilities.obtain_uniformrandom_element();
		organisms.push_back( epoch_reproduce( prev_organisms.at( random_organism ) , random_organism )   ); // 	QUIZÁS NO ES RANDOM_OPRGANISM EL CORRECTO SEGUNDO PARAMETRO
	}

	spatiation();
}















Genetic_Encoding Population::epoch_reproduce(Genetic_Encoding organism, int poblation_place){
	int random_mother; // for mating
	int random_niche_mother;
	Genetic_Encoding organism_father;
	Genetic_Encoding organism_mother;// for mating

	if(100 *((double)rand())/RAND_MAX < PERCENTAGE_OFFSPRING_WITHOUT_CROSSOVER){
			organism = mutation_change_weight(organism);
	}
	else{
		if( ((double)rand())/RAND_MAX < PROBABILITY_INTERSPACIES_MATING){
			while(true){
				random_niche_mother = rand()%current_niches.size();
				if(random_niche_mother != organism.niche)break;
				if(current_niches.size() == 1 ){
					//cerr << "Warning:: In function Epoch:: Exist only one niche\n";
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
				random_mother = rand()%current_niches.at(organism.niche).organism_position.size();
				if(current_niches.at(organism.niche).organism_position.at(random_mother) != poblation_place )break;
				if(current_niches.at(organism.niche).organism_position.size()==1)break;
			}

			organism_mother = prev_organisms.at(current_niches.at(organism.niche).organism_position.at(random_mother));
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
	ORGANISM_DISTANCE_1 = atof(pch);
	pch = strtok (NULL,delimiters);
	pch = strtok (NULL,delimiters);
	ORGANISM_DISTANCE_2 = atof(pch);
	pch = strtok (NULL,delimiters);
	pch = strtok (NULL,delimiters);
	ORGANISM_DISTANCE_3 = atof(pch);
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
