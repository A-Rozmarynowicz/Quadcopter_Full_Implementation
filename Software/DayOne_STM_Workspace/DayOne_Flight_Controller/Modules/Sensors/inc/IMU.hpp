#ifndef IMU_HPP
#define IMU_HPP

#include "stm32f4xx_hal.h"
#include "Sensor.hpp"
#include <cstdint>

enum class IMU_UPDATE_RATES
{
	Hz_833 = 0b01110000,
	Hz_1660 = 0b10000000,
};

enum class IMU_ACC_SCALE
{
	G_8 =  0b00001100,
	G_16 = 0b00000100,
};

enum class IMU_GYRO_SCALE
{
	DPS_500 =  0b00000100,
	DPS_1000 = 0b00001000,
};


class IMU : public Sensor
{
private:
	IMU_UPDATE_RATES update_rate;
	IMU_SCALE scale
	void Select_Update_Rate(IMU_UPDATE_RATES rate);
public:
	void Initialize(IMU_UPDATE_RATES update_rate) override;

	void DMA_TxRxCompleteCallback() override;
	void DMA_ErrorCallback() override;

	void Read_Data() override;
};

#endif
