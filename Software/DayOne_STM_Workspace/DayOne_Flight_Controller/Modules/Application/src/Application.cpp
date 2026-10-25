#include "Application/inc/Application.hpp"
#include "main.h"

IMU imu;
Magnetometer magnetometer;

extern "C" void App_Init(SPI_HandleTypeDef* hspi1, SPI_HandleTypeDef* hspi2)
{
//    imu.Setup(hspi1, IMU_CS_GPIO_Port, IMU_CS_Pin);
//    SENSOR_STATUS code = imu.Initialize(IMU_UPDATE_RATES::Hz_833, IMU_ACC_SCALE::G_8, IMU_GYRO_SCALE::DPS_500);
//    imu.Prepare_For_Measurements();

	magnetometer.Setup(hspi2, MAGNETOMETER_CS_GPIO_Port, MAGNETOMETER_CS_Pin);
	SENSOR_STATUS code = magnetometer.Initialize(MAGNETOMETER_UPDATE_RATES::Hz_400);
	magnetometer.Prepare_For_Measurements();
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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == MAGNETOMETER_INTERRUPT_Pin)
	{
		HAL_GPIO_TogglePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin);
		magnetometer.Data_Ready_EXTI_Handler();
	}
}

uint8_t imu_counter = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	magnetometer.Task_Perform_Transaction();
//	if (imu_counter == 0)
//	{
//		imu_counter = 1;
//		imu.Task_Perform_Transaction();
//		HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_SET);
//	}
//	else
//	{
//		IMU_Measurement imu_meas;
//		bool ok = imu.Get_Measurement(imu_meas);
//		if (!ok)
//		{
//			uint8_t check = 2 + 5;
//			return;
//		}
//		HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_RESET);
//		imu_counter = 0;
//		imu.Measurement_Cycle_Finished();
//	}
}

