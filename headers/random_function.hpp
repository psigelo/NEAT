#ifndef RND_FNC_HPP
#define RND_FNC_HPP
#include <cmath>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <string>
using namespace std;



/**
* \brief The objective of the class is return a random function with the string that is represented by it.
*/
class Return_random_function{
	public:
		double (*function)(double);
		string str_name;
};

/**
* \brief this function is a gaussian that is altered to have domain [-1,1] and range [-1,1].
*/
double node_gaussian(double x);
/**
* \brief this function is a sigmoid that is altered to have domain [-1,1] and range [-1,1].
*/
double node_sigmoid(double x);
/**
* \brief this function is a sin that is altered to have domain [-1,1] and range [-1,1].
*/
double node_sin(double x);
/**
* \brief this function is a cos that is altered to have domain [-1,1] and range [-1,1].
*/
double node_cos(double x);

/**
* \brief return the random function object wich contain the function and its string.
*/
Return_random_function get_random_function();

/**
* \brief Retrieve the random function associated to a string name.
*/
Return_random_function obtain_function_fromm_name(string name);

#endif