#include "NEAT.hpp"
#include "genetic_encoding.hpp"
#include <ctime>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

extern "C" {
    #include "extApi.h"
}

using namespace ANN_USM;
using namespace std;

#define TIME_SIMULATION 6.0
#define TIME_WITHOUT_FITNESS_CALC 1.0
#define WAVE_FRECUENCY 0.64
#define FRECUENCY_CMD 160.0
#define N_LEGS 4
#define GRA_LIB_EXT 1
#define GRA_LIB 2


#define RAD (double)M_PI/180.0



#define LIMIT_INF_EXT -2.2
#define LIMIT_SUP_EXT 0.8

#define LIMIT_INF_INT -0.8
#define LIMIT_SUP_INT 0.8

#define LIMIT_INF_CENT -0.4014
#define LIMIT_SUP_CENT 0.4014



double getTimeSimulation(timeval final, timeval init)
{
	return (double)((final.tv_sec-init.tv_sec)*1000.0+(final.tv_usec-init.tv_usec)/1000.0)/1000.0;
}

double gaussian(double x){
	return (1.3/(sqrt(2*M_PI)*0.3))*exp(-pow(x-1.3,2)/(2*0.3));
}

double fitness( Genetic_Encoding organism ,int handle_joints[N_LEGS*GRA_LIB + GRA_LIB_EXT], int handle_tips[N_LEGS], int * handle_dummy, int * clientID){

	//static double max_angle_limit[] = MAX_ANGLE_LIMIT;
	//static double min_angle_limit[] = MIN_ANGLE_LIMIT;
	timeval init;
	timeval final;

	vector <double> inputs_to_NEAT;
	vector <double> output_from_NEAT;
	vector <double> output_from_NEAT_t_minis_1;

	vector < vector <double> > pass_positions; // Para sacar la cantidad de cambios de direccion
											  //  at(0): present   at(1): t-1   at(2): t-2

	int cantidad_cambios_direccion_totales(0);
	static vector <int> next_slope;


	float position_init[3];
	float position_end[3];

	gettimeofday(&init,NULL);
	gettimeofday(&final,NULL);

/*
	//===============Se dan los valores iniciales========
	for(int i=0; i < (int)joints.size(); i++){
		joints.at(i)->setInitialPosition();
		simulator->simSetJointTargetPosition(joints.at(i), simx_opmode_oneshot_wait);
		output_from_NEAT_t_minis_1.push_back(joints.at(i)->getCurrentPosition());
	}
	//===================================================
*/
	
	for(int i=0; i < N_LEGS*GRA_LIB + GRA_LIB_EXT; i++){
		output_from_NEAT.push_back(0);
		output_from_NEAT_t_minis_1.push_back(0.0);
		next_slope.push_back(1);
		vector < double > tmp;
		pass_positions.push_back( tmp );
		for(int j=0; j < 3; j++){
			pass_positions.at(i).push_back(0.0);
		}
	}
	


	simxStartSimulation(*clientID, simx_opmode_oneshot_wait);
	simxStartSimulation(*clientID, simx_opmode_oneshot_wait);

	while(getTimeSimulation(final,init) < TIME_SIMULATION){

		//LOS PRIMEROS SEGUNDOS NO SE CALCULA EL FITNESS.
		if(getTimeSimulation(final,init) < TIME_WITHOUT_FITNESS_CALC){

			
			vector <double> means_of_outputs;
			for(int i=0; i < N_LEGS*GRA_LIB + GRA_LIB_EXT; i++){
				means_of_outputs.push_back(0.0);
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
				//for(int i=0; i < N_LEGS*GRA_LIB + GRA_LIB_EXT; i++){
				//	inputs_to_NEAT.push_back(output_from_NEAT_t_minis_1.at(i));
				//}
				temp_output = organism.eval(inputs_to_NEAT);
				// EL PROMEDIO DE LAS ENTRA
				for (int i = 0; i < N_LEGS*GRA_LIB + GRA_LIB_EXT; ++i)
				{
					means_of_outputs.at(i) += temp_output.at(i)/4.0;	
				}
				usleep((int)((1.0/FRECUENCY_CMD)*1000000.0)); // Para que se tomen a 160 Hz las mediciones
			}

			for (int i = 0; i < N_LEGS*GRA_LIB + GRA_LIB_EXT; ++i)
			{
				output_from_NEAT.at(i) = means_of_outputs.at(i); 
				output_from_NEAT_t_minis_1.at(i) = output_from_NEAT.at(i);	
			}
			
			for(int i = 0; i < N_LEGS*GRA_LIB + GRA_LIB_EXT; i++)
			{
				
				pass_positions.at(i).at(2) = pass_positions.at(i).at(1);
				pass_positions.at(i).at(1) = pass_positions.at(i).at(0);
				pass_positions.at(i).at(0) = output_from_NEAT[i];
				
				
				double aux_slope = pass_positions.at(i).at(0) - pass_positions.at(i).at(1);

				if(aux_slope != 0)
				{
					if(aux_slope < 0) next_slope.at(i) = -1;
					else next_slope.at(i) = 1; 
				}
			} 

			simxPauseCommunication(*clientID, 1);
			simxSetJointTargetPosition(*clientID, handle_joints[0], LIMIT_INF_INT + (LIMIT_SUP_INT - LIMIT_INF_INT)*0.5*output_from_NEAT[0]*M_PI , simx_opmode_oneshot);
			simxSetJointTargetPosition(*clientID, handle_joints[1], LIMIT_INF_INT + (LIMIT_SUP_INT - LIMIT_INF_INT)*0.5*output_from_NEAT[1]*M_PI , simx_opmode_oneshot);
			simxSetJointTargetPosition(*clientID, handle_joints[2], LIMIT_INF_INT + (LIMIT_SUP_INT - LIMIT_INF_INT)*0.5*output_from_NEAT[2]*M_PI , simx_opmode_oneshot);
			simxSetJointTargetPosition(*clientID, handle_joints[3], LIMIT_INF_INT + (LIMIT_SUP_INT - LIMIT_INF_INT)*0.5*output_from_NEAT[3]*M_PI , simx_opmode_oneshot);
			
			simxSetJointTargetPosition(*clientID, handle_joints[4], LIMIT_INF_EXT + (LIMIT_SUP_EXT - LIMIT_INF_EXT)*0.5*output_from_NEAT[4]*M_PI , simx_opmode_oneshot);
			simxSetJointTargetPosition(*clientID, handle_joints[5], LIMIT_INF_EXT + (LIMIT_SUP_EXT - LIMIT_INF_EXT)*0.5*output_from_NEAT[5]*M_PI , simx_opmode_oneshot);
			simxSetJointTargetPosition(*clientID, handle_joints[6], LIMIT_INF_EXT + (LIMIT_SUP_EXT - LIMIT_INF_EXT)*0.5*output_from_NEAT[6]*M_PI , simx_opmode_oneshot);
			simxSetJointTargetPosition(*clientID, handle_joints[7], LIMIT_INF_EXT + (LIMIT_SUP_EXT - LIMIT_INF_EXT)*0.5*output_from_NEAT[7]*M_PI , simx_opmode_oneshot);
			
			simxSetJointTargetPosition(*clientID, handle_joints[8], LIMIT_INF_EXT + (LIMIT_SUP_CENT - LIMIT_INF_CENT)*0.5*output_from_NEAT[8]*M_PI , simx_opmode_oneshot);
			simxPauseCommunication(*clientID, 0);


			simxGetObjectPosition(*clientID, *handle_dummy, -1, position_init, simx_opmode_oneshot_wait);
			gettimeofday(&final,NULL); //Define el tiempo actual.
		}






		else{ //AHORA COMIENZA EL CALCULO DEL FITNESS



			vector <double> means_of_outputs;
			for(int i=0; i < N_LEGS*GRA_LIB + GRA_LIB_EXT; i++){
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
				//for(int i=0; i < N_LEGS*GRA_LIB + GRA_LIB_EXT; i++){
				//	inputs_to_NEAT.push_back(output_from_NEAT_t_minis_1.at(i));
				//}
				temp_output = organism.eval(inputs_to_NEAT);
				// EL PROMEDIO DE LAS ENTRA
				for (int i = 0; i < N_LEGS*GRA_LIB + GRA_LIB_EXT; ++i)
				{
					means_of_outputs.at(i) += temp_output.at(i)/4.0;	
				}

				usleep((int)((1.0/FRECUENCY_CMD)*1000000.0)); // Para que se tomen a 160 Hz las mediciones
			}


			for (int i = 0; i < N_LEGS*GRA_LIB + GRA_LIB_EXT; ++i)
			{
				output_from_NEAT.at(i) = means_of_outputs.at(i); 
				output_from_NEAT_t_minis_1.at(i) = output_from_NEAT.at(i);	
			}

			
			for(int i = 0; i < N_LEGS*GRA_LIB + GRA_LIB_EXT; i++)
			{
				pass_positions.at(i).at(2) = pass_positions.at(i).at(1);
				pass_positions.at(i).at(1) = pass_positions.at(i).at(0);
				pass_positions.at(i).at(0) = output_from_NEAT[i];

				int pass_slope = next_slope.at(i);
				double aux_slope = pass_positions.at(i).at(0) - pass_positions.at(i).at(1);
				if(aux_slope != 0)
				{
					if(aux_slope < 0) next_slope.at(i) = -1;
					else next_slope.at(i) = 1; 
				}	
				if(next_slope.at(i) != pass_slope) cantidad_cambios_direccion_totales++;
				
			} 



			simxPauseCommunication(*clientID, 1);
			simxSetJointTargetPosition(*clientID, handle_joints[0], LIMIT_INF_INT + (LIMIT_SUP_INT - LIMIT_INF_INT)*0.5*output_from_NEAT[0]*M_PI , simx_opmode_oneshot);
			simxSetJointTargetPosition(*clientID, handle_joints[1], LIMIT_INF_INT + (LIMIT_SUP_INT - LIMIT_INF_INT)*0.5*output_from_NEAT[1]*M_PI , simx_opmode_oneshot);
			simxSetJointTargetPosition(*clientID, handle_joints[2], LIMIT_INF_INT + (LIMIT_SUP_INT - LIMIT_INF_INT)*0.5*output_from_NEAT[2]*M_PI , simx_opmode_oneshot);
			simxSetJointTargetPosition(*clientID, handle_joints[3], LIMIT_INF_INT + (LIMIT_SUP_INT - LIMIT_INF_INT)*0.5*output_from_NEAT[3]*M_PI , simx_opmode_oneshot);
			
			simxSetJointTargetPosition(*clientID, handle_joints[4], LIMIT_INF_EXT + (LIMIT_SUP_EXT - LIMIT_INF_EXT)*0.5*output_from_NEAT[4]*M_PI , simx_opmode_oneshot);
			simxSetJointTargetPosition(*clientID, handle_joints[5], LIMIT_INF_EXT + (LIMIT_SUP_EXT - LIMIT_INF_EXT)*0.5*output_from_NEAT[5]*M_PI , simx_opmode_oneshot);
			simxSetJointTargetPosition(*clientID, handle_joints[6], LIMIT_INF_EXT + (LIMIT_SUP_EXT - LIMIT_INF_EXT)*0.5*output_from_NEAT[6]*M_PI , simx_opmode_oneshot);
			simxSetJointTargetPosition(*clientID, handle_joints[7], LIMIT_INF_EXT + (LIMIT_SUP_EXT - LIMIT_INF_EXT)*0.5*output_from_NEAT[7]*M_PI , simx_opmode_oneshot);
			
			simxSetJointTargetPosition(*clientID, handle_joints[8], LIMIT_INF_EXT + (LIMIT_SUP_CENT - LIMIT_INF_CENT)*0.5*output_from_NEAT[8]*M_PI , simx_opmode_oneshot);
			simxPauseCommunication(*clientID, 0);



			int error=simxGetObjectPosition(*clientID, *handle_dummy, -1, position_end, simx_opmode_oneshot_wait);
			if(error != 0) cerr << "simxGetObjectPosition - " << error << endl;
			gettimeofday(&final,NULL); //Define el tiempo actual.
		}
		
	}

	simxStopSimulation(*clientID, simx_opmode_oneshot_wait);
	//cerr << position_init[0] << "\t"<< position_init[1] << "\t"<< position_init[2] << "\t"<< position_end[0] << "\t"<< position_end[1] << "\t"<< position_end[2] << endl;
	//cerr << "cambios totales: " << cantidad_cambios_direccion_totales << "\tDistancia total: " << sqrt(pow(position_init[0] - position_end[0],2) + pow(position_init[1] - position_end[1],2)) << endl;
	
	//double distance =  sqrt(pow(position_init[0] - position_end[0],2) + pow(position_init[1] - position_end[1],2));
	double distance_in_rect = abs((1.0/sqrt(2.0)) * (position_end[0] - position_end[1]));
	cerr << "distance_in_rect: " << distance_in_rect << endl;
	double frec = cantidad_cambios_direccion_totales/(9.0 * (double)(TIME_SIMULATION - TIME_WITHOUT_FITNESS_CALC)) ;
	cerr << "frec: " << frec << endl;
	double fitness = pow(2, distance_in_rect*gaussian( frec ) ) - 0.99999;
	cerr << "fitness: " << fitness << endl;
	return fitness;

}






int main(int argc, char** argv){

	std::srand(std::time(0)); // use current time as seed for random generator
	if(argc < 3){
		cerr << "Arguments missing, The relative path to the user definitions and genetic encoding files (.genetic_encoding files) must be defined!. \n";
		exit(1);
	}


	int clientID = simxStart((simxChar*)"127.0.0.1",19997,true,true,2000,5);
	if(clientID == -1) {cerr << "ERROR::clientID = 1" << endl; exit(1);}
	// ============= VREP INITIALIZATIONS ============= //			
	
	


	
	int handle_joints [N_LEGS*GRA_LIB + GRA_LIB_EXT];
	int handle_tips [ N_LEGS ];
	int handle_dummy;

	for(int i = 0; i < N_LEGS*GRA_LIB + GRA_LIB_EXT; i++)
	{
		stringstream joint;
		joint << "joint" << i;
		int error = simxGetObjectHandle(clientID, joint.str().c_str(), &handle_joints[i], simx_opmode_oneshot_wait);
		if(error != 0){cerr << "error on simxgetobjecthandle::Joints: " << error << endl; exit(1);}
		//simulator->simGetJointPosition(joints.at(i), simx_opmode_oneshot_wait);
	}

	for(int i = 0; i < N_LEGS; i++)
	{			
		stringstream tip;
		tip << "tip" << i; 
		int error = simxGetObjectHandle(clientID, tip.str().c_str(), &handle_tips[i], simx_opmode_oneshot_wait);
		if(error != 0){cerr << "error on simxgetobjecthandle::Tips: " << error << endl; exit(1);}
	}

	simxGetObjectHandle(clientID, "center", &handle_dummy, simx_opmode_oneshot_wait);

	// ================================================ //


	// Se imprimen el promedio de todos los fitness de cada generacion para luego graficar.
	FILE * fitness_file = fopen("./Statistics/quadratot2_fitness.txt", "w");
	double fitness_file_value(0);

	Population poblacion(argv[1],argv[2], (char *)"cerebro");


	

	for (int i = 0; i < poblacion.GENERATIONS; ++i){
		fitness_file_value = 0.0;
		poblacion.print_to_file_currrent_generation();
		for (int j = 0; j < (int)poblacion.organisms.size(); ++j)
		{

			cerr << "=======================================================================" << endl;
			cerr << "G: " << i << "\tP: " << j << endl; 
			poblacion.organisms.at(j).fitness = fitness(poblacion.organisms.at(j), handle_joints, handle_tips, &handle_dummy, &clientID);
			fitness_file_value += poblacion.organisms.at(j).fitness;
			sleep(1); // Para que no muera VREṔ
		}

		fprintf(fitness_file, "%lf\n", fitness_file_value);
		fflush(fitness_file);

		poblacion.epoch();
	}

	fclose(fitness_file);

	cout << "Fitness champion: " << poblacion.fitness_champion << "\n\n"<< endl;
	cout << poblacion.champion.ANN_function() << endl;
	cout << poblacion.champion << endl;




	return 0;
}