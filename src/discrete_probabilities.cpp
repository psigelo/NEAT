#ifndef DISCRETE_PROBABILITIES_CPP
#define DISCRETE_PROBABILITIES_CPP

#include "discrete_probabilities.hpp"

    Discrete_Probabilities::Discrete_Probabilities(){
        sumatory=0;
        id=0;
    }

    void  Discrete_Probabilities::add_element(double value){
        sumatory+=value;
        position_of_resultant_element.push_back(id++);
        value_of_element.push_back(value);
    }


    // retorna aleatoriamente pero sin tomar en cuenta el valor probabilistico de ningún elemento.
    int Discrete_Probabilities::take_one_randomly(){
        int random_value =  rand()%position_of_resultant_element.size();
        int returned_value = position_of_resultant_element.at(random_value);
        int limit_for = position_of_resultant_element.size();
        vector <int> position_of_resultant_element_temp;
        for(int i = 0; i < limit_for ; i++)
        {
            if(i != random_value)
                position_of_resultant_element_temp.push_back(position_of_resultant_element.at(i));
        }
        position_of_resultant_element = position_of_resultant_element_temp;
        sumatory -= value_of_element.at(returned_value);
        return (returned_value);
    }

    int Discrete_Probabilities::obtain_uniformrandom_element(){
        double random_value =  ((double)rand())/RAND_MAX;
        double acum_probability = 0.0;

            for(int i=0; i < (int)position_of_resultant_element.size(); i++){
                acum_probability += value_of_element.at(position_of_resultant_element.at(i))/sumatory;
                if(random_value <= acum_probability){
                    return position_of_resultant_element.at(i);
                }
            }

            cerr << "Error::Discrete_Probabilities::obtain_uniformrandom_element:: Probability not found" << endl;
            exit(1);
    }


#endif
