#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#ifdef __cplusplus

#include "Sensors/inc/IMU.hpp"
#include "Sensors/inc/Magnetometer.hpp"

extern IMU imu;
extern Magnetometer magnetometer;

extern "C" {
#endif

void App_Init(SPI_HandleTypeDef* hspi1, SPI_HandleTypeDef* hspi2);
void App_Process(void);

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim);
#ifdef __cplusplus
}
#endif

#endif
