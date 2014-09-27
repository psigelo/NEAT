#ifndef DISCRETE_PROBABILITIES_HPP
#define DISCRETE_PROBABILITIES_HPP

#include <vector>
#include <stdlib.h>
#include <iostream>
using namespace std;


struct organismProbabilities{
    organismProbabilities(double _fitness,int _specie, int _positionInSpecie){
        specie = _specie;
        positionInSpecie = _positionInSpecie;
        fitness = _fitness;
    }

    int specie;
    int positionInSpecie;
    double fitness;
};

struct specieProbabilities{
    double getTotal(){
        return sumatory;
    }
    void addOrganism(organismProbabilities orgm){
        sumatory += orgm.fitness;
        listOrganisms.push_back(orgm);
    }
    organismProbabilities getOrganismRandomly(double random_value){
        random_value /= sumatory;
        double acum_probability = 0.0;
        
        for(uint i=0; i < listOrganisms.size(); i++){   
            acum_probability += listOrganisms.at(i).fitness/sumatory;
            if(random_value <= acum_probability){
                
                organismProbabilities result = listOrganisms.at(i);
                sumatory -= result.fitness;
                listOrganisms.erase(listOrganisms.begin() + i);

                return result;
            }
        }
        cerr << "random_value t1: " <<  random_value << endl;
        cerr << "acum_probability: " <<  acum_probability << "\tlistOrganisms.size(): "<< listOrganisms.size() << "\trandom_value: " << random_value << endl;
        cerr << "Error::discreteProbabilities::specieProbabilities::getOrganismRandomly::type 1:: Probability not found" << endl;
        exit(1);
    }
    organismProbabilities getOrganismRandomly(){
        if(sumatory == 0){
            cerr << "ERROR::getOrganismRandomly::sumatory is 0 and an amount of " << listOrganisms.size()  << " organisms" << endl;
            exit(1);
        }
        return getOrganismRandomly( sumatory*(rand()/(double)RAND_MAX));
    }

    vector <organismProbabilities> listOrganisms;
    double sumatory;
};

class discreteProbabilities{
    public:
        discreteProbabilities();
        void addOrganismProbability(double fitness,int specie,int positionIntoSpecie);
        void repositioningOrganism(organismProbabilities orgm);
        organismProbabilities getFatherRandomly();
        organismProbabilities getMotherRandomly(int fatherSpecie);
        organismProbabilities getMotherInterSpecieRandomly(int fatherSpecie);
    private:
        int id;
        double sumatory;
        vector <int> position_of_resultant_element;
        vector <double> value_of_element;
        vector <specieProbabilities> listSpecies;
        vector <int> transformSpecieInPositionIntoSpecieList;
};


#endif
