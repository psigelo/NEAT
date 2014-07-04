#ifndef OBJECT_CPP
#define OBJECT_CPP

#include "Object.hpp"
using namespace ANN_USM;

Object::Object(char name[])
{
	this->name = new char;
	int size = sizeof(name);
	strncpy(this->name, name, size);
	position = new double[3];
	orientation = new double[3];
}

Object::Object()
{

}

Object::~Object()
{

}

void Object::setHandle(int handle)
{
	this->handle = handle;
}

void Object::setPosition(float position[3])
{
	for(int i = 0; i < 3; i++)
		this->position[i] = (double)position[i];
}

void Object::setOrientation(float orientation[3])
{
	for(int i = 0; i < 3; i++)
		this->orientation[i] = (double)orientation[i];
}
	
int Object::getHandle()
{
	return handle;
}

char * Object::getName()
{
	return name;
}

double * Object::getPosition()
{
	return position;
}

double * Object::getOrientation()
{
	return orientation;
}

#endif