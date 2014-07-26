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

double identity(double x){
	return x;
}

static int RANDOM_f_iterator = 0;
static vector <Return_random_function> RANDOM_functions;

void inicialization(){
	if(RANDOM_f_iterator == 0){
		
		Return_random_function rrf;
		
		rrf.function = node_gaussian;
		rrf.str_name = "gaussian";
		RANDOM_functions.push_back(rrf);
		
		rrf.function = node_sigmoid;
		rrf.str_name = "sigmoid";
		RANDOM_functions.push_back(rrf);
		
		rrf.function = node_sin;
		rrf.str_name = "sin";
		RANDOM_functions.push_back(rrf);
		
		rrf.function = node_cos;
		rrf.str_name = "cos";
		RANDOM_functions.push_back(rrf);

		rrf.function = identity;
		rrf.str_name = "identity";
		RANDOM_functions.push_back(rrf);

		RANDOM_f_iterator = 1;
	}
}


Return_random_function get_random_function(){
	inicialization();
	int i = rand()%(int)RANDOM_functions.size();
	return RANDOM_functions[i];
}

Return_random_function obtain_function_fromm_name(string name){
	inicialization();
	for (int i = 0; i < (int)RANDOM_functions.size(); ++i)
	{
		if(name.compare(RANDOM_functions[i].str_name) == 0 ){
			return RANDOM_functions[i];
		}
	}
	cerr << "In random_function.cpp:::In function obtain_funcion_of_string:::incorrect node compare" << endl;
	exit(1);
}



#endif