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
		string str;
};

/**
* \brief The objective of the class is return a random function with the string that is represented by it.
*/
class Random_function{
	public:
		/**
		* \brief set all random functions and their strings.
		*/
		Random_function();
		/*
		* \brief  the return is a class that contain a pointer to the function and a string.
		*/
		Return_random_function get_random_function();
		vector <Return_random_function > functions;
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


#endif