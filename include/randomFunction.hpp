#ifndef RANDOMFUNCTION_HPP
#define RANDOMFUNCTION_HPP

#include <cmath>
#include <string>

using namespace std;

namespace NEAT_USM {

enum FUNCTION { IDENTITY, SIGMOID, GAUSSIAN, COS, SIN, FUNCTION_SIZE, RANDOM };

/**
* \brief The objective of the class is return a random function with the string that is represented by it.
*/
class randomFunction{

	public:

		/**
		* \brief Generate a random function
		*/
		randomFunction();
		/**
		* \brief Generate a specific function
		*/
		randomFunction( FUNCTION _function );
		/**
		* \brief Get function name
		*/
		string getName();
		/**
		* \brief Get function
		*/
		FUNCTION getFunction();
		/**
		* \brief Change the current function to a new one randomly
		*/
		void updateRandomFunction();
		void updateRandomFunction( FUNCTION _function );
		/**
		* \brief Evaluate the random function
		*/
		double eval(double value);

	private:

		string stringName;
		FUNCTION function;
		double (randomFunction::*functionPointer)(double x);		
		double nodeGaussian(double x);		
		double nodeSigmoid(double x);		
		double nodeSin(double x);
		double nodeCos(double x);
		double nodeIdentity(double x);
};

}

#endif