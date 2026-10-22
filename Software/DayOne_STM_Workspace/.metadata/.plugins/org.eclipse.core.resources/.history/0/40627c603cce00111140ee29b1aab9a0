#ifndef MAGNETOMETER_HPP
#define MAGNETOMETER_HPP

#include "stm32f4xx_hal.h"
#include "Sensor.hpp"
#include <cstdint>

class Magnetometer : public Sensor
{
public:
	void Initialize() override;

	void DMA_TxRxCompleteCallback() override;
	void DMA_ErrorCallback() override;

	void Read_Data() override;
};

#endif
