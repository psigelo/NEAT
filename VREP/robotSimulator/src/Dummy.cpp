#ifndef DUMMY_CPP
#define DUMMY_CPP

#include "Dummy.hpp"
using namespace ANN_USM;

Dummy::Dummy(char name[]) : Object(name)
{
	velocity = 0.0;
}

Dummy::Dummy()
{

}

Dummy::~Dummy()
{

}

void Dummy::setVelocity(float velocity)
{
	this->velocity = (double)velocity;
}

double Dummy::getVelocity()
{
	return velocity;
}

#endif

