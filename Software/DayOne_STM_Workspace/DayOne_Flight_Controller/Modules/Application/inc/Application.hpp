#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#ifdef __cplusplus

#include "Communication/inc/ESP_Handler.hpp"
#include "State_Estimation/inc/State_Estimator.hpp"

extern State_Estimator state_estimator;
extern ESP_Handler esp_handler;

extern "C"
{
#endif

void App_Init(
		SPI_HandleTypeDef *hspi1,
		SPI_HandleTypeDef *hspi2,
		I2C_HandleTypeDef *i2c1);
void App_Process(void);

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
#ifdef __cplusplus
}
#endif

#endif
