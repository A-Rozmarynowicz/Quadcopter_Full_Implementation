#ifndef IMU_HPP
#define IMU_HPP

#include "stm32f4xx_hal.h"
#include "Sensor.hpp"
#include <cstdint>
#include "Configuration/inc/Sensor_Settings.hpp"

constexpr uint8_t IMU_BUFFER_SIZE = 13;

struct IMU_Measurement
{
	float acc_x, acc_y, acc_z;
	float gyro_x, gyro_y, gyro_z;
};

class IMU: public Sensor<IMU_BUFFER_SIZE>
{
private:
	IMU_UPDATE_RATES_ENUM update_rate;
	IMU_ACC_SCALES_ENUM acc_scale;
	IMU_GYRO_SCALES_ENUM gyro_scale;
	float acc_sensitivity_g;
	float gyro_sensitivity_dps;
	IMU_Measurement current_measurement;

	void Update_Sensitivities();

	HAL_StatusTypeDef Handle_Acc_Control_Registers();
	HAL_StatusTypeDef Handle_Gyro_Control_Registers();
	bool Parse_Measurement(uint8_t *received_data, uint8_t size);
protected:
	bool Assert_ID_Register() override;
public:
	SENSOR_STATUS Initialize(
			IMU_UPDATE_RATES_ENUM p_update_rate,
			IMU_ACC_SCALES_ENUM p_acc_scale,
			IMU_GYRO_SCALES_ENUM p_gyro_scale);

	void DMA_TxRxCompleteCallback() override;
	void DMA_TxCompleteCallback() override;
	void DMA_ErrorCallback() override;

	HAL_StatusTypeDef Reset_Chip() override;
	void Prepare_For_Measurements() override;
	void Measurement_Cycle_Finished() override;
	void Task_Perform_Transaction() override;

	bool Get_Measurement(IMU_Measurement &meas);
};

#endif
