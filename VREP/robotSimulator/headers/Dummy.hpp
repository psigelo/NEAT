#ifndef DUMMY_HPP
#define DUMMY_HPP

#include "Object.hpp"

namespace ANN_USM
{
	class Dummy : public Object
	{
		double velocity;

	public:

		Dummy(char name[]);
		Dummy();
		~Dummy();

		void setVelocity(float velocity);

		double getVelocity();
	};
}

#endif