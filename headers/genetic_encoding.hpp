#ifndef GENETIC_ENCODING_hPP
#define GENETIC_ENCODING_hPP


#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <strings.h>

#include "function.hpp"

using namespace std;

namespace ANN_USM
{
	enum gene_type
	{
		INPUT,
		HIDDEN,
		OUTPUT
	};

	/**********************************************************************************************************************
		Connection
	***********************************************************************************************************************/

	class connection_gene
	{
		public:

			void c_g(int innovation, int in, int out, double weight, bool enable); // fill a conection gene
			void c_g(bool exist);
			
			int innovation;
			int in;
			int out;

			double weight;

			bool enable;
			bool exist; 
	};

	/**********************************************************************************************************************
		Node
	***********************************************************************************************************************/

	class node_gene
	{
		public:

			node_gene();

			void n_g(int node, gene_type type, string function); // fill a node gene
			void n_g(int node, gene_type type);
			void n_g(bool exist);
			void increase_incoming_connection();
			void eval(double value);
			void increase_accumulative_result(double value);

			bool is_ready();

			int incoming_connections;
			int counter;
			int row;
			int node;

			bool exist;

			double accumulative_result;
			double final_result;
			
			gene_type type;

			Function * function;
	};

	/**********************************************************************************************************************
		CPPN
	***********************************************************************************************************************/

	class Genetic_Encoding
	{
		public:

			int niche;

			void add_node(int node, int row, gene_type type);
			void add_node(int node, int row, gene_type type, int function);
			void add_node(node_gene node);
			void add_connection(int innovation, int in, int out, double weight);
			void add_connection(connection_gene orgm);
			void change_weight(int innovation, double weight);
			void save(char path[]); // save to a file
			void load(char path[]); // Load an genetic_encoding from a file.

			string JSON();

			vector <double> eval(vector <double> inputs);

			vector <connection_gene> Lconnection_genes; //List of connections genes
			vector <node_gene> Lnode_genes;

			void spread_final_result(int node, double value);

			vector<connection_gene> get_outgoing_connections(int node);

			vector<int> input_nodes;			// Input nodes
			vector<int> output_nodes;			// Output nodes
	};
}

ostream & operator<<(ostream & o, ANN_USM::Genetic_Encoding & encoding);

#endif
