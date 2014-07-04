#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <stdlib.h>
#include <iostream>
#include <cstring>

using namespace std;

namespace ANN_USM
{
	class Object
	{
		int handle;
		char * name;
		double * position;
		double * orientation;

	public:
		Object(char name[]);
		Object();
		~Object();	

		void setHandle(int handle);	
		void setPosition(float position[3]);
		void setOrientation(float orientation[3]);

		int getHandle();
		char * getName();
		double * getOrientation();
		double * getPosition();
	};
}

#endif