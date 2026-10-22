#include "Sensors/inc/IMU.hpp"
#include "stm32f4xx_hal.h"

#define IMU_CTRL1_REG 0x10 // Accelerometer settings
#define IMU_CTRL2_REG 0x11 // Gyroscope settings

void IMU::Initialize()
{
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
}

void IMU::DMA_TxRxCompleteCallback()
{
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
	uint8_t cipka = 15 + 5;
}

void IMU::DMA_ErrorCallback()
{
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
}

void IMU::Handle_Acc_Control_Registers() {
}

void IMU::Handle_Gyro_Control_Registers() {
}

void IMU::Initialize(IMU_UPDATE_RATES p_update_rate, IMU_ACC_SCALE p_acc_scale,
		IMU_GYRO_SCALE p_gyro_scale) {
}

void IMU::Read_Data()
{
	tx_buffer[0] = 0x8F;
	tx_buffer[1] = 0x00;
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);
	if (HAL_SPI_TransmitReceive_DMA(spi, tx_buffer, rx_buffer, 2) != HAL_OK)
	{
		HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
	}
}
