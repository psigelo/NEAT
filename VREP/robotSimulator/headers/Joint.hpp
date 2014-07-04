#ifndef JOINT_HPP
#define JOINT_HPP

#include "Object.hpp"
#include <cstring>
#include <cmath>

#define PRECISION 6

namespace ANN_USM
{	
	class Joint : public Object
	{
		double * positions;
		double initial_position;
		double max_angle;
		double min_angle;

		int pass_slope_sign;
		int next_slope_sign;
		bool joint_change_direction;

		double truncValue(double value, int precision);
		double conversion(const char * way, double value);

	public:

		Joint(char name[], double max_angle, double min_angle, double position);
		~Joint();
		void setPosition(double position);
		void setInitialPosition();
		void setInitialPosition(double position);
		double * getAllPosition();
		double getCurrentPosition();
		double getInitialPosition();
		double getMaxAngle();
		double getMinAngle();
		bool getJointChangeDirection();
	};
}

#endif