#include "Application/inc/Application.hpp"
#include "main.h"

IMU imu;
Magnetometer magnetometer;

extern "C" void App_Init(SPI_HandleTypeDef* hspi1, SPI_HandleTypeDef* hspi2)
{
    imu.Setup(hspi1, IMU_CS_GPIO_Port, IMU_CS_Pin);
    imu.Read_Data();
	magnetometer.Setup(hspi2, MAGNETOMETER_CS_GPIO_Port, MAGNETOMETER_CS_Pin);
	magnetometer.Read_Data();
}

extern "C" void App_Process(void)
{
    // Your application logic
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1)
    {
        imu.DMA_TxRxCompleteCallback();
    }
    if (hspi->Instance == SPI2)
    {
        magnetometer.DMA_TxRxCompleteCallback();
    }
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1)
    {
        imu.DMA_ErrorCallback();
    }
    if (hspi->Instance == SPI2)
    {
    	magnetometer.DMA_ErrorCallback();
    }
}

