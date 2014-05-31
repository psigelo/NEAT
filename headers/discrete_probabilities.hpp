#ifndef DISCRETE_PROBABILITIES_HPP
#define DISCRETE_PROBABILITIES_HPP

#include <vector>
#include <stdlib.h>
#include <iostream>
using namespace std;


class Discrete_Probabilities{
    public:
        void add_element(double value);
        int obtain_uniformrandom_element();
        int take_one_randomly();
        Discrete_Probabilities();

    private:
        int id;
        double sumatory;
        vector <int> position_of_resultant_element;
        vector <double> value_of_element;
};


#endif
