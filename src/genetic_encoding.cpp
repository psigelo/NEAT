#ifndef GENETIC_ENCODING_CPP
#define GENETIC_ENCODING_CPP

#include "genetic_encoding.hpp"

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
	//connection_gene * new_connection = new connection_gene(int innovation, int in, int out, double weight);
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



void Genetic_Encoding::add_node(int node, gene_type type){
	int list_size(Lnode_genes.size());
	if(node >= list_size)
	{
		node_gene missing_node_gene;
		node_gene new_node;

		missing_node_gene.n_g(false);
		new_node.n_g(node, type);

		for (int i = 0; i < node - list_size; ++i)
		{
			Lnode_genes.push_back(missing_node_gene);
		}
		Lnode_genes.push_back(new_node);
	}
	else
		if(Lnode_genes[node].exist)
			cerr << "ERROR::In function add_node , you wanted to add a node gene with a node number that already exists" << endl;	
		else
			Lnode_genes[node].n_g(node, type);
			
}


ostream & operator<<(ostream & o, Genetic_Encoding & encoding) { 
	o << "{\n\t\"Genetic_Encoding\":\n\t{\n\t\t\"nodes\":\n\t\t[\n";

	int node_size(encoding.Lnode_genes.size());
	int connection_size(encoding.Lconnection_genes.size());

	for (int i = 0; i < node_size-1; ++i)
	{
		if(encoding.Lnode_genes[i].exist){
			o << "\t\t\t{\"exist\": " << encoding.Lnode_genes[i].exist  << ",\"node\": " <<encoding.Lnode_genes[i].node << ",\"type\": " << encoding.Lnode_genes[i].type << "},\n";
		}
		else{
			o << "\t\t\t{\"exist\": " << encoding.Lnode_genes[i].exist << "},\n";
		}
	}

	if(encoding.Lnode_genes[node_size-1].exist)
	{
		o << "\t\t\t{\"exist\": " << encoding.Lnode_genes[node_size-1].exist  << ",\"node\": " <<encoding.Lnode_genes[node_size-1].node << ",\"type\": " << encoding.Lnode_genes[node_size-1].type << "}\n";
	}
	else{
		o << "\t\t\t{\"exist\": " << encoding.Lnode_genes[node_size-1].exist << "}\n";
	}


	o << "\t\t],\n\t\t\"connection_genes\":\n\t\t[\n";
	for (int i = 0; i < connection_size-1; ++i)
	{
		if(encoding.Lconnection_genes[i].exist){
			o << "\t\t\t{\"exist\": " << encoding.Lconnection_genes[i].exist << ",\"innovation\": " << encoding.Lconnection_genes[i].innovation << ",\"in\": " << encoding.Lconnection_genes[i].in << ",\"out\": " << encoding.Lconnection_genes[i].out << ",\"weight\": " << encoding.Lconnection_genes[i].weight << ",\"enable\": " << encoding.Lconnection_genes[i].enable << "},\n";
		}
		else{
			o << "\t\t\t{\"exist\": " << encoding.Lconnection_genes[i].exist << "},\n";
		}
	}

	if(encoding.Lconnection_genes[connection_size-1].exist)
	{
		o << "\t\t\t{\"exist\": " << encoding.Lconnection_genes[connection_size-1].exist << ",\"innovation\": " << encoding.Lconnection_genes[connection_size-1].innovation << ",\"in\": " << encoding.Lconnection_genes[connection_size-1].in << ",\"out\": " << encoding.Lconnection_genes[connection_size-1].out << ",\"weight\": " << encoding.Lconnection_genes[connection_size-1].weight << ",\"enable\": " << encoding.Lconnection_genes[connection_size-1].enable << "}\n";
	}
	else
	{
		o << "\t\t\t{\"exist\": " << encoding.Lconnection_genes[connection_size-1].exist << "}\n";
	}
	
	o << "\t\t]\n\t}\n}";
	return o;
}



#endif