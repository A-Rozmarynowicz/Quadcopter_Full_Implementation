#include "Sensors/inc/Magnetometer.hpp"
#include "stm32f4xx_hal.h"


void Magnetometer::DMA_TxRxCompleteCallback()
{
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
}

void Magnetometer::DMA_ErrorCallback()
{
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
}

void Magnetometer::Read_ID_Register()
{
}

void Magnetometer::Measurement_Cycle_Finished()
{
}

void Magnetometer::Task_Perform_Transaction()
{
//	HAL_SPI_TransmitReceive_DMA(hspi, current_transmit_buffer, current_receive_buffer, Size)
}
