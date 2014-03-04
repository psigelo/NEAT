#ifndef GENETIC_ENCODING_CPP
#define GENETIC_ENCODING_CPP

#include "genetic_encoding.hpp"
#include <stdio.h>
#include <cstring>

using namespace ANN_USM;

void connection_gene::c_g(int innovation, int in, int out, double weight, bool enable){
	this->innovation=innovation;
	this->in=in;
	this->out=out;
	this->weight=weight;
	this->enable=enable;
	this->exist=true;
}
void connection_gene::c_g(bool exist){
	this->exist=exist;
}

void node_gene::n_g(int node, gene_type type){
	this->node = node;
	this->type = type;
	this->exist = true;
}

void node_gene::n_g(bool exist){
	this->exist = exist;
}



void Genetic_Encoding::add_connection(int innovation, int in, int out, double weight){
	int list_size(Lconnection_genes.size());
	if(innovation >= list_size)
	{
		connection_gene missing_connection_gene;
		connection_gene new_connection;

		missing_connection_gene.c_g(false); // conection that not exist in this genoma
		new_connection.c_g(innovation,in,out,weight,true);

		for (int i = 0; i < innovation-list_size; ++i)
		{
			Lconnection_genes.push_back(missing_connection_gene);
		}
		Lconnection_genes.push_back(new_connection);
	}

	else 
		if(Lconnection_genes[innovation].exist)
			cerr << "ERROR::In function add_connection, you wanted to add a connection gene with an innovation that already exists" << endl;	
		else
			Lconnection_genes[innovation].c_g(innovation,in,out,weight,true);
}



void Genetic_Encoding::add_connection(connection_gene conn){
	int list_size(Lconnection_genes.size());
	if(conn.innovation >= list_size)
	{
		connection_gene missing_connection_gene;
		connection_gene new_connection;

		missing_connection_gene.c_g(false); // conection that not exist in this genoma
		new_connection.c_g(conn.innovation,conn.in,conn.out,conn.weight, conn.enable);

		for (int i = 0; i < conn.innovation-list_size; ++i)
		{
			Lconnection_genes.push_back(missing_connection_gene);
		}
		Lconnection_genes.push_back(new_connection);
	}

	else 
		if(Lconnection_genes[conn.innovation].exist)
			cerr << "ERROR::In function add_connection, you wanted to add a connection gene with an innovation that already exists" << endl;	
		else
			Lconnection_genes[conn.innovation].c_g(conn.innovation,conn.in,conn.out,conn.weight,conn.enable);
}





void Genetic_Encoding::add_node(int node, int row , gene_type type){
	int list_size(Lnode_genes.size());
	if(node >= list_size)
	{
		node_gene missing_node_gene;
		node_gene new_node;

		missing_node_gene.n_g(false);
		new_node.n_g(node, type);
		new_node.row=row;
		for (int i = 0; i < node - list_size; ++i)
		{
			Lnode_genes.push_back(missing_node_gene);
		}
		Lnode_genes.push_back(new_node);
	}
	else
		if(Lnode_genes[node].exist)
			cerr << "ERROR::In function add_node , you wanted to add a node gene with a node number that already exists" << endl;	
		else{
			Lnode_genes[node].n_g(node, type);
			Lnode_genes[node].row = row;
		}
}

void Genetic_Encoding::add_node(node_gene node){
	int list_size(Lnode_genes.size());
	if(node.node >= list_size)
	{
		node_gene missing_node_gene;
		node_gene new_node;

		missing_node_gene.n_g(false);
		new_node.n_g(node.node, node.type);

		for (int i = 0; i < node.node - list_size; ++i)
		{
			Lnode_genes.push_back(missing_node_gene);
		}
		new_node.row = node.row;
		Lnode_genes.push_back(new_node);
	}
	else
		if(Lnode_genes[node.node].exist)
			cerr << "ERROR::In function add_node , you wanted to add a node gene with a node number that already exists" << endl;	
		else{
			Lnode_genes[node.node].n_g(node.node, node.type);
			Lnode_genes[node.node].row = node.row;
		}
}

ostream & operator<<(ostream & o, Genetic_Encoding & encoding) { 
	o << encoding.JSON();	
	return o;
}



string Genetic_Encoding::JSON(){
	stringstream o;
	o << "{\n\t\"Genetic_Encoding\":\n\t{\n\t\t\"nodes\":\n\t\t[\n";

	int node_size(Lnode_genes.size());
	int connection_size(Lconnection_genes.size());

	for (int i = 0; i < node_size; ++i)
	{
		if(Lnode_genes[i].exist)
			o << "\t\t\t{\"exist\": " << Lnode_genes[i].exist  << ",\"node\": " <<Lnode_genes[i].node << ",\"type\": " << Lnode_genes[i].type << ", \"row\": " << Lnode_genes[i].row;
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
	int contador(0);
	bool connection(false);
	int row;
	char * pch;
	char delimiters[] = " \n\":\t{},[";
	pch = strtok (buffer,delimiters);
	
	do{
		pch = strtok (NULL, delimiters);
		if(!(pch[0] == ']')){
			if(connection){
				if(!strncmp(pch, "exist",5)){
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
						Cnew_node.c_g(innovation, in, out, weight, (bool)enable);
						Lconnection_genes.push_back(Cnew_node);
					}
					else{
						Cnew_node.c_g(false);
						Lconnection_genes.push_back(Cnew_node);
					}
				} 
			}
			else{
				if(!strncmp(pch, "exist",5)){
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
						Nnew_node.n_g(node, (gene_type) type);
						Nnew_node.row=row;
						Lnode_genes.push_back(Nnew_node);
					}
					else{
						Nnew_node.n_g(false);
						Lnode_genes.push_back(Nnew_node);
					}
				} 
			}
		}
		else{
			contador=contador+1;
			connection = true;
			if(contador==2){
				break;
			}
		}
	}while (pch != NULL);
}



vector <double> Genetic_Encoding::eval(std::vector<double> inputs){
	
	int amount_inputs_nodes(0);
	int amount_outputs_nodes(0);
	int amount_hiden_nodes (0);



	vector < vector < int> > nodes_row;
	vector < int > empty_vector;
	for (int i = 0; i < (int)row_orderer_list.size(); ++i)
		nodes_row.push_back(empty_vector);


	vector <int> outputs_positions;
	for(int i=0; i < (int)Lnode_genes.size() ; i++){

		if(Lnode_genes[i].exist){

			if(Lnode_genes[i].type == INPUT){
				amount_inputs_nodes++;
			}
			else if(Lnode_genes[i].type == OUTPUT){
				amount_outputs_nodes++;
				outputs_positions.push_back(i);
			}
			else{
				amount_hiden_nodes++;
			}

			nodes_row[Lnode_genes[i].row].push_back(i);

		}
	}
	if( amount_inputs_nodes != (int)inputs.size()){
		cerr << "ERROR:: In function Genetic_Encoding::eval, the amount of inputs is not correct!.";
		exit(1);
	}


	double entradas_temp(0.0);

	vector < vector <int> > inputs_to_node;
	for (int i = 0; i < (int)Lnode_genes.size(); ++i)
		inputs_to_node.push_back(empty_vector);


	for (int i = 0; i < (int)Lconnection_genes.size(); ++i)
	{
		if(Lconnection_genes[i].exist && Lconnection_genes[i].enable) inputs_to_node[Lconnection_genes[i].out].push_back(i);
	}

	//inputs
	for (int i = 0; i < (int)inputs.size(); ++i)
	{
		Lnode_genes[i].node_output_value = inputs[i];
	}

	for (int i = 1; i < (int)row_orderer_list.size(); ++i)
	{
		
		for (int j = 0; j < (int)nodes_row[row_orderer_list[i]].size(); ++j)
			{
				entradas_temp = 0;
				for (int k = 0; k < (int)inputs_to_node[nodes_row[row_orderer_list[i]][j]].size(); ++k)
				{
					entradas_temp += Lnode_genes[  Lconnection_genes[  inputs_to_node[ nodes_row[row_orderer_list[i]][j] ][k]  ].in  ].node_output_value * Lconnection_genes[   inputs_to_node[ nodes_row[row_orderer_list[i]][j] ][k]   ].weight;
				}
				Lnode_genes[nodes_row[row_orderer_list[i]][j]].node_output_value = Fsigmoide(entradas_temp);
			}	
	}

	vector <double> outputs;
	for (int i = 0; i < (int)outputs_positions.size(); ++i)
	{
		outputs.push_back(Lnode_genes[outputs_positions[i]].node_output_value);
	}

	return outputs;
}
  


string Genetic_Encoding::ANN_function(){
	
	
	int amount_inputs_nodes(0);
	int amount_outputs_nodes(0);
	int amount_hiden_nodes (0);



	vector < vector < int> > nodes_row;
	vector < int > empty_vector;
	for (int i = 0; i < (int)row_orderer_list.size(); ++i)
		nodes_row.push_back(empty_vector);


	vector <int> outputs_positions;
	for(int i=0; i < (int)Lnode_genes.size() ; i++){

		if(Lnode_genes[i].exist){

			if(Lnode_genes[i].type == INPUT){
				amount_inputs_nodes++;
			}
			else if(Lnode_genes[i].type == OUTPUT){
				amount_outputs_nodes++;
				outputs_positions.push_back(i);
			}
			else{
				amount_hiden_nodes++;
			}

			nodes_row[Lnode_genes[i].row].push_back(i);

		}
	}
	



	vector < vector <int> > inputs_to_node;
	for (int i = 0; i < (int)Lnode_genes.size(); ++i)
		inputs_to_node.push_back(empty_vector);


	for (int i = 0; i < (int)Lconnection_genes.size(); ++i)
	{
		if(Lconnection_genes[i].exist && Lconnection_genes[i].enable) inputs_to_node[Lconnection_genes[i].out].push_back(i);
	}


	stringstream ss;

	//inputs 
	for (int i = 0; i < amount_inputs_nodes; ++i)
	{
		ss << "X" << i;
		Lnode_genes[i].str  = ss.str();
		ss.str("");
	}

	for (int i = 1; i < (int)row_orderer_list.size(); ++i)
	{
		
		for (int j = 0; j < (int)nodes_row[row_orderer_list[i]].size(); ++j)
			{
				for (int k = 0; k < (int)inputs_to_node[nodes_row[row_orderer_list[i]][j]].size(); ++k)
				{
					if(k==0){
						ss << "sgmd";
						if(true){// Para representar que con esto se puede cambiar para mathematica o para otro lenguaje
							ss << "[";
						}
					}
					else{
						ss << "+";
					}
					ss << Lnode_genes[  Lconnection_genes[  inputs_to_node[ nodes_row[row_orderer_list[i]][j] ][k]  ].in  ].str << "*" << Lconnection_genes[   inputs_to_node[ nodes_row[row_orderer_list[i]][j] ][k]   ].weight;
				}
				if(true){ // Para representar que con esto se puede cambiar para mathematica o para otro lenguaje
					ss << "]";
				}
				//cerr << ss.str() << endl;
				Lnode_genes[nodes_row[row_orderer_list[i]][j]].str = ss.str();
				ss.str("");
			}	
	}

	stringstream outputs;
	for (int i = 0; i < (int)outputs_positions.size(); ++i)
	{
		outputs << Lnode_genes[outputs_positions[i]].str << endl;
	}
	
	return outputs.str();

}


double Genetic_Encoding::Fsigmoide(double x){
	return (1/(1+exp(SIGMOID_CONSTANT*x)));
}
#endif