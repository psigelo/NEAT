#ifndef T_G_E_CPP
#define T_G_E_CPP

#include "genetic_encoding.hpp"
#include <ctime>

using namespace ANN_USM;

int main(){

	std::srand(std::time(0)); // use current time as seed for random generator
	Genetic_Encoding * encoding = new Genetic_Encoding();
	
	encoding->add_node(0,INPUT);
	encoding->add_node(3,HIDDEN);
	encoding->add_node(1,INPUT);
	encoding->add_node(2,OUTPUT);
	

	encoding->add_connection(0,0,2,(rand()%1000)/1000.0);
	encoding->add_connection(1,1,2,(rand()%1000)/1000.0);
	encoding->add_connection(4,0,2,(rand()%1000)/1000.0);
	encoding->add_connection(2,3,2,(rand()%1000)/1000.0);
	encoding->add_connection(3,1,3,(rand()%1000)/1000.0);
	encoding->add_connection(9,0,3,(rand()%1000)/1000.0);

	cout << *encoding << endl;

}

#endif