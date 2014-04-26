#ifndef RND_FNC_CPP
#define RND_FNC_CPP
#include "random_function.hpp"

double node_gaussian(double x)
{
	return  0.5*((1/(2.50663*0.1))*exp(-0.5*x*x*10)) -1; 
}

double node_sigmoid(double x){
	return 2*(1/(1+exp(-4.9*x))) -1;
}

double node_sin(double x){ // becose the node entry is in [-1,1] and the sin domain is [-Pi, Pi].
	return sin(M_PI*x);
}

double node_cos(double x){ // becose the node entry is in [-1,1] and the cos domain is [-Pi, Pi]. 
	return cos(M_PI*x);
}


Return_random_function Random_function::get_random_function(){
	int i = rand()%(int)functions.size();
	return functions[i];
}


Random_function::Random_function(){
	Return_random_function rrf;
	rrf.function = node_gaussian;
	rrf.str = "gaussian";
	functions.push_back(rrf);
	rrf.function = node_sigmoid;
	rrf.str = "sigmoid";
	functions.push_back(rrf);
	rrf.function = node_sin;
	rrf.str = "sin";
	functions.push_back(rrf);
	rrf.function = node_cos;
	rrf.str = "cos";
	functions.push_back(rrf);
}

#endif