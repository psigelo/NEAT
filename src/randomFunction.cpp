
#include "randomFunction.hpp"
#include <iostream>

using namespace NEAT_USM;

randomFunction::randomFunction(){
	updateRandomFunction();
}
randomFunction::randomFunction( FUNCTION _function ){
	if( _function == RANDOM ) updateRandomFunction();
	else updateRandomFunction( _function );
}
void randomFunction::updateRandomFunction(){
	updateRandomFunction( static_cast< FUNCTION >( rand() % (int)FUNCTION_SIZE ) ) ;
}
void randomFunction::updateRandomFunction( FUNCTION _function ){
	function = _function;
	switch( _function ){
		case GAUSSIAN:
			functionPointer = & randomFunction::nodeGaussian;
			stringName = "gaussian";
		break;
		case SIGMOID:
			functionPointer = & randomFunction::nodeSigmoid;
			stringName = "sigmoid";
		break;
		case SIN:
			functionPointer = & randomFunction::nodeSin;
			stringName = "sin";
		break;
		case COS:
			functionPointer = & randomFunction::nodeCos;
			stringName = "cos";
		break;
		case IDENTITY:
			functionPointer = & randomFunction::nodeIdentity;
			stringName = "identity";
		break;
		default:
			cerr << "In randomFunction.cpp:::In function randomFunction:::function not defined" << endl;
			exit(1);
	}
}
string randomFunction::getName(){
	return stringName;
}
double randomFunction::eval(double value){
	return (this->*functionPointer)(value);
}
double randomFunction::nodeGaussian(double x){
	return  0.5*((1/(2.50663*0.1))*exp(-0.5*x*x*10)) -1; 
}
double randomFunction::nodeSigmoid(double x){
	return 2*(1/(1+exp(-4.9*x))) -1;
}
double randomFunction::nodeSin(double x){ // becose the node entry is in [-1,1] and the sin domain is [-Pi, Pi].
	return sin(M_PI*x);
}
double randomFunction::nodeCos(double x){ // becose the node entry is in [-1,1] and the cos domain is [-Pi, Pi]. 
	return cos(M_PI*x);
}
double randomFunction::nodeIdentity(double x){ // becose the node entry is in [-1,1] and the cos domain is [-Pi, Pi]. 
	return (abs(x) < 1 )? x : ((x < 0)? -1: 1);
}
FUNCTION randomFunction::getFunction(){
	return function;
}