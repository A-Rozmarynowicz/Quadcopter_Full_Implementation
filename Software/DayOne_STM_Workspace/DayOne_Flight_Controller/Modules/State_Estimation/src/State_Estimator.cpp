#include "State_Estimation/inc/State_Estimator.hpp"
#include "Configuration/inc/Sensor_Config.hpp"

#pragma region Setup

bool State_Estimator::Initialize()
{
	return false;
}

bool State_Estimator::Initialize_Sensors(
		SPI_BUS_STRUCTURE &imu_spi,
		SPI_BUS_STRUCTURE &magnetometer_spi)
{
	bool imu_ok = _setup_imu(imu_spi, IMU_UPDATE_RATE, IMU_ACC_SCALE, IMU_GYRO_SCALE);
	bool mag_ok = _setup_magnetometer(magnetometer_spi, MAGNETOMETER_UPDATE_RATE);

	if (imu_ok && mag_ok)
	{
		return true;
	}
	return false;
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

bool State_Estimator::_setup_magnetometer(
		SPI_BUS_STRUCTURE &magnetometer_spi,
		MAGNETOMETER_UPDATE_RATES_ENUM magnetometer_update_rate)
{
	magnetometer.Setup(magnetometer_spi);
	HAL_StatusTypeDef code1 = magnetometer.Reset_Chip();
	HAL_Delay(15);
	SENSOR_STATUS code2 = magnetometer.Initialize(magnetometer_update_rate);

	if (code1 != HAL_OK || code2 != SENSOR_STATUS::INIT_OK)
	{
		return false;
	}
	return true;
}

#pragma endregion


#pragma region Callbacks

bool State_Estimator::SPI_DMA_TxCompleteCallback()
{
	return false;
}

bool State_Estimator::SPI_DMA_TxRxCompleteCallback()
{
	return false;
}

bool State_Estimator::SPI_DMA_ErrorCallback()
{
	return false;
}

bool State_Estimator::GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	return false;
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
