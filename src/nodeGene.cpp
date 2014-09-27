
#include "nodeGene.hpp"
#include <iostream>
using namespace NEAT_USM;

nodeGene::nodeGene( int _historicalNumber, geneType _type, int _layer, FUNCTION _function ){
	historicalNumber  = _historicalNumber;
	type  = _type;
	layer = _layer;
	function = new randomFunction( _function );
	nodeOutputValue = 0;
	innerTemporalSum = 0;
}
nodeGene::nodeGene( int _historicalNumber, geneType _type, int _layer ){
	new (this)nodeGene( _historicalNumber, _type, _layer, RANDOM ); // Constructor call another contructoer in c++11 version.
}
void nodeGene::changeRandomFunctionRandomly(){
	function->updateRandomFunction();
}
void nodeGene::setLayer( int _layer ){ 
	layer = _layer; 
}
void nodeGene::setHistoricalNumber( int _historicalNumber ){ 
	historicalNumber = _historicalNumber; 
}
void nodeGene::setType( geneType _type ){ 
	type = _type; 
}
void nodeGene::setNodeOutputValue( double _nodeOutputValue ){ 
	nodeOutputValue = _nodeOutputValue; 
}
int nodeGene::getLayer(){ 
	return layer; 
}
int nodeGene::getHistoricalNumber(){ 
	return historicalNumber; 
}
geneType nodeGene::getType(){ 
	return type; 
}
double nodeGene::getNodeOutputValue(){ 
	return nodeOutputValue; 
}
string nodeGene::getFunctionName(){
	return function->getName();
}
void nodeGene::spread(){
	nodeOutputValue = function->eval( innerTemporalSum );
	innerTemporalSum = 0;
}
void nodeGene::reset(){
	nodeOutputValue = innerTemporalSum = 0;
}
FUNCTION nodeGene::getFunction(){
	return function->getFunction();
}
void nodeGene::sumIncomingConnections(double value){
	innerTemporalSum+=value;
}
void nodeGene::setFunction(FUNCTION func){
	delete (function);
	function = new randomFunction(func);
}