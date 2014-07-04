#ifndef ROBOTSIMULATOR_HPP
#define ROBOTSIMULATOR_HPP

#include "Object.hpp"
#include "Joint.hpp"
#include "Dummy.hpp"
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace ANN_USM;
using namespace std;

extern "C" {
    #include "extApi.h"
}

#define PORTNB 19998

class RobotSimulator
{
	int clientID;
	ofstream vrep_error;

public:
	RobotSimulator();
	~RobotSimulator();

	bool clientIdStatus();

	void simStart();
	void simFinish();
	int simGetConnectionId();
	void simPauseCommunication(int action);

	void simStartSimulation(simxInt operationMode);
	void simStopSimulation(simxInt operationMode);

	void simGetObjectHandle(Object * object, simxInt operationMode);
	void simGetObjectPosition(Object * object, Object * relativeTo, simxInt operationMode);
	void simGetObjectPosition(Object * object, int relativeTo, simxInt operationMode);
	void simGetObjectVelocity(Dummy * dummy, simxInt operationMode);
	void simGetObjectOrientation(Object * object, Object * relativeTo, simxInt operationMode);
	void simGetObjectOrientation(Object * object, int relativeTo, simxInt operationMode);

	double simGetJointPosition(Joint * joint, simxInt operationMode);
	void simSetJointTargetPosition(Joint * joint, simxInt operationMode);

	void simAddStatusbarMessage(char * message, simxInt operationMode);

};

#endif