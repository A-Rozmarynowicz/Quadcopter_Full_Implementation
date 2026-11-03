#ifndef STATE_ESTIMATION_INC_KALMAN_FILTER_HPP_
#define STATE_ESTIMATION_INC_KALMAN_FILTER_HPP_


class Kalman_Filter
{


public:
	void Propagate_State();
	void Update_With_Measurement();
};

#endif /* STATE_ESTIMATION_INC_KALMAN_FILTER_HPP_ */
