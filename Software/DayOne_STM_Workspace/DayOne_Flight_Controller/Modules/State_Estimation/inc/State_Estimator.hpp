#ifndef STATE_ESTIMATION_INC_STATE_ESTIMATOR_HPP_
#define STATE_ESTIMATION_INC_STATE_ESTIMATOR_HPP_

#include "Sensors/inc/IMU.hpp"
#include "Sensors/inc/Magnetometer.hpp"
#include "State_Estimation/inc/Kalman_Filter.hpp"
#include "Communication/inc/Bus_Structures.hpp"

class State_Estimator
{
private:
	Kalman_Filter KF_linear_x, KF_linear_y, KF_linear_z,
		KF_angular_x, KF_angular_y, KF_angular_z;

	IMU imu;
	Magnetometer magnetometer;
public:
	void Initialize();
	void Setup_SPI(
			SPI_BUS_STRUCTURE &imu_spi,
			SPI_BUS_STRUCTURE &magnetometer_spi);
};


#endif /* STATE_ESTIMATION_INC_STATE_ESTIMATOR_HPP_ */
