#include "connectionGene.hpp"

using namespace NEAT_USM;

connectionGene::connectionGene(int _innovation, int _in, int _out, double _weight, bool _enable){
	innovation 	=	_innovation;
	in  		=	_in;
	out   		=	_out;
	weight 		= 	_weight;
	enable 		= 	_enable;
}

int 	connectionGene::getInnovation(){
	return innovation;
}
int 	connectionGene::getIn(){
	return in;
}
int 	connectionGene::getOut(){
	return out;
}
bool	connectionGene::getEnable(){
	return enable;
}
double 	connectionGene::getWeight(){
	return weight;
}
void connectionGene::setInnovation(int _innovation){
	innovation = _innovation;
}
void connectionGene::setIn(int _in){
	in = _in;
}
void connectionGene::setOut(int _out){
	out = _out;
}
void connectionGene::setEnable(bool _enable){
	enable=_enable;
}
void connectionGene::setWeight(double _weight){
	weight=_weight;
}