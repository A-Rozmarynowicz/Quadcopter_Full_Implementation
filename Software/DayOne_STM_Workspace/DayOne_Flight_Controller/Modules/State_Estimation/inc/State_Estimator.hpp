#ifndef STATE_ESTIMATION_INC_STATE_ESTIMATOR_HPP_
#define STATE_ESTIMATION_INC_STATE_ESTIMATOR_HPP_

#include "State_Estimation/inc/Kalman_Filter.hpp"

class State_Estimator
{
private:
	Kalman_Filter KF_linear_x, KF_linear_y, KF_linear_z,
		KF_angular_x, KF_angular_y, KF_angular_z;

public:
	void Initialize();

};


#endif /* STATE_ESTIMATION_INC_STATE_ESTIMATOR_HPP_ */
