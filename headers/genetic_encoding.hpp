#ifndef GENETIC_ENCODING_hPP
#define GENETIC_ENCODING_hPP


#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <strings.h>

using namespace std;

namespace ANN_USM{
	enum gene_type{
		INPUT,
		HIDDEN,
		OUTPUT
	};

	class connection_gene{
		public:
			void c_g(int innovation, int in, int out, double weight, bool enable); // fill a conection gene
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
			void n_g(int node, gene_type type); // fill a node gene
			void n_g(bool exist);
		
			int row;

			bool exist;
			int node;
			gene_type type;
	};

	class Genetic_Encoding{
		public:
			int niche;
			void add_node(int node, int row, gene_type type);
			void add_node(node_gene node);
			void add_connection(int innovation, int in, int out, double weight);
			void add_connection(connection_gene orgm);
			void change_weight(int innovation, double weight);
			string JSON();
			void save(char path[]); // save to a file
			void load(char path[]); // Load an genetic_encoding from a file.

			vector <double> eval(vector <double> inputs); // To implementate

		//private:
			vector <connection_gene> Lconnection_genes; //List of connections genes
			vector <node_gene> Lnode_genes;
	};

	
}

ostream & operator<<(ostream & o, ANN_USM::Genetic_Encoding & encoding);
#endif