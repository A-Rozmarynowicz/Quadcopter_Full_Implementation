#ifndef STATE_ESTIMATION_INC_STATE_ESTIMATOR_HPP_
#define STATE_ESTIMATION_INC_STATE_ESTIMATOR_HPP_

#include "Sensors/inc/IMU.hpp"
#include "Sensors/inc/Magnetometer.hpp"
#include "State_Estimation/inc/Kalman_Filter.hpp"
#include "Communication/inc/Bus_Structures.hpp"


class State_Estimator
{
private:
	bool enabled;

	Kalman_Filter KF_linear_x, KF_linear_y, KF_linear_z,
		KF_angular_x, KF_angular_y, KF_angular_z;

	IMU imu;
	Magnetometer magnetometer;

	bool _setup_imu(
			SPI_BUS_STRUCTURE &imu_spi,
			IMU_UPDATE_RATES_ENUM imu_update_rate,
			IMU_ACC_SCALES_ENUM imu_acc_scale,
			IMU_GYRO_SCALES_ENUM imu_gyro_scale);

	bool _setup_magnetometer(
			SPI_BUS_STRUCTURE &magnetometer_spi,
			MAGNETOMETER_UPDATE_RATES_ENUM magnetometer_update_rate);
public:
	bool Initialize();
	bool Setup_SPI(
			SPI_BUS_STRUCTURE &imu_spi,
			SPI_BUS_STRUCTURE &magnetometer_spi);
	bool Initialize_Sensors();

	bool SPI_DMA_TxCompleteCallback();
	bool SPI_DMA_TxRxCompleteCallback();
	bool SPI_DMA_ErrorCallback();
	bool GPIO_EXTI_Callback(uint16_t GPIO_Pin);

	void Task_1ms();

	void Set_Enable(bool en);
};


#endif /* STATE_ESTIMATION_INC_STATE_ESTIMATOR_HPP_ */
