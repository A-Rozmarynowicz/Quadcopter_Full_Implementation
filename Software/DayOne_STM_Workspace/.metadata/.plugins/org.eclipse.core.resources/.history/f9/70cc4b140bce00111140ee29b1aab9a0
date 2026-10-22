#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <cstdint>
#include "stm32f4xx_hal.h"

uint8_t cipka = 8;

class Sensor
{
private:
	SPI_HandleTypeDef spi;
	GPIO_TypeDef cs_port;
	uint16_t cs_pin;
public:
	void DMA_TxRxCompleteCallback();
	void IMU_ErrorCallback();

	SPI_HandleTypeDef Get_SPI() {
		return spi;
	}
};


#endif
