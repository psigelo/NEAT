#ifndef DISCRETE_PROBABILITIES_CPP
#define DISCRETE_PROBABILITIES_CPP

#include "discreteProbabilities.hpp"

discreteProbabilities::discreteProbabilities(){
    sumatory=0;
}
void discreteProbabilities::addOrganismProbability(double fitness,int specie,int positionIntoSpecie){
    sumatory += fitness;
    while(specie >= (int)transformSpecieInPositionIntoSpecieList.size() ){
        transformSpecieInPositionIntoSpecieList.push_back(-1);
    }
    if(transformSpecieInPositionIntoSpecieList.at(specie) == -1){
        transformSpecieInPositionIntoSpecieList.at(specie) = listSpecies.size();
        listSpecies.push_back(specieProbabilities());
    }
    listSpecies.at(transformSpecieInPositionIntoSpecieList.at(specie)).addOrganism( organismProbabilities(fitness,specie,positionIntoSpecie) );
}

void discreteProbabilities::repositioningOrganism(organismProbabilities orgm){
    listSpecies.at(transformSpecieInPositionIntoSpecieList.at(orgm.specie)).addOrganism( organismProbabilities(orgm.fitness,orgm.specie,orgm.positionInSpecie) );
}

organismProbabilities discreteProbabilities::getFatherRandomly(){
    double random_value =  ((double)rand())/RAND_MAX;
    double acum_probability = 0.0;
    double inferiorLimit = 0.0;
    for(uint i=0; i < listSpecies.size(); i++){
        acum_probability += listSpecies.at(i).getTotal()/sumatory;
        if(random_value <= acum_probability){
            return listSpecies.at(i).getOrganismRandomly( (random_value - inferiorLimit)*sumatory );
        }
        inferiorLimit = acum_probability;
    }
    cerr << "Error::discreteProbabilities::getFatherRandomly::Probability not found \t" << acum_probability << "\trandom_value: " << random_value  << "\tlistSpecies.size(): " << listSpecies.size() << endl;
    exit(1);
}
organismProbabilities discreteProbabilities::getMotherRandomly(int fatherSpecie){
    if(listSpecies.at( transformSpecieInPositionIntoSpecieList.at(fatherSpecie) ).getTotal() > 0)
        return listSpecies.at( transformSpecieInPositionIntoSpecieList.at(fatherSpecie) ).getOrganismRandomly();
    else{
        return getMotherInterSpecieRandomly(fatherSpecie);
    }
}
organismProbabilities discreteProbabilities::getMotherInterSpecieRandomly(int fatherSpecie){
    int fatherSpecieInList =  transformSpecieInPositionIntoSpecieList.at(fatherSpecie);
    cerr << "k1" << endl;
    if(listSpecies.size()  == 1){
        cerr << "size: " << listSpecies.at(fatherSpecieInList).listOrganisms.size() << endl;
        return listSpecies.at( fatherSpecieInList ).getOrganismRandomly(); // No exist other specie then the mother is in the same specie
    }

    cerr << "k2" << endl;
    while(true){
        int randomSpecie = rand()%listSpecies.size();
        if(fatherSpecieInList != randomSpecie){
            if(listSpecies.at( randomSpecie ).getTotal() > 0 )
                return listSpecies.at( randomSpecie ).getOrganismRandomly();
        }
    }
    
}
#endif
