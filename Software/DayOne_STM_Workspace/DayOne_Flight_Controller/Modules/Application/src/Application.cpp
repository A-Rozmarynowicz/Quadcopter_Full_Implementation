#include "Application/inc/Application.hpp"
#include "main.h"

IMU imu;
Magnetometer magnetometer;
ESP_Handler esp_handler;

extern "C" void App_Init(SPI_HandleTypeDef* hspi1, SPI_HandleTypeDef* hspi2, I2C_HandleTypeDef* i2c1)
{
//    imu.Setup(hspi1, IMU_CS_GPIO_Port, IMU_CS_Pin);
//    SENSOR_STATUS code = imu.Initialize(IMU_UPDATE_RATES::Hz_833, IMU_ACC_SCALE::G_8, IMU_GYRO_SCALE::DPS_500);
//    imu.Prepare_For_Measurements();
//	magnetometer.Setup(hspi2, MAGNETOMETER_CS_GPIO_Port, MAGNETOMETER_CS_Pin);
//	HAL_StatusTypeDef code1 = magnetometer.Reset_Chip();
//	HAL_Delay(15);
//	SENSOR_STATUS code2 = magnetometer.Initialize(MAGNETOMETER_UPDATE_RATES::Hz_580);

	esp_handler.Setup(i2c1, ESP_REQUEST_GPIO_Port, ESP_REQUEST_Pin);
	esp_handler.Initialize();
}

extern "C" void App_Process(void)
{
    // Your application logic
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1)
    {
        imu.DMA_TxCompleteCallback();
    }
    if (hspi->Instance == SPI2)
    {
        magnetometer.DMA_TxCompleteCallback();
    }
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
		magnetometer.Data_Ready_EXTI_Handler();
	}
}


void HAL_I2C_AddrCallback(
    I2C_HandleTypeDef *hi2c,
    uint8_t TransferDirection,
    uint16_t AddrMatchCode)
{
	if (hi2c->Instance == I2C1)
	{
		esp_handler.I2C_Listen_Callback(TransferDirection);
	}
}

#pragma region I2C
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1)
    {
        esp_handler.I2C_Slave_RX_Callback();
    }
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1)
    {
        esp_handler.I2C_Slave_TX_Callback();
    }
}


void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
    HAL_I2C_EnableListen_IT(hi2c);
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    uint32_t error = HAL_I2C_GetError(hi2c);

    // Inspect error and perform appropriate recovery.
}

#pragma endregion

uint8_t imu_counter = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
//	magnetometer.Task_Perform_Transaction();
//	if (magnetometer.Is_Data_Ready())
//	{
//		Magnetometer_Measurement meas;
//		bool measurement_ok = magnetometer.Get_Measurement(meas);
//		HAL_GPIO_TogglePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin);
//	}
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

