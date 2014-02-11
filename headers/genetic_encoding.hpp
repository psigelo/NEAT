#ifndef GENETIC_ENCODING_hPP
#define GENETIC_ENCODING_hPP


#include <vector>
#include <cstdlib>
#include <iostream>

using namespace std;

namespace ANN_USM{
	enum gene_type{
		INPUT,
		HIDDEN,
		OUTPUT
	};

	class connection_gene{
		public:
			void c_g(int innovation, int in, int out, double weight, bool enable);
			void c_g(bool exist);
			int innovation;
			int in;
			int out;
			bool enable;
			double weight;
			bool exist; 
	};

	class node_gene{
		public:
			void n_g(int node, gene_type type);
			void n_g(bool exist);
			bool exist;
			int node;
			gene_type type;
	};

	class Genetic_Encoding{
		public:
			//Genetic_Encoding();
			void add_node(int node, gene_type type);
			void add_connection(int innovation, int in, int out, double weight);
			void change_weight(int innovation, double weight);
			Genetic_Encoding * duplicate();
			//friend ostream& operator<<(ostream &o, Genetic_Encoding & encoding);
		
			vector <connection_gene> Lconnection_genes; //List of connections genes
			vector <node_gene> Lnode_genes;
	};

	
}

ostream & operator<<(ostream & o, ANN_USM::Genetic_Encoding & encoding);


#endif