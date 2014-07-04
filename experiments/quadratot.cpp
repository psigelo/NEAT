#include "NEAT.hpp"
#include "genetic_encoding.hpp"
#include <ctime>
#include "RobotSimulator.hpp"
#include "Dummy.hpp"
#include "Joint.hpp"
#include <sys/time.h>


extern "C" {
    #include "extApi.h"
}

using namespace ANN_USM;
using namespace std;

#define TIME_SIMULATION 12.0
#define TIME_WITHOUT_FITNESS_CALC 2.0
#define WAVE_FRECUENCY 0.64
#define FRECUENCY_CMD 160.0
#define N_LEGS 4
#define GRA_LIB_EXT 1
#define GRA_LIB 2


#define RAD (double)M_PI/180.0
#define MAX_ANGLE_INNER 60.0*RAD
#define MIN_ANGLE_INNER -85.0*RAD
#define MAX_ANGLE_OUTER 39.0*RAD
#define MIN_ANGLE_OUTER -113.0*RAD
#define MAX_ANGLE_CENTER 23.0*RAD
#define MIN_ANGLE_CENTER -23.0*RAD
#define MAX_ANGLE_LIMIT {MAX_ANGLE_INNER,	MAX_ANGLE_INNER,	MAX_ANGLE_INNER,	MAX_ANGLE_INNER,	MAX_ANGLE_OUTER,	MAX_ANGLE_OUTER,	MAX_ANGLE_OUTER,	MAX_ANGLE_OUTER,	MAX_ANGLE_CENTER}
#define MIN_ANGLE_LIMIT {MIN_ANGLE_INNER,	MIN_ANGLE_INNER,	MIN_ANGLE_INNER,	MIN_ANGLE_INNER,	MIN_ANGLE_OUTER,	MIN_ANGLE_OUTER,	MIN_ANGLE_OUTER,	MIN_ANGLE_OUTER,	MIN_ANGLE_CENTER}



double getTimeSimulation(timeval final, timeval init)
{
	return (double)((final.tv_sec-init.tv_sec)*1000.0+(final.tv_usec-init.tv_usec)/1000.0)/1000.0;
}



double fitness(Genetic_Encoding organism, RobotSimulator * simulator, vector < Joint * > joints, vector < Dummy * > tips){

	timeval init;
	timeval final;

	vector <double> inputs_to_NEAT;
	vector <double> output_from_NEAT;
	vector <double> output_from_NEAT_t_minis_1;



	gettimeofday(&init,NULL);
	gettimeofday(&final,NULL);


	//===============Se dan los valores iniciales========

	for(int i=0; i < (int)joints.size(); i++){
		joints.at(i)->setInitialPosition();
		simulator->simSetJointTargetPosition(joints.at(i), simx_opmode_oneshot_wait);
		output_from_NEAT_t_minis_1.push_back(joints.at(i)->getCurrentPosition());
	}

	//===================================================

	
	for(int i=0; i < (int)joints.size(); i++){
		output_from_NEAT.push_back(0);
		output_from_NEAT_t_minis_1.push_back(0);
	}
	


	simulator->simStartSimulation(simx_opmode_oneshot_wait);

	while(getTimeSimulation(final,init) < TIME_SIMULATION){

		//LOS PRIMEROS SEGUNDOS NO SE CALCULA EL FITNESS.
		//if(getTimeSimulation(final,init) < TIME_WITHOUT_FITNESS_CALC){

			
			vector <double> means_of_outputs;
			for(int i=0; i < (int)joints.size(); i++){
				means_of_outputs.push_back(0);
			}

			for(int j=0; j < 4; j++ ){ // SE CALCULAN 4 VECES LAS MEDICIONES A 160 HZ PARA OBTENER UNA MEDICIÓN DEL PROMEDIO A 40HZ
				gettimeofday(&final,NULL); // Define el tiempo actual.
				vector <double> temp_output;
				inputs_to_NEAT.clear(); //SE BORRAN LOS DATOS DE ENTRADA.
										//OBTENER LAS ENTRADAS A LA RED
										//PRIMERO LAS ENTRADAS SINUSOIDALES Y COSENOIDALES.
				inputs_to_NEAT.push_back(sin(2.0*M_PI*WAVE_FRECUENCY*getTimeSimulation(final,init)));
				inputs_to_NEAT.push_back(cos(2.0*M_PI*WAVE_FRECUENCY*getTimeSimulation(final,init)));

				// SE USAN LOS VALORES DE SALIDA DEL NEAT COMO ENTRADAS DEL NEAT, DADO QUE LA SALIDA DEL NEAT SON LOS VALORES QUE 
				// EN ESTE MOMENTO CORRESPONDEN A LA POSICION ACTUAL DEL NEAT.
				for(int i=0; i < (int)joints.size(); i++){
					inputs_to_NEAT.push_back(output_from_NEAT_t_minis_1.at(i));
				}
				temp_output = organism.eval(inputs_to_NEAT);
				for (int i = 0; i < (int)joints.size(); ++i)
				{
					means_of_outputs.at(i) += temp_output.at(i)/4.0;	
				}

				usleep((int)((1.0/FRECUENCY_CMD)*1000000.0)); // Para que se tomen a 160 Hz las mediciones
			}


			for (int i = 0; i < (int)joints.size(); ++i)
			{
				output_from_NEAT.at(i) = means_of_outputs.at(i); 
				joints.at(i)->setPosition(output_from_NEAT.at(i));
				output_from_NEAT_t_minis_1.at(i) = output_from_NEAT.at(i);	
			}

			simulator->simPauseCommunication(1);
			for(int i = 0; i < (int)joints.size(); i++)
			{
				simulator->simSetJointTargetPosition(joints.at(i), simx_opmode_oneshot);
			}
			simulator->simPauseCommunication(0);
			gettimeofday(&final,NULL); //Define el tiempo actual.
		/*}
		else{ //AHORA COMIENZA EL CALCULO DEL FITNESS

		}
		*/
	}

	simulator->simStopSimulation(simx_opmode_oneshot_wait);


	return 10;

}






int main(int argc, char** argv){

	std::srand(std::time(0)); // use current time as seed for random generator
	if(argc < 3){
		cerr << "Arguments missing, The relative path to the user definitions and genetic encoding files (.genetic_encoding files) must be defined!. \n";
		exit(1);
	}


	RobotSimulator * simulator = new RobotSimulator();
	simulator->simStart();

	cerr << "paso" << endl;

	// ============= VREP INITIALIZATIONS ============= //			
	
	vector < Joint * > joints;
	vector < Dummy * > tips;
	Dummy * center_dummy = new Dummy((char*)"center");

	double max_angle_limit[] = MAX_ANGLE_LIMIT;
	double min_angle_limit[] = MIN_ANGLE_LIMIT;


	cerr << "paso2" << endl;
	for(int i = 0; i < N_LEGS*GRA_LIB + GRA_LIB_EXT; i++)
	{
		stringstream joint;
		joint << "joint" << i;
		joints.push_back(new Joint((char*)joint.str().c_str(), max_angle_limit[i], min_angle_limit[i], 0));

		cerr << "wtf" << endl;
		simulator->simGetObjectHandle(joints.at(i), simx_opmode_oneshot_wait);
		cerr << "wtf2" << endl;
		simulator->simGetJointPosition(joints.at(i), simx_opmode_oneshot_wait);
	}
	cerr << "paso3" << endl;
	for(int i = 0; i < N_LEGS; i++)
	{			
		stringstream tip;
		tip << "tip" << i; 
		tips.push_back(new Dummy((char*)tip.str().c_str()));

		simulator->simGetObjectHandle(tips.at(i), simx_opmode_oneshot_wait);
	}
	cerr << "paso4" << endl;
	simulator->simGetObjectHandle(center_dummy, simx_opmode_oneshot_wait);

	// ================================================ //




	
	Population poblacion(argv[1],argv[2]);
	cerr << poblacion.GENERATIONS << "\t" << poblacion.POPULATION_MAX << "\t" << poblacion.organisms.size() << endl;
	cerr << poblacion.organisms.at(5) << endl;
	for (int i = 0; i < poblacion.GENERATIONS; ++i){
		for (int i = 0; i < (int)poblacion.organisms.size(); ++i)
		{
			poblacion.organisms.at(i).fitness = fitness(poblacion.organisms.at(i),simulator, joints, tips);
		}
		poblacion.epoch();
	}
	cout << "Fitness champion: " << poblacion.fitness_champion << "\n\n"<< endl;
	cout << poblacion.champion.ANN_function() << endl;
	cout << poblacion.champion << endl;
	return 0;
}