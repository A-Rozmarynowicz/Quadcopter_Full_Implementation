#include "State_Estimation/inc/State_Estimator.hpp"

#pragma region Setup

bool State_Estimator::Initialize()
{
}

bool State_Estimator::Setup_SPI(
		SPI_BUS_STRUCTURE &imu_spi,
		SPI_BUS_STRUCTURE &magnetometer_spi)
{
	imu.Setup(imu_spi);

}

bool State_Estimator::Initialize_Sensors()
{
}
#pragma endregion


#pragma region Sensor Setup

bool State_Estimator::_setup_imu(
		SPI_BUS_STRUCTURE &imu_spi,
		IMU_UPDATE_RATES_ENUM imu_update_rate,
		IMU_ACC_SCALES_ENUM imu_acc_scale,
		IMU_GYRO_SCALES_ENUM imu_gyro_scale)
{
	imu.Setup(imu_spi);
	SENSOR_STATUS code = imu.Initialize(imu_update_rate, imu_acc_scale, imu_gyro_scale);
	imu.Prepare_For_Measurements();

	if (code == SENSOR_STATUS::INIT_OK)
	{
		return true;
	}
	return false;
}

bool State_Estimator::_setup_magnetometer(SPI_BUS_STRUCTURE &magnetometer_spi)
{

}

#pragma endregion


#pragma region Callbacks

bool State_Estimator::SPI_DMA_TxCompleteCallback()
{
}

bool State_Estimator::SPI_DMA_TxRxCompleteCallback()
{
}

bool State_Estimator::SPI_DMA_ErrorCallback()
{
}

bool State_Estimator::GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
}

void State_Estimator::Task_1ms()
{
}

#pragma endregion

#pragma region Settings

void State_Estimator::Set_Enable(bool en)
{
}

#pragma endregion
