#ifndef GENETIC_ENCODING_hPP
#define GENETIC_ENCODING_hPP


#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <strings.h>
#include <stdio.h>
#include <cstring>
#include <cmath>
#include "random_function.hpp"

using namespace std;

namespace ANN_USM{
	enum gene_type{
		INPUT,
		HIDDEN,
		OUTPUT
	};

	class connection_gene{
		public:
			/**
			* \brief Change or set the value to the variables to innovation, in, out, weight, enable.
			* \param innovation: the innovation of the genne_connection.
			* \param in: is the input node.
			* \param out: is the output node.
			* \param weight: is the weight of the connection.
			* \param enable: if this connection is present in the genome.
			*/
			void _connection_gene(
				int innovation,
				int in, int out,
				double weight,
				bool enable
				); // fill a conection gene
			/**
			* \brief Change the value of the exist variable
			* \param exist: if this connection is present in the genome.
			*/
			void _connection_gene(bool exist);
			int 	innovation;
			int 	in;
			int 	out;
			bool	enable;
			double 	weight;
			bool 	exist;
	};




	class node_gene{
		public:
			/**
			* \brief Change or set the values of node id and type of node
			* \param node: set the hystorical node value.
			* \param type: 0 if is an input, 1 if is an hiden and 2 if is an output node.
			* \param row: The row of the node.
			* \param random_function the random function with its string.
			*/
			void _node_genne( // fill a node gene
				int node,
				gene_type type,
				int row,
				Return_random_function random_function
				);
			/**
			* \brief Changes the value of the exist variable.
			* \param exist: if this node is present in the genome.
			*/
			void _node_genne(bool exist);

			/**
			* \brief Changes the random function of the node with another random function randomly (even may be the same)
			*/
			void change_random_function_randomly();

			/**
			*	\brief Return the past output.
			*/
			double obtain_past_value();

			int 		row;
			bool 		exist;
			int 		node;
			gene_type 	type;
			double 		node_output_value;
			string 		str;
			double		output_t_minus_1;
			Return_random_function random_function;
	};

	class Genetic_Encoding{
		public:
			/**
			* \brief add a node to the genome
			* \param node: Historical node value
			* \param row: in wich row of the genome is.
			* \param type: which type of node is; 0 if is an input, 1 if is an hiden and 2 if is an output node.
			*/
			void add_node(
				int node,
				int row,
				gene_type type
				);
			/**
			* \brief add a node to the genome.
			* \param node: an node that already exist.
			*/
			void add_node(node_gene node);
			/**
			* \brief add a connection to the genome.
			* \param innovation: the innovation of the genne_connection.
			* \param in: is the input node
			* \param out: is the output node
			* \param weight: is the weight of the connection.
			* \param enable: if this connection is present in the genome
			*/
			void add_connection(
				int innovation,
				int in,
				int out,
				double weight
				);
			/**
			* \brief add a node to the genome.
			* \param node: an node that already exist.
			*/
			void 				add_connection(connection_gene orgm);

			/**
			* \brief store the genome in file in json format.
			* \param path: Is where the file is stored.
			*/
			void 				save(char path[]); // save to a file
			/**
			* \brief load  a genome from file and set this organism variables.
			* \param path: Is where the file is stored.
			*/
			void 				load(char path[]); // Load an genetic_encoding from a file.
			/**
			* /brief return a string with the entire genome write like JSON format.
			*/
			string 				JSON();

			vector <double> 	eval(vector <double> inputs); // To implementate
			string 				ANN_function();


			/**
			\brief return the number of outputs of the NEAT
			*/
			int 				getNEATOutputSize();

			/**
			\brief Put to 0 the past value of all nodes.
			*/
			void 				reset_past_values();

			vector <connection_gene> 	Lconnection_genes; //List of connections genes
			vector <node_gene> 			Lnode_genes;
			double 						fitness;
			double 						shared_fitness;
			int 						niche;
			vector < int > 				row_orderer_list;
			vector < vector <int> >		nodes_at_row;
			vector < int > 				outputs_positions;
	};
}
ostream & operator<<(ostream & o, ANN_USM::Genetic_Encoding & encoding);


#endif
