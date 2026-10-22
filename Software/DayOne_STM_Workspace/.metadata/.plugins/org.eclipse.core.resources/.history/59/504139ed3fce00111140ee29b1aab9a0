#include "Sensors/inc/Magnetometer.hpp"
#include "stm32f4xx_hal.h"

void Magnetometer::Initialize()
{
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
}

void Magnetometer::DMA_TxRxCompleteCallback()
{
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
	uint8_t cipka = 15 + 5;
}

void Magnetometer::DMA_ErrorCallback()
{
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
}

void Magnetometer::Read_Data()
{
	tx_buffer[0] = 0x2F | 0x80;
	tx_buffer[1] = 0x00;
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);
	if (HAL_SPI_TransmitReceive_DMA(spi, tx_buffer, rx_buffer, 2) != HAL_OK)
	{
		HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
	}
}
