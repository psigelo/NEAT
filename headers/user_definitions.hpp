#ifndef UDER_DEF_HPP
#define UDER_DEF_HPP
/*
* 	USER DEFINITIONS
*/
#define POPULATION_MAX 100
#define DISTANCE_CONST_1 1.0
#define DISTANCE_CONST_2 0.4
#define DISTANCE_CONST_3 1.0
#define DISTANCE_CONST_4 1.0
#define DISTANCE_THRESHOLD 3.0

#define PERCENT_MUTATION_CONNECTION 0.25
#define PERCENTAGE_OFFSPRING_WITHOUT_CROSSOVER 25
#define PROBABILITY_INTERSPACIES_MATING 0.01
#define SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE 0.07
#define SMALLER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION 0.05
#define LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_NODE 0.01
#define LARGER_POPULATIONS_PROBABILITY_ADDING_NEW_CONNECTION 0.3
//falta la del disable
#define PROBABILITY_CONNECTION_WEIGHT_MUTATING 80
/* Lnodes.size() comparation */
#define LARGE_POPULATION_DISCRIMINATOR 10
#define GENERATIONS 100
#define SIGMOID_CONSTANT -5
#include <cmath>



double sigmoide(double x);


#endif