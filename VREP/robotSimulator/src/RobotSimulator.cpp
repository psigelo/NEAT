#ifndef ROBOTSIMULATOR_CPP
#define ROBOTSIMULATOR_CPP

#include "RobotSimulator.hpp"

RobotSimulator::RobotSimulator()
{
	vrep_error.open("error_files/vrep_error.txt");
}

RobotSimulator::~RobotSimulator()
{
	vrep_error.close();
}

bool RobotSimulator::clientIdStatus()
{
	if(clientID != -1) return true;
	else return false;
}

void RobotSimulator::simStart()
{
	clientID = simxStart((simxChar*)"127.0.0.1",PORTNB,true,true,2000,5);
	cerr << "clientID" << clientID << endl;
}

void RobotSimulator::simFinish()
{	
	simxFinish(clientID);
}

int RobotSimulator::simGetConnectionId()
{
	return simxGetConnectionId(clientID);
}

void RobotSimulator::simPauseCommunication(int action)
{
	simxPauseCommunication(clientID, action);
}

void RobotSimulator::simStartSimulation(simxInt operationMode)
{
	int error = simxStartSimulation(clientID, operationMode);
	if(error != 0) vrep_error << " try 1 simxStartSimulation : " << error << endl;
	error = simxStartSimulation(clientID, operationMode);
	if(error != 0) vrep_error << "try 2 simxStartSimulation : " << error << endl;
}

void RobotSimulator::simStopSimulation(simxInt operationMode)
{
	int error = simxStopSimulation(clientID, operationMode);
	if(error != 0) vrep_error << "simxStopSimulation : " << error << endl;
}

void RobotSimulator::simGetObjectHandle(Object * object, simxInt operationMode)
{
	int handle;
	int error = simxGetObjectHandle(clientID, object->getName(), &handle, operationMode);	
	if(error != 0) vrep_error << "simxGetObjectHandle - " << object->getName() << " : "<< error << endl;
	object->setHandle(handle);
}

void RobotSimulator::simGetObjectPosition(Object * object, Object * relativeTo, simxInt operationMode)
{
	float position[3];
	int error = simxGetObjectPosition(clientID, object->getHandle(), relativeTo->getHandle(), position, operationMode);
	if(error != 0) vrep_error << "simxGetObjectPosition - " << object->getName() << " : "<< error << endl;
	object->setPosition(position);
}

void RobotSimulator::simGetObjectPosition(Object * object, int relativeTo, simxInt operationMode)
{
	float position[3];
	int error = simxGetObjectPosition(clientID, object->getHandle(), relativeTo, position, operationMode);
	if(error != 0) vrep_error << "simxGetObjectPosition - " << object->getName() << " : "<< error << endl;
	object->setPosition(position);
}

void RobotSimulator::simGetObjectVelocity(Dummy * dummy, simxInt operationMode)
{
	float velocity;
	int error = simxGetObjectVelocity(clientID, dummy->getHandle(), &velocity, NULL, operationMode);
	if(error != 0) vrep_error << "simxGetObjectVelocity - " << dummy->getName() << " : "<< error << endl;
	dummy->setVelocity(velocity);
}

void RobotSimulator::simGetObjectOrientation(Object * object, Object * relativeTo, simxInt operationMode)
{
	float orientation[3];
	int error = simxGetObjectOrientation(clientID,object->getHandle(), relativeTo->getHandle(), orientation, operationMode);
	if(error != 0) vrep_error << "simxGetObjectOrientation - " << object->getName() << " : "<< error << endl;
	object->setOrientation(orientation);
}

void RobotSimulator::simGetObjectOrientation(Object * object, int relativeTo, simxInt operationMode)
{
	float orientation[3];
	int error = simxGetObjectOrientation(clientID,object->getHandle(), relativeTo, orientation, operationMode);
	if(error != 0) vrep_error << "simxGetObjectOrientation - " << object->getName() << " : "<< error << endl;
	object->setOrientation(orientation);
}

double RobotSimulator::simGetJointPosition(Joint * joint, simxInt operationMode)
{
	float joint_pos;
	int error = simxGetJointPosition(clientID, joint->getHandle(), &joint_pos, operationMode);
	if(error != 0) vrep_error << "simxGetJointPosition - " << joint->getName() << " : "<< error << endl;
	joint->setInitialPosition((double)joint_pos);	
	return (double)joint_pos;
}

void RobotSimulator::simSetJointTargetPosition(Joint * joint, simxInt operationMode)
{
	int error = simxSetJointTargetPosition(clientID, joint->getHandle(), (float)joint->getCurrentPosition(), operationMode);
	if(error != 0) vrep_error << "simxSetJointTargetPosition - " << joint->getName() << " : "<< error << endl;		
}

void RobotSimulator::simAddStatusbarMessage(char * message, simxInt operationMode)
{
	simxAddStatusbarMessage(clientID,(char*)message, operationMode);	
} 



#endif