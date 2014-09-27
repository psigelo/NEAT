#ifndef NODEGENE_HPP
#define NODEGENE_HPP

#include "randomFunction.hpp"

namespace NEAT_USM {

enum geneType { INPUT, HIDDEN, OUTPUT };

class nodeGene {

	public:
		/**
		* \param _node Set the historical node value.
		* \param _type Node type. 0 for input, 1 for hidden and 2 for output.
		* \param _row The row of the node.
		*/
		nodeGene( int _historicalNumber, geneType _type, int _layer );
		/**
		* \param _node Set the historical node value.
		* \param _type Node type. 0 for input, 1 for hidden and 2 for output.
		* \param _row The row of the node.
		* \param _function Function of the node.
		*/
		nodeGene( int _historicalNumber, geneType _type, int _layer, FUNCTION _function );
		/**
		* \brief Changes the random function of the node with another random function randomly
		*/
		void changeRandomFunctionRandomly();
		/**
		* \brief Set the layer in which the node will be
		* \param _layer is the layer (which can be seen as a row, a column or level) in which the node will be positioned
		*/
		void setLayer( int _layer );
		/**
		* \brief Set the historical node identifier
		* \param _node is the node to be modified
		*/
		void setHistoricalNumber( int _node );
		/**
		* \brief Set the node type
		* \param _type can be either INPUT, HIDDEN or OUTPUT
		*/
		void setType( geneType _type );
		/**
		* \brief When all the previous nodes to this one have sent their output value, the current node calculates its output value with the current function.
		* \param _nodeOutputValue is the new output value of the node
		* \warning This function must not being used at least you are extremely sure of what you are doing
		*/
		void setNodeOutputValue( double _nodeOutputValue );
		/**
		* \brief Get the layer in which the node is positioned
		* \return the layer position
		*/
		int getLayer();
		/**
		* \brief Get the historical node
		* \return the historical node
		*/
		int getHistoricalNumber();
		/**
		* \brief Get the node type
		* \return the node type
		*/
		geneType getType();
		/**
		* \brief When all the previous nodes to this one have sent their output value, the current node calculates its output value with the current function.
		* \return the output value of the node
		*/
		double getNodeOutputValue();
		/**
		* \brief Get the function name
		* \return the function name
		*/
		string getFunctionName();
		/**
		* \brief Evaluates the current sum of values from the previous nodes. Then the nodes of an upper layer could use this value to evaluate its own sum of values, and so on.
		*/
		void spread();
		/**
		* \brief reset the current acumulate values
		*/
		void reset();
		/**
		* \brief get the function represented by an interger.
		*/
		FUNCTION getFunction();

		/**
		* \brief Sum an incoming conections 
		* \param value value of incoming connection
		*/
		void sumIncomingConnections(double value);

		/**
		*	\brief Set the function of the node.	 
		*/
		void setFunction(FUNCTION func);

	private:

		int 			layer;
		int 			historicalNumber;
		geneType 		type;
		double 			nodeOutputValue;
		double			innerTemporalSum;
		randomFunction 	* function;
};

}

#endif // NODEGENE_HPP