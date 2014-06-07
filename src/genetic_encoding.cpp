#ifndef GENETIC_ENCODING_CPP
#define GENETIC_ENCODING_CPP

#include "genetic_encoding.hpp"


using namespace ANN_USM;


// ================================ CONNECTION_GENE ====================================================//
void connection_gene::_connection_gene(int innovation, int in, int out, double weight, bool enable){
	this->innovation=innovation;
	this->in=in;
	this->out=out;
	this->weight=weight;
	this->enable=enable;
	this->exist=true;
}
void connection_gene::_connection_gene(bool exist){
	this->exist=exist;
}
//======================================================================================================//



// ================================ NODE_GENE ====================================================//
void node_gene::_node_genne(int node, gene_type type, int row, Return_random_function random_function){
	this->node  = node;
	this->type  = type;
	this->row   = row;
	this->exist = true;
	this->random_function = random_function;
	this->node_output_value = 0;
}
void node_gene::_node_genne(bool exist){
	this->exist = exist;
	this->node_output_value = 0;
}

void node_gene::change_random_function_randomly(){
	Return_random_function rf;
	rf = get_random_function();
	this->random_function = rf;
}
//=================================================================================================//









int Genetic_Encoding::getNEATOutputSize(){
	return((int)nodes_at_row.at(row_orderer_list.at(row_orderer_list.size()-1)).size());
}



/*
procedure:
    if the historical value of the node is greater than the greater historical node
	then is created all nodes that are between both with the variable exist equal
	to 0 and then is created the node in its historical node position.

	in other case the node is present in the genome and only is changed its values.
*/
void Genetic_Encoding::add_node(int node, int row , gene_type type){


	int list_size(Lnode_genes.size()); //amount of node genes in this genome, present or not present (exist = true or false).

	if(node >= list_size)
	{
		node_gene missing_node_gene;
		missing_node_gene._node_genne(false);
		Return_random_function rf;
		for (int i = 0; i < node - list_size; ++i)
			Lnode_genes.push_back(missing_node_gene);
		node_gene new_node;
		rf = get_random_function();

		new_node._node_genne(node, type, row, rf);
		Lnode_genes.push_back(new_node);
	}
	else{
		if(Lnode_genes[node].exist){
			cerr << "ERROR::In function add_node , you wanted to add a node gene with a node number that already exists" << endl;
			exit(1);
		}
		else{
			Return_random_function rf;
			rf = get_random_function();
			Lnode_genes[node]._node_genne(node, type, row, rf);
		}
	}

	//=============FOR EVAL() FUNCTION==========================//
	while( row > (int)nodes_at_row.size() -1 ){
		vector <int> empty;
		nodes_at_row.push_back(empty);
	}
	nodes_at_row.at(row).push_back( node );
	if(type == OUTPUT) outputs_positions.push_back( node );
	//==========================================================//
}








/*
procedure:
    if the historical value of the node is greater than the greater historical node
	then is created all nodes that are between both with the variable exist equal
	to 0 and then is created the node in its historical node position.

	in other case the node is present in the genome and only is changed its values.
*/
void Genetic_Encoding::add_node(node_gene node){

	int list_size(Lnode_genes.size());

	if(node.node >= list_size)
	{
		node_gene missing_node_gene;
		missing_node_gene._node_genne(false);
		for (int i = 0; i < node.node - list_size; ++i)
			Lnode_genes.push_back(missing_node_gene);
		node_gene new_node;
		new_node._node_genne(node.node, node.type, node.row, node.random_function);
		Lnode_genes.push_back(new_node);
	}
	else
		if(Lnode_genes[node.node].exist){
			cerr << "ERROR::In function add_node , you wanted to add a node gene with a node number that already exists" << endl;
			exit(1);
		}
		else
			Lnode_genes[node.node]._node_genne(node.node, node.type, node.row, node.random_function);

	//=============FOR EVAL() FUNCTION==========================//
	while( node.row > (int)nodes_at_row.size() -1 ){
		vector <int> empty;
		nodes_at_row.push_back(empty);
	}
	nodes_at_row.at(node.row).push_back( node.node );
	if(node.type == OUTPUT) outputs_positions.push_back( node.node );
	//==========================================================//
}



/*
procedure:
    if the innovation value of the connection is greater than the greater innovation value
	then is created all connection that are between both with the variable exist equal
	to 0 and then is created the connection in its innovation position.

	in other case the connection is present in the genome and only is changed its values.
*/
void Genetic_Encoding::add_connection(int innovation, int in, int out, double weight){
	int list_size(Lconnection_genes.size());
	if(innovation >= list_size)
	{
		connection_gene missing_connection_gene;
		missing_connection_gene._connection_gene(false); // conection that not exist in this genoma
		for (int i = 0; i < innovation-list_size; ++i)
			Lconnection_genes.push_back(missing_connection_gene);
		connection_gene new_connection;
		new_connection._connection_gene(innovation,in,out,weight,true);
		Lconnection_genes.push_back(new_connection);

	}

	else
		if(Lconnection_genes[innovation].exist)
			cerr << "ERROR::In function add_connection, you wanted to add a connection gene with an innovation that already exists" << endl;
		else
			Lconnection_genes[innovation]._connection_gene(innovation,in,out,weight,true);
}


/*
procedure:
    if the innovation value of the connection is greater than the greater innovation value
	then is created all connection that are between both with the variable exist equal
	to 0 and then is created the connection in its innovation position.

	in other case the connection is present in the genome and only is changed its values.
*/
void Genetic_Encoding::add_connection(connection_gene conn){
	int list_size(Lconnection_genes.size());
	if(conn.innovation >= list_size)
	{
		connection_gene missing_connection_gene;
		missing_connection_gene._connection_gene(false); // conection that not exist in this genoma
		for (int i = 0; i < conn.innovation-list_size; ++i)
			Lconnection_genes.push_back(missing_connection_gene);
		connection_gene new_connection;
		new_connection._connection_gene(conn.innovation,conn.in,conn.out,conn.weight, conn.enable);
		Lconnection_genes.push_back(new_connection);
	}
	else
		if(Lconnection_genes[conn.innovation].exist)
			cerr << "ERROR::In function add_connection, you wanted to add a connection gene with an innovation that already exists" << endl;
		else
			Lconnection_genes[conn.innovation]._connection_gene(conn.innovation,conn.in,conn.out,conn.weight,conn.enable);
}











string Genetic_Encoding::JSON(){
	int 			node_size 		(Lnode_genes.size());
	int 			connection_size (Lconnection_genes.size());
	stringstream 	o;
	o << "{\n\t\"Genetic_Encoding\":\n\t{" ;


	o << "\n\t\t\"row_order\":\n\t\t[\n\t\t\t";
	for (int i = 0; i < (int)row_orderer_list.size(); ++i)
	{
		o << row_orderer_list[i];
		if(i < (int)row_orderer_list.size()-1)
			o << ",";
		else{
			o << "\n\t\t]";
		}
	}

	o << "\n\t\t\"nodes\":\n\t\t[\n";
	for (int i = 0; i < node_size; ++i)
	{
		if(Lnode_genes[i].exist)
			o << "\t\t\t{\"exist\": " << Lnode_genes[i].exist  << ",\"node\": " <<Lnode_genes[i].node << ",\"type\": " << Lnode_genes[i].type << ", \"row\": " << Lnode_genes[i].row << ", \"function\": \"" << Lnode_genes[i].random_function.str_name << "\"";
		else
			o << "\t\t\t{\"exist\": " << Lnode_genes[i].exist ;
		if(i<node_size-1)
			o <<  "},\n";
		else
			o <<  "}\n";
	}

	o << "\t\t],\n\t\t\"connection_genes\":\n\t\t[\n";
	for (int i = 0; i < connection_size; ++i)
	{
		if(Lconnection_genes[i].exist)
			o << "\t\t\t{\"exist\": " << Lconnection_genes[i].exist << ",\"innovation\": " << Lconnection_genes[i].innovation << ",\"in\": " << Lconnection_genes[i].in << ",\"out\": " << Lconnection_genes[i].out << ",\"weight\": " << Lconnection_genes[i].weight << ",\"enable\": " << Lconnection_genes[i].enable;
		else
			o << "\t\t\t{\"exist\": " << Lconnection_genes[i].exist;

		if(i<connection_size-1)
			o <<  "},\n";
		else
			o <<  "}\n";
	}

	o << "\t\t]\n\t}\n}";
	return o.str();
}





void Genetic_Encoding::save(char path[]){
	ofstream file;
	file.open (path);
	file << JSON();
	file.close();
}



void Genetic_Encoding::load(char path[]){

	node_gene Nnew_node;
	connection_gene Cnew_node;

	Lconnection_genes.clear();
	Lnode_genes.clear();

	ifstream file (path);
	file.seekg (0, file.end);
    int length = file.tellg();
    file.seekg (0, file.beg);
	char buffer[length]; // In JSON format
	file.read (buffer,length);
	file.close();

	bool exist;
	int node;
	int type;
	int innovation;
	int in;
	int out;
	double weight;
	int enable;
	int row;
	char * pch;
	char delimiters[] = " \n\":\t{},[";
	Return_random_function random_function;

	pch = strtok (buffer,delimiters);

	pch = strtok (NULL, delimiters);
	while( true ){ // rows
		pch = strtok (NULL, delimiters);
		if(pch[0] == ']')break;
		row_orderer_list.push_back(atoi(pch));
	}


	pch = strtok (NULL, delimiters);
	while( 1 ){ // nodes
		pch = strtok (NULL, delimiters);
		if(pch[0] == ']')break;
		pch = strtok (NULL, delimiters);
		exist = atoi(pch);
		if(exist){
			pch = strtok (NULL, delimiters);
			pch = strtok (NULL, delimiters);
			node = atoi(pch);
			pch = strtok (NULL, delimiters);
			pch = strtok (NULL, delimiters);
			type = atoi(pch);
			pch = strtok (NULL, delimiters);
			pch = strtok (NULL, delimiters);
			row = atoi(pch);
			pch = strtok (NULL, delimiters);
			pch = strtok (NULL, delimiters);
			random_function = obtain_function_fromm_name(pch);
			Nnew_node._node_genne(node, (gene_type) type, row, random_function);
			add_node(Nnew_node);
		}
		else{
			Nnew_node._node_genne(false);
			add_node(Nnew_node);
		}
	}

	pch = strtok (NULL, delimiters);
	while( true ){ // connections
		pch = strtok (NULL, delimiters);
		if(pch[0] == ']')break;
		pch = strtok (NULL, delimiters);
		exist = atoi(pch);
		if(exist){
			pch = strtok (NULL, delimiters);
			pch = strtok (NULL, delimiters);
			innovation = atoi(pch);
			pch = strtok (NULL, delimiters);
			pch = strtok (NULL, delimiters);
			in = atoi(pch);
			pch = strtok (NULL, delimiters);
			pch = strtok (NULL, delimiters);
			out = atoi(pch);
			pch = strtok (NULL, delimiters);
			pch = strtok (NULL, delimiters);
			weight = (double)atof(pch);
			pch = strtok (NULL, delimiters);
			pch = strtok (NULL, delimiters);
			enable = atoi(pch);
			Cnew_node._connection_gene(innovation, in, out, weight, (bool)enable);
			add_connection(Cnew_node);
		}
		else{
			Cnew_node._connection_gene(false);
			add_connection(Cnew_node);
		}
	}
}



/*
* falta: enlaces hacia atrás.
*/
vector <double> Genetic_Encoding::eval(std::vector<double> inputs){

	vector <double> 				outputs;
	vector < vector <int> > 	inputs_to_node;
	vector < int > 					  empty_vector;
	double entradas_temp(0.0);


	// ================= COSAS QUE PODRIAN MEJORARSE (POR VELOCIDAD)===================
	for (int i = 0; i < (int)Lnode_genes.size(); ++i)
		inputs_to_node.push_back(empty_vector);
	for (int i = 0; i < (int)Lconnection_genes.size(); ++i)
	{
		if(Lconnection_genes.at(i).exist && Lconnection_genes.at(i).enable) inputs_to_node.at(Lconnection_genes.at(i).out).push_back(i);
	}

	vector <int> row_orderer_list_adapted;
	for(int i = 0; i < (int)row_orderer_list.size(); i++)
	{
		if(row_orderer_list.at(i)  < (int)nodes_at_row.size()){
			if(nodes_at_row.at(row_orderer_list.at(i)).size() > 0){
				row_orderer_list_adapted.push_back(row_orderer_list.at(i));
			}
		}
	}
	//======================================================================




	// Loading inputs
	for (int i = 0; i < (int)inputs.size(); ++i)
	{
		Lnode_genes.at(i).node_output_value = (*Lnode_genes.at(i).random_function.function)(inputs[i]);
	}

	for (int i = 1; i < (int)row_orderer_list_adapted.size(); ++i)
	{
		for (int j = 0; j < (int)nodes_at_row.at(row_orderer_list_adapted.at(i)).size(); ++j)
		{
			entradas_temp = 0;
			int current_node_position = nodes_at_row.at(    row_orderer_list_adapted.at(i)    ).at( j );
			for (int k = 0; k < (int)inputs_to_node.at(current_node_position).size(); ++k)
			{
				connection_gene current_input_to_node_connection =  Lconnection_genes.at(  inputs_to_node.at( current_node_position ).at(k)  );
			 	entradas_temp += Lnode_genes.at(  current_input_to_node_connection.in  ).node_output_value * current_input_to_node_connection.weight;
			}
			Lnode_genes.at(current_node_position).node_output_value = (*Lnode_genes.at(current_node_position).random_function.function)(entradas_temp);

		}
	}
	for (int i = 0; i < (int)outputs_positions.size(); ++i)
	{
		outputs.push_back(Lnode_genes.at(outputs_positions.at(i)).node_output_value);
	}
	return outputs;
}





string Genetic_Encoding::ANN_function(){

	vector < vector <int> > 	inputs_to_node;
	vector < int > 					  empty_vector;
	int 					amount_inputs_nodes(0);




	// ================= COSAS QUE PODRIAN MEJORARSE (POR VELOCIDAD)===================
	for (int i = 0; i < (int)Lnode_genes.size(); ++i)
		inputs_to_node.push_back(empty_vector);
	for (int i = 0; i < (int)Lconnection_genes.size(); ++i)
	{
		if(Lconnection_genes[i].exist && Lconnection_genes.at(i).enable) inputs_to_node.at(Lconnection_genes.at(i).out).push_back(i);
	}

	vector <int> row_orderer_list_adapted;
	for(int i = 0; i < (int)row_orderer_list.size(); i++)
	{
		if(row_orderer_list.at(i)  < (int)nodes_at_row.size()){
			if(nodes_at_row.at(row_orderer_list.at(i)).size() > 0){
				row_orderer_list_adapted.push_back(row_orderer_list.at(i));
			}
		}
	}
	//======================================================================


	for(int i=0; i < (int)Lnode_genes.size() ; i++)
		if(Lnode_genes[i].exist)
			if(Lnode_genes[i].type == INPUT)
				amount_inputs_nodes++;

	stringstream ss;

	// Loading inputs
	for (int i = 0; i < amount_inputs_nodes; ++i)
	{
		ss << Lnode_genes.at(i).random_function.str_name << "[ ";
		ss << "X" << i;
		ss << " ]" ;
		Lnode_genes.at(i).str  = ss.str();
		ss.str("");
	}

	for (int i = 1; i < (int)row_orderer_list_adapted.size(); ++i)
	{
		for (int j = 0; j < (int)nodes_at_row.at(row_orderer_list_adapted.at(i)).size(); ++j)
		{
			int current_node_position = nodes_at_row[    row_orderer_list_adapted[i]    ] [ j ];
			
			for (int k = 0; k < (int)inputs_to_node.at(current_node_position).size(); ++k)
			{
				if(k==0){
					ss << " " << Lnode_genes[  current_node_position  ].random_function.str_name << "[ ";
				
				}
				else{
					ss << " + ";
				}

				connection_gene current_input_to_node_connection =  Lconnection_genes[  inputs_to_node[ current_node_position ][k] ];
			 	ss <<  Lnode_genes.at(  current_input_to_node_connection.in  ).str <<  " * " <<  current_input_to_node_connection.weight;
			}
			ss << "] ";
			Lnode_genes[nodes_at_row[row_orderer_list_adapted[i]][j]].str = ss.str();
			ss.str("");
		}
	}

	stringstream outputs;
	for (int i = 0; i < (int)outputs_positions.size(); ++i)
	{
		outputs << Lnode_genes.at(outputs_positions.at(i)).str << endl;
	}
	return outputs.str();
}



ostream & operator<<(ostream & o, Genetic_Encoding & encoding) {
	o << encoding.JSON();
	return o;
}


#endif
