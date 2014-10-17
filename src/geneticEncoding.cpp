#include "geneticEncoding.hpp"


using namespace NEAT_USM;

//Constructor copy
// Is used to copy organisms.
geneticEncoding::geneticEncoding(const geneticEncoding & genetic_encoding){
	this->listNodeGenes = genetic_encoding.listNodeGenes;
	this->listConnectionGenes = genetic_encoding.listConnectionGenes;
	this->connectionsThatCanMutateNodes = genetic_encoding.connectionsThatCanMutateNodes;
	this->transformHistoricalNodeToListNodeGenes = genetic_encoding.transformHistoricalNodeToListNodeGenes;
	this->transformInnovationToListConnectionGenes = genetic_encoding.transformInnovationToListConnectionGenes;
	this->connectionsThatCanBeMutated=genetic_encoding.connectionsThatCanBeMutated;
	this->listNodesAtLayer = genetic_encoding.listNodesAtLayer;
	this->listConnectionTowardSpecificLayer = genetic_encoding.listConnectionTowardSpecificLayer;
	this->fitness = genetic_encoding.fitness;
	this->sharedFitness = genetic_encoding.sharedFitness;
	this->amountInputs = amountInputs;
	this->amountOutputs = amountOutputs;
	this->amountHidden = amountHidden;
	this->layerOrdererList = genetic_encoding.layerOrdererList;
	this->input_pointer = genetic_encoding.input_pointer;
	this->output_pointer = genetic_encoding.output_pointer;
	this->specie = -1; // no have specie defined
	this->presentLayers = genetic_encoding.presentLayers;
	this->NEATinformation = genetic_encoding.NEATinformation;
	year = 0;
	for(uint i=0; i< this->listNodeGenes.size();i++){
		this->listNodeGenes.at(i).reset();
	}
}


geneticEncoding::geneticEncoding(char path[], globalInformationOfNEAT * NEATinformation)
{
	this->NEATinformation = NEATinformation;
	load(path);
}


geneticEncoding::geneticEncoding(){}
/*
procedure:
    if the historical value of the node is greater than the greater historical node
	then is created all nodes that are between both with the variable exist equal
	to 0 and then is created the node in its historical node position.

	in other case the node is present in the genome and only are changed its values.
*/
void geneticEncoding::addNode(nodeGene _node){

	listNodeGenes.push_back(_node);
	
	while((int)transformHistoricalNodeToListNodeGenes.size() <= _node.getHistoricalNumber()){
		transformHistoricalNodeToListNodeGenes.push_back(-1);
	}
	transformHistoricalNodeToListNodeGenes.at(_node.getHistoricalNumber()) = (int)listNodeGenes.size()-1;
	if(_node.getType() == INPUT){
		input_pointer.push_back(listNodeGenes.size()-1);
	}
	else if(_node.getType() == OUTPUT){
		output_pointer.push_back(listNodeGenes.size()-1);
	}
	// For eval, we need listNodesAtLayer completed.
	// thus we can optimize time in eval method.
	while((int)listNodesAtLayer.size() <=  _node.getLayer()){
		listNodesAtLayer.push_back(vector <int>());
	}
	listNodesAtLayer.at(_node.getLayer()).push_back(_node.getHistoricalNumber());

	//For mutation connections:
	//If a node is created then 2*|nodes| is the amount of posibly new connections to mutate (because is with directions forward and backward)
	int superiorLimit = (int)listNodeGenes.size()-1;
	for (int i = 0; i < superiorLimit; ++i)
	{		
		connectionsThatCanBeMutated.push_back({listNodeGenes.at(i).getHistoricalNumber() , listNodeGenes.at(superiorLimit).getHistoricalNumber()});
		connectionsThatCanBeMutated.push_back({listNodeGenes.at(superiorLimit).getHistoricalNumber(), listNodeGenes.at(i).getHistoricalNumber()});
	}
	connectionsThatCanBeMutated.push_back({listNodeGenes.at(superiorLimit).getHistoricalNumber(), listNodeGenes.at(superiorLimit).getHistoricalNumber()	});
}
/*
procedure:
    if the innovation value of the connection is greater than the greater innovation value
	then is created all connection that are between both with the variable exist equal
	to 0 and then is created the connection in its innovation position.

	in other case the connection is present in the genome and only is changed its values.
*/
void geneticEncoding::addConnection(connectionGene _connection){
	listConnectionGenes.push_back(_connection);
	while((int)transformInnovationToListConnectionGenes.size() <= _connection.getInnovation()){
		transformInnovationToListConnectionGenes.push_back(-1);
	}
	transformInnovationToListConnectionGenes.at(_connection.getInnovation()) = listConnectionGenes.size()-1;
	// For eval, we need listConnectionTowardSpecificLayer completed.
	// thus we can optimize time in eval method.
	while((int)listConnectionTowardSpecificLayer.size() <= listNodeGenes.at( transformHistoricalNodeToListNodeGenes.at( _connection.getOut() ) ).getLayer() ){
		listConnectionTowardSpecificLayer.push_back(vector <int>());
	}
	
	listConnectionTowardSpecificLayer.at( listNodeGenes.at( transformHistoricalNodeToListNodeGenes.at( _connection.getOut()) ).getLayer() ).push_back(_connection.getInnovation());
	//For mutationNode
	//When a connection is created is an candidate for a new node mutation
	connectionsThatCanMutateNodes.push_back( listConnectionGenes.at(listConnectionGenes.size()-1).getInnovation() );
}

void geneticEncoding::save(char path[]){
	ofstream file;
	file.open (path);
	file << *this;
	file.close();
}


void geneticEncoding::load(char path[]){
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	//int new_node_number(0);
	vector < int > old_to_new_node_number;  // old_to_new_node_number.at( old_number ) = new_number
	vector <int> & transformLayerInPositionOfLayer = NEATinformation->getTransformLayerInPositionOfLayerReference();
	vector <int> & globalLayerOrderer = NEATinformation->getGlobalLayerOrdererReference();

	fp = fopen(path, "r");
	if (fp == NULL){
		cerr << "ERROR::geneticEncoding::load::File not founded" << endl;
		exit(EXIT_FAILURE);
	}

	while ((read = getline(&line, &len, fp)) != -1) {
		char * pch;
		pch = strtok (line,"\t");
		
			if(*pch == 'l')
			{
				pch = strtok (NULL,"\t");
				while(pch != NULL){
					
					int layer = atoi(pch);
					layerOrdererList.push_back(layer);
					globalLayerOrderer.push_back(layer);

					while(transformLayerInPositionOfLayer.size() <= (uint)layer)
						transformLayerInPositionOfLayer.push_back({-1});
					transformLayerInPositionOfLayer.at(layer) = layerOrdererList.size() - 1;

					while( (int)presentLayers.size() <= layer)
						presentLayers.push_back(-1);
					presentLayers.at(layer)=1;

					pch = strtok (NULL,"\t");
				}
				
			}
			else if(*pch == 'n')
			{
				int historicalNumber = NEATinformation->newNodeNumber();
				pch = strtok(NULL,"\t");
				int old_historicalNumber = atoi(pch);
				while( (int)old_to_new_node_number.size() <= old_historicalNumber){
					old_to_new_node_number.push_back(-1);
				}
				old_to_new_node_number.at(old_historicalNumber) = historicalNumber;
				pch = strtok(NULL,"\t");
				int type = atoi(pch);
				pch = strtok(NULL,"\t");
				int layer = atoi(pch);
				pch = strtok(NULL,"\t");
				int function = atoi(pch);
				addNode( nodeGene(historicalNumber,static_cast<geneType> (type),layer,static_cast<FUNCTION> (function) ) );
			}
			else if(*pch == 'c')
			{ 
				pch = strtok(NULL,"\t");
				int innovation = NEATinformation->newInnovation();
				pch = strtok(NULL,"\t");
				int in = old_to_new_node_number.at(atoi(pch));
				pch = strtok(NULL,"\t");
				int out = old_to_new_node_number.at(atoi(pch));
				pch = strtok(NULL,"\t");
				double weight = atof(pch);
				pch = strtok(NULL,"\t");
				bool enable = atoi(pch);
				addConnection(connectionGene(innovation,in,out,weight,enable) );
				for (uint i = 0; i < connectionsThatCanBeMutated.size(); ++i)
					if(connectionsThatCanBeMutated.at(i).at(0) == in && connectionsThatCanBeMutated.at(i).at(1) == out ){
						connectionsThatCanBeMutated.erase(connectionsThatCanBeMutated.begin() + i);
						break;
					}
			}
			else
			{
				cerr << "ERROR::geneticEncoding::load()::Error on load path." << pch << endl;
				exit(1);
			}
		
	}
	free(line);
}

vector <double> geneticEncoding::eval(std::vector<double> inputs){
	if(inputs.size() != input_pointer.size()) {cerr << "Error::geneticEncoding::eval::inputs.size is not correct"<< endl;exit(1);}
	vector <double> result;
	for (uint i = 0; i < input_pointer.size(); ++i)
		listNodeGenes.at(input_pointer.at(i)).sumIncomingConnections(inputs.at(i));

	for (uint i = 0; i < layerOrdererList.size(); ++i)
	{	
		for (uint j = 0; j < listConnectionTowardSpecificLayer.at( layerOrdererList.at(i) ).size(); ++j){
			int connection = listConnectionTowardSpecificLayer.at(layerOrdererList.at(i)).at(j);
			int connectionPosition = transformInnovationToListConnectionGenes.at(connection);
			if( listConnectionGenes.at( connectionPosition ).getEnable() ){
				int out = transformHistoricalNodeToListNodeGenes.at(listConnectionGenes.at(connectionPosition).getOut());
				int in = transformHistoricalNodeToListNodeGenes.at(listConnectionGenes.at(connectionPosition).getIn());
				listNodeGenes.at( out ).sumIncomingConnections(listNodeGenes.at(in).getNodeOutputValue()*listConnectionGenes.at(connectionPosition).getWeight());
				//listNodeGenes.at(listConnectionGenes.at(connectionPosition).getOut()).sumIncomingConnections(listNodeGenes.at(listConnectionGenes.at(listConnectionTowardSpecificLayer.at(layerOrdererList.at(i)).at(j)).getIn()).getNodeOutputValue() * listConnectionGenes.at(listConnectionTowardSpecificLayer.at(layerOrdererList.at(i)).at(j)).getWeight());
			}
		}
		
		for(uint j=0; j< listNodesAtLayer.at(layerOrdererList.at(i)).size(); ++j){
			listNodeGenes.at( transformHistoricalNodeToListNodeGenes.at( listNodesAtLayer.at(layerOrdererList.at(i)).at(j) ) ).spread();		
		}
	}
	
	for (uint i = 0; i < output_pointer.size(); ++i)
		result.push_back( listNodeGenes.at(output_pointer.at(i)).getNodeOutputValue() );	
	return result;
}


void geneticEncoding::putRandomsWeight(){
	for (uint i = 0; i < listConnectionGenes.size(); ++i)
		listConnectionGenes.at(i).setWeight(2*(rand()/(double)RAND_MAX) -1);
}


void geneticEncoding::mutationChangeWeight(double probabilityChangeWeight){
	double delta;
	for(uint i = 0; i < listConnectionGenes.size(); i++ )
		if( rand()/(double)RAND_MAX <= probabilityChangeWeight ){
			delta = 2*(rand()%RAND_MAX)/(double)RAND_MAX-1;
			listConnectionGenes.at(i).setWeight( delta*0.2 + 0.8*listConnectionGenes.at(i).getWeight());
		}
}
// Can be added a new node to the genome.
// Population manage the historicalNumbers of all population
void geneticEncoding::mutateNode(){
	// Steps: 1) Find stochaticatlly the connection to be mutated.
	// 		  2) Obtain the historical node value and its layer.
	//		  3) Obtain innovation of connection to create.
	if (connectionsThatCanMutateNodes.size()==0)return;
	int random = rand()%connectionsThatCanMutateNodes.size();
	int connectionToMutateInListConnectionGenes =  transformInnovationToListConnectionGenes.at(connectionsThatCanMutateNodes.at(random));
	listConnectionGenes.at(connectionToMutateInListConnectionGenes).setEnable(false);
	int in 	= listConnectionGenes.at(connectionToMutateInListConnectionGenes).getIn();
	int out = listConnectionGenes.at(connectionToMutateInListConnectionGenes).getOut();
	int layer_in = listNodeGenes.at( transformHistoricalNodeToListNodeGenes.at(in) ).getLayer();
	int layer_out = listNodeGenes.at(transformHistoricalNodeToListNodeGenes.at(out)).getLayer();


	int historicalNumber = obtainHistoricalNodeNumber(
													in,
													out
													);
	int layer = obtainLayer(
							historicalNumber,
							layer_in,
							layer_out
							);
	addNode( nodeGene(historicalNumber, HIDDEN, layer , RANDOM) );
	int innovation1 = obtainInnovation(in,historicalNumber);
	int innovation2 = obtainInnovation(historicalNumber, out);
	addConnection( connectionGene(innovation1, in, historicalNumber, listConnectionGenes.at(connectionToMutateInListConnectionGenes).getWeight(),1 ) );
	addConnection( connectionGene(innovation2, historicalNumber, out, 1.0,1 ) );
	//Segun yo las siguientes lineas no deberian ir porque estan en addConnection
	//connectionsThatCanMutateNodes.push_back(listConnectionGenes.size() -1 );
	//connectionsThatCanMutateNodes.push_back(listConnectionGenes.size() -2 );
	connectionsThatCanMutateNodes.erase(connectionsThatCanMutateNodes.begin()+random);
}


void geneticEncoding::mutateConnection(){
	if(connectionsThatCanBeMutated.size() == 0) return;
	int random = rand()%connectionsThatCanBeMutated.size();
	vector < int > connectionToCreate = connectionsThatCanBeMutated.at(random); // connectionToCreate.at(1) = hitorical_number_of_initial_in_node  connectionToCreate.at(1) = hitorical_number_of_initial_out_node
	int innovation = obtainInnovation( connectionToCreate.at(0), connectionToCreate.at(1) );
	addConnection( connectionGene(innovation, connectionToCreate.at(0), connectionToCreate.at(1), 2.0*(rand()%RAND_MAX)/(double)RAND_MAX - 1.0 ,1) );
	connectionsThatCanBeMutated.erase(connectionsThatCanBeMutated.begin() + random);
}


int geneticEncoding::obtainHistoricalNodeNumber(int initial_in, int initial_out){
	vector < vector <int> > & historicalNodeNumbers = NEATinformation->getHistoricalNodeNumberReference();
	try{
		if(historicalNodeNumbers.at(initial_in).at(initial_out) >= 0 )
			return historicalNodeNumbers.at(initial_in).at(initial_out);
		else
			historicalNodeNumbers.at(initial_in).at(initial_out) = NEATinformation->newNodeNumber();
	}
	catch(...){
		while((int)historicalNodeNumbers.size()-1 < initial_in)
		{
			vector <int> temp;
			historicalNodeNumbers.push_back(temp);
		}
		while((int)historicalNodeNumbers.at(initial_in).size()-1 < initial_out)
			historicalNodeNumbers.at(initial_in).push_back(-1);
		if(historicalNodeNumbers.at(initial_in).at(initial_out) < 0)
			historicalNodeNumbers.at(initial_in).at(initial_out) = NEATinformation->newNodeNumber();
	}

	return historicalNodeNumbers.at(initial_in).at(initial_out);
}

int geneticEncoding::obtainInnovation(int initial_in, int initial_out){
	vector < vector <int> > & historicalInnovation = NEATinformation->getHistoricalInnovationReference();
	try{
		if(historicalInnovation.at(initial_in).at(initial_out) >= 0)
			return historicalInnovation.at(initial_in).at(initial_out);
		else
			historicalInnovation.at(initial_in).at(initial_out) = NEATinformation->newInnovation();
	}
	catch(...){
		while((int)historicalInnovation.size()-1 < initial_in)
		{
			vector <int> temp;
			historicalInnovation.push_back(temp);
		}
		while((int)historicalInnovation.at(initial_in).size()-1 < initial_out)
			historicalInnovation.at(initial_in).push_back(-1);
		
		if(historicalInnovation.at(initial_in).at(initial_out) < 0)
			historicalInnovation.at(initial_in).at(initial_out) = NEATinformation->newInnovation();
	}
	return historicalInnovation.at(initial_in).at(initial_out);
}



int geneticEncoding::obtainLayer(int historicalNodeNumber, int layerNodeInitialIn, int layerNodeInitialOut){
	cerr << "obtainLayer 1" << endl;
	vector <int> & historicalLayer = NEATinformation->getHistoricalLayerReference();
	try{
		if(historicalLayer.at(historicalNodeNumber) >= 0) 
			return historicalLayer.at(historicalNodeNumber);
	}
	catch (...){}

	vector <int> & transformLayerInPositionOfLayer = NEATinformation->getTransformLayerInPositionOfLayerReference();
	vector <int> & globalLayerOrderer = NEATinformation->getGlobalLayerOrdererReference();
	int rowPositionIn = transformLayerInPositionOfLayer.at(layerNodeInitialIn);
	int rowPositionOut = transformLayerInPositionOfLayer.at(layerNodeInitialOut);
	int position = ((rowPositionOut > rowPositionIn) ? rowPositionIn + 1: rowPositionOut + 1);

	while((int)historicalLayer.size()-1 < historicalNodeNumber)
		historicalLayer.push_back(-1);
	if(rowPositionIn == rowPositionOut ){
		historicalLayer.at(historicalNodeNumber) = layerNodeInitialIn;
		return layerNodeInitialIn;
	}
	if(abs(rowPositionOut - rowPositionIn) == 1 ){
		historicalLayer.at(historicalNodeNumber) = globalLayerOrderer.size();
		globalLayerOrderer.insert(globalLayerOrderer.begin() + position, globalLayerOrderer.size());
		transformLayerInPositionOfLayer.push_back(position);
		for (uint i = 0; i < transformLayerInPositionOfLayer.size()-1; ++i)
			if(transformLayerInPositionOfLayer.at(i) >= position ){transformLayerInPositionOfLayer.at(i)++;}
	}
	else{
		historicalLayer.at(historicalNodeNumber) = globalLayerOrderer.at(position);
	}
	
	while((int)presentLayers.size() <= historicalLayer.at(historicalNodeNumber))
		presentLayers.push_back(-1);


	if(presentLayers.at(historicalLayer.at(historicalNodeNumber)) == -1){
		presentLayers.at(historicalLayer.at(historicalNodeNumber)) = 1;
		int inferior;
		int position_inf;
		
		if(rowPositionIn > rowPositionOut){
			inferior = layerNodeInitialIn;
		}else{
			inferior = layerNodeInitialOut;
		}

		for (int i = 0; i < (int)layerOrdererList.size(); ++i)
		{
			if(layerOrdererList.at(i) == inferior){
				position_inf = i;
				break;
			}
		}
	
		this->printHumanReadable();
		layerOrdererList.insert(layerOrdererList.begin() + position_inf, globalLayerOrderer.at(position));
		

	}	

	return historicalLayer.at(historicalNodeNumber);
}


void geneticEncoding::changeConnection(connectionGene & conn){
    int position =	transformInnovationToListConnectionGenes.at( conn.getInnovation() );
    listConnectionGenes.at(position).setWeight( conn.getWeight() );
    listConnectionGenes.at(position).setEnable( conn.getEnable() );
}
void geneticEncoding::changeNode(nodeGene & node){
	int position = transformHistoricalNodeToListNodeGenes.at( node.getHistoricalNumber() );
	listNodeGenes.at(position).setFunction(node.getFunction());
}
int geneticEncoding::getSpecie(){
	return specie;
}
void geneticEncoding::setSpecie(int niche){
	this->specie = specie;
}

double geneticEncoding::getFitness(){
	return fitness;
}
void   geneticEncoding::setFitness(double fitness){
	this->fitness = fitness;
}
double geneticEncoding::getSharedFitness(){
	return sharedFitness;
}
void   geneticEncoding::setSharedFitness(double x){
	sharedFitness=x;
}
void geneticEncoding::mutateFunction(){
	int random = rand()%listNodeGenes.size(); 
	listNodeGenes.at(random).setFunction(RANDOM);
}
void geneticEncoding::setFatherSpecie(int fatherSpecie){
	this->fatherSpecie = fatherSpecie;
}
int geneticEncoding::getFatherSpecie(){
	return fatherSpecie;
}
void geneticEncoding::setMotherSpecie(int motherSpecie){
	this->motherSpecie = motherSpecie;
}
int geneticEncoding::getMotherSpecie(){
	return motherSpecie;
}
int 	geneticEncoding::getYear(){
	return year;
}
void 	geneticEncoding::setYear(int year){
	this->year=year;
}
void 	geneticEncoding::incrementYear(){
	year++;
}








namespace NEAT_USM{
	// Crossover
	geneticEncoding * operator * (geneticEncoding & encoding1, geneticEncoding & encoding2){


		geneticEncoding * resultOrganism = new geneticEncoding(encoding1); // The copi constructor is called.
		resultOrganism->setSpecie(-1);
		resultOrganism->setFatherSpecie(encoding1.getSpecie());
		resultOrganism->setMotherSpecie(encoding2.getSpecie());
		//Layers:
		vector <int > & globalLayerOrderer = resultOrganism->NEATinformation->getGlobalLayerOrdererReference();

		resultOrganism->layerOrdererList.clear();

		for (uint i = 0; i < globalLayerOrderer.size(); ++i)
		{
			int layer = globalLayerOrderer.at(i);
			if((int)resultOrganism->presentLayers.size() > layer && (int)encoding2.presentLayers.size() > layer ){
				if(resultOrganism->presentLayers.at(layer) || encoding2.presentLayers.at(layer))
					resultOrganism->layerOrdererList.push_back(layer);	
			}
			else if((int)resultOrganism->presentLayers.size() > layer){
				if(resultOrganism->presentLayers.at(layer))
					resultOrganism->layerOrdererList.push_back(layer);	
			}
			else if((int)encoding2.presentLayers.size() > layer){
				if(encoding2.presentLayers.at(layer))
					resultOrganism->layerOrdererList.push_back(layer);	
			}
		}

		// Nodes: 
		uint inferiorLimit(0),superiorLimit(0); 
		bool isSuperiorResultOrganism;
		if( resultOrganism->transformHistoricalNodeToListNodeGenes.size() <  encoding2.transformHistoricalNodeToListNodeGenes.size()){
			inferiorLimit = resultOrganism->transformHistoricalNodeToListNodeGenes.size();
			superiorLimit = encoding2.transformHistoricalNodeToListNodeGenes.size();
			isSuperiorResultOrganism=false;
		}
		else{
			superiorLimit = resultOrganism->transformHistoricalNodeToListNodeGenes.size();
			inferiorLimit = encoding2.transformHistoricalNodeToListNodeGenes.size();
			isSuperiorResultOrganism=true;
		}

		for (uint i = 0; i < inferiorLimit; ++i)
		{
			if(encoding2.transformHistoricalNodeToListNodeGenes.at(i) == -1){
				;
			}
			else if(resultOrganism->transformHistoricalNodeToListNodeGenes.at(i) == -1 ){
				resultOrganism->addNode(encoding2.listNodeGenes.at(encoding2.transformHistoricalNodeToListNodeGenes.at(i)));
			}
			else if(rand()%2 == 1)
				resultOrganism->changeNode(encoding2.listNodeGenes.at(encoding2.transformHistoricalNodeToListNodeGenes.at(i)));
		}
		if(!isSuperiorResultOrganism){
			for (uint i = inferiorLimit; i < superiorLimit; ++i){
				if(encoding2.transformHistoricalNodeToListNodeGenes.at(i) != -1){
					resultOrganism->addNode(encoding2.listNodeGenes.at(encoding2.transformHistoricalNodeToListNodeGenes.at(i)));
				}
			}
		}
			



		// Connections:
		if( resultOrganism->transformInnovationToListConnectionGenes.size() <  encoding2.transformInnovationToListConnectionGenes.size()){
			inferiorLimit = resultOrganism->transformInnovationToListConnectionGenes.size();
			superiorLimit = encoding2.transformInnovationToListConnectionGenes.size();
			isSuperiorResultOrganism=false;
		}
		else{
			superiorLimit = resultOrganism->transformInnovationToListConnectionGenes.size();
			inferiorLimit = encoding2.transformInnovationToListConnectionGenes.size();
			isSuperiorResultOrganism=true;
		}

		for (uint i = 0; i < inferiorLimit; ++i)
		{
			if(encoding2.transformInnovationToListConnectionGenes.at(i) == -1){
				;
			}
			else if(resultOrganism->transformInnovationToListConnectionGenes.at(i) == -1 ){
				resultOrganism->addConnection(encoding2.listConnectionGenes.at(encoding2.transformInnovationToListConnectionGenes.at(i)));
			}
			else if( encoding2.listConnectionGenes.at(encoding2.transformInnovationToListConnectionGenes.at(i)).getEnable() )
				if(rand()%2 == 1)
					resultOrganism->changeConnection(encoding2.listConnectionGenes.at(encoding2.transformInnovationToListConnectionGenes.at(i)));

		}
		if(!isSuperiorResultOrganism)
			for (uint i = inferiorLimit; i < superiorLimit; ++i)
				if(encoding2.transformInnovationToListConnectionGenes.at(i) != -1)
					resultOrganism->addConnection(encoding2.listConnectionGenes.at(encoding2.transformInnovationToListConnectionGenes.at(i)));


		// before end we can know if is compatible with parents
		// is random for simetry
		int firstFather = rand()%2;
		if(firstFather){
			if(*resultOrganism %  encoding1 < resultOrganism->NEATinformation->getDistanceThreshold() )
				resultOrganism->setSpecie(encoding1.getSpecie());
			else if(*resultOrganism %  encoding2 < resultOrganism->NEATinformation->getDistanceThreshold() )
				resultOrganism->setSpecie(encoding2.getSpecie());
		}
		else{
			if(*resultOrganism %  encoding2 < resultOrganism->NEATinformation->getDistanceThreshold() )
				resultOrganism->setSpecie(encoding2.getSpecie());
			else if(*resultOrganism %  encoding1 < resultOrganism->NEATinformation->getDistanceThreshold() )
				resultOrganism->setSpecie(encoding1.getSpecie());
		}

		return resultOrganism;
	}

	ostream & operator << (ostream & o, geneticEncoding & encoding) {
		//First layers:
		o << "l\t";
		for (int i = 0; i < (int)encoding.layerOrdererList.size(); ++i)
		{
			o << encoding.layerOrdererList.at(i) << "\t";
		}
		o << "\n";
		//Second Nodes
		for (uint i = 0; i < encoding.listNodeGenes.size(); ++i)
		{
				o << "n\t" << encoding.listNodeGenes.at(i).getHistoricalNumber() << "\t" << encoding.listNodeGenes.at(i).getType() << "\t" << encoding.listNodeGenes.at(i).getLayer() << "\t" << encoding.listNodeGenes.at(i).getFunction() << endl;	
		}
		//Third Connections
		for (uint i = 0; i < encoding.listConnectionGenes.size(); ++i)
		{
				o << "c\t" << encoding.listConnectionGenes.at(i).getInnovation() << "\t" << encoding.listConnectionGenes.at(i).getIn() << "\t" << encoding.listConnectionGenes.at(i).getOut() << "\t" << encoding.listConnectionGenes.at(i).getWeight() << "\t" << encoding.listConnectionGenes.at(i).getEnable() << endl;
		}
		return o;
	}

	//Compatibility distance
	double  operator % (geneticEncoding & encoding1, geneticEncoding & encoding2){
		int inferiorLimit(0),superiorLimit(0); 
		bool isSuperiorEncoding1;
		double W(0.0),D(0.0),E(0.0);

		if( encoding1.transformInnovationToListConnectionGenes.size() <  encoding2.transformInnovationToListConnectionGenes.size()){
			inferiorLimit = encoding1.transformInnovationToListConnectionGenes.size();
			superiorLimit = encoding2.transformInnovationToListConnectionGenes.size();
			isSuperiorEncoding1=false;
		}
		else{
			superiorLimit = encoding1.transformInnovationToListConnectionGenes.size();
			inferiorLimit = encoding2.transformInnovationToListConnectionGenes.size();
			isSuperiorEncoding1=true;
		}
		for (int i = 0; i < inferiorLimit; ++i)
		{
			
			int position_enc1 = encoding1.transformInnovationToListConnectionGenes.at(i);
			int position_enc2 = encoding2.transformInnovationToListConnectionGenes.at(i);
			if( position_enc1 != -1 && position_enc2 != -1  ){
				W += abs(encoding1.listConnectionGenes.at(position_enc1).getWeight() - encoding2.listConnectionGenes.at(position_enc2).getWeight() );
			}
			else if(position_enc1 != -1 || position_enc2 != -1 ){
				D++;
			}
		}
		if(isSuperiorEncoding1)
			for (int i = inferiorLimit; i < superiorLimit; ++i)
			{
				int position_enc1 = encoding1.transformInnovationToListConnectionGenes.at(i);
				if(position_enc1 != -1)
					E++;
			}
		else
			for (int i = inferiorLimit; i < superiorLimit; ++i)
			{
				int position_enc2 = encoding2.transformInnovationToListConnectionGenes.at(i);
				if(position_enc2 != -1)
					E++;
			}

		//Lo siguiente es para que los genomas pequeños sean capaces de generar nichos diferentes así lograr una diversificación desde un principio.
		double divisor = inferiorLimit;
		
		return (encoding1.NEATinformation->getOrganismDistanceE()*E/divisor + encoding1.NEATinformation->getOrganismDistanceD()*D/divisor + encoding1.NEATinformation->getOrganismDistanceW()*W);	
	}
}



void geneticEncoding::printHumanReadable(){
	cerr << "layers: \n";
	for (int i = 0; i < (int)this->layerOrdererList.size(); ++i)
	{
		cerr << this->layerOrdererList.at(i) << "\t";
	}
	cerr << "\n";
	//Second Nodes
	for (uint i = 0; i < this->listNodeGenes.size(); ++i)
	{
			cerr << "node \tId: " << this->listNodeGenes.at(i).getHistoricalNumber() << "\tTipe: " << this->listNodeGenes.at(i).getType() << "\tLayer: " << this->listNodeGenes.at(i).getLayer() << "\tFunction: " << this->listNodeGenes.at(i).getFunction() << endl;	
	}
	//Third Connections
	for (uint i = 0; i < this->listConnectionGenes.size(); ++i)
	{
			cerr << "c\tId: " << this->listConnectionGenes.at(i).getInnovation() << "\tIn: " << this->listConnectionGenes.at(i).getIn() << "\tOut: " << this->listConnectionGenes.at(i).getOut() << "\tWiehght: " << this->listConnectionGenes.at(i).getWeight() << "\tEnable: " << this->listConnectionGenes.at(i).getEnable() << endl;
	}
}


void geneticEncoding::printLayersOnly(){ // for debuging mode
	cerr << "layers: \n";
	for (int i = 0; i < (int)this->layerOrdererList.size(); ++i)
	{
		cerr << this->layerOrdererList.at(i) << "\t";
	}
	cerr << "\n";
}