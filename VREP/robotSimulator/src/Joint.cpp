#ifndef JOINT_CPP
#define JOINT_CPP

#include "Joint.hpp"
#include <iostream>
using namespace ANN_USM;

double Joint::truncValue(double value, int precision)
{
	return (double)floor(value*pow(10,precision))/pow(10,precision);
}

double Joint::conversion(const char * way, double value)
{
	if (!strcmp(way,(char *)"RAD"))	return (max_angle - min_angle )*(value + 1.0)/2.0 + min_angle;
	else if (!strcmp(way,(char *)"SCALE")) return 2.0*(value - min_angle)/(max_angle - min_angle) - 1.0;
	else return 0.0;
}

Joint::Joint(char name[], double max_angle, double min_angle, double position) : Object(name)
{
	this->max_angle = max_angle;
	this->min_angle = min_angle;
	initial_position = truncValue(conversion((char *)"SCALE", position), PRECISION);

	positions = new double[3];
	positions[0] = initial_position;
	positions[1] = initial_position;
	positions[2] = initial_position;	
	joint_change_direction = false;
}

Joint::~Joint()
{

}

void Joint::setPosition(double position)
{
	double aux_slope = 0;

	positions[2] = positions[1];
	positions[1] = positions[0];
	positions[0] = truncValue(position,PRECISION);	

	pass_slope_sign = next_slope_sign;
	aux_slope = positions[0] - positions[1];

	if(aux_slope != 0)
	{
		if(aux_slope < 0) next_slope_sign = -1;
		else next_slope_sign = 1; 
	}		

	if(next_slope_sign != pass_slope_sign) joint_change_direction = true;
	else joint_change_direction = false;

}

void Joint::setInitialPosition()
{
	positions[2] = positions[1] = positions[0] = initial_position;	
	pass_slope_sign = 1;
	next_slope_sign = 1;	
	joint_change_direction = false;
}

void Joint::setInitialPosition(double position)
{
	positions[2] = positions[1] = positions[0] = initial_position = truncValue(conversion((char *)"SCALE", position), PRECISION);		
	pass_slope_sign = 1;
	next_slope_sign = 1;	
	joint_change_direction = false;
}

double * Joint::getAllPosition()
{
	return positions;
}

double Joint::getCurrentPosition()
{
	return truncValue(conversion((char *)"RAD", positions[0]), PRECISION);
}

double Joint::getInitialPosition()
{
	return initial_position;
}

double Joint::getMaxAngle()
{
	return max_angle;
}

double Joint::getMinAngle()
{
	return min_angle;
}

bool Joint::getJointChangeDirection()
{
	return joint_change_direction;
}

#endif

