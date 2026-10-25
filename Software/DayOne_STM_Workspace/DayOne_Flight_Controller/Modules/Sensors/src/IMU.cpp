#include "Sensors/inc/IMU.hpp"
#include "stm32f4xx_hal.h"

enum REGISTERS : uint8_t
{
	IMU_WHOAMI_REG = 0x0F, // Product ID
	IMU_CTRL1_ACC_REG = 0x10, // Accelerometer settings
	IMU_CTRL2_GYRO_REG = 0x11, // Gyroscope settings

	IMU_TEMP_REG = 0x20, // Temperature read register
	IMU_MEASUREMENT_REG = 0x22, // Temperature read register
};

constexpr uint8_t TARGET_WHOAMI_VALUE = 0x6c;

enum MEASUREMENT_DATA_LAYOUT
{
	OUTX_L_G = 1,
	OUTX_H_G,
	OUTY_L_G,
	OUTY_H_G,
	OUTZ_L_G,
	OUTZ_H_G,

	OUTX_L_A,
	OUTX_H_A,
	OUTY_L_A,
	OUTY_H_A,
	OUTZ_L_A,
	OUTZ_H_A,
};

SENSOR_STATUS IMU::Initialize(IMU_UPDATE_RATES p_update_rate, IMU_ACC_SCALE p_acc_scale,
		IMU_GYRO_SCALE p_gyro_scale)
{
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);

	update_rate = p_update_rate;
	acc_scale = p_acc_scale;
	gyro_scale = p_gyro_scale;

	bool id_ok = Assert_ID_Register();
	HAL_StatusTypeDef acc_code = Handle_Acc_Control_Registers();
	HAL_StatusTypeDef gyro_code = Handle_Gyro_Control_Registers();
	Update_Sensitivities();

	if (acc_code == HAL_OK && gyro_code == HAL_OK && id_ok == true)
	{
		return SENSOR_STATUS::INIT_OK;
	}
	return SENSOR_STATUS::INIT_FAIL;
}

void IMU::DMA_TxRxCompleteCallback()
{
	Parse_Measurement(current_receive_buffer, IMU_BUFFER_SIZE);
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
}

void IMU::DMA_ErrorCallback()
{
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
}

HAL_StatusTypeDef IMU::Handle_Acc_Control_Registers()
{
	current_transmit_buffer[0] = Get_Write_Command(REGISTERS::IMU_CTRL1_ACC_REG);
	current_transmit_buffer[1] = static_cast<uint8_t>(update_rate) | static_cast<uint8_t>(acc_scale);

	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);
	HAL_StatusTypeDef code = HAL_SPI_Transmit(spi, current_transmit_buffer, IMU_BUFFER_SIZE, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);

	Change_Buffer_Pointers();

	return code;
}


HAL_StatusTypeDef IMU::Handle_Gyro_Control_Registers()
{
	current_transmit_buffer[0] = Get_Write_Command(REGISTERS::IMU_CTRL2_GYRO_REG);
	current_transmit_buffer[1] = static_cast<uint8_t>(update_rate) | static_cast<uint8_t>(gyro_scale);

	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);
	HAL_StatusTypeDef code = HAL_SPI_Transmit(spi, current_transmit_buffer, IMU_BUFFER_SIZE, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);

	Change_Buffer_Pointers();

	return code;
}

bool IMU::Assert_ID_Register()
{
	current_transmit_buffer[0] = Get_Read_Command(REGISTERS::IMU_WHOAMI_REG);

	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);
	HAL_StatusTypeDef code = HAL_SPI_TransmitReceive(spi, current_transmit_buffer, current_receive_buffer,
			IMU_BUFFER_SIZE, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);

	bool ok = current_receive_buffer[1] == TARGET_WHOAMI_VALUE;

	Change_Buffer_Pointers();

	return (code == HAL_OK) && (ok == true);
}

void IMU::Prepare_For_Measurements()
{
	for (uint8_t i = 0; i<IMU_BUFFER_SIZE; i++)
	{
		transmit_buffer_1[i] = 0;
		transmit_buffer_2[i] = 0;
	}
	transmit_buffer_1[0] = Get_Read_Command(REGISTERS::IMU_MEASUREMENT_REG);
	transmit_buffer_2[0] = Get_Read_Command(REGISTERS::IMU_MEASUREMENT_REG);
}

void IMU::Measurement_Cycle_Finished()
{
	Change_Buffer_Pointers();
}

bool IMU::Parse_Measurement(uint8_t* received_data, uint8_t size) // TODO: Maybe FPU
{
	if (size != IMU_BUFFER_SIZE)
	{
		return false;
	}

	int16_t raw_A_x = (int16_t) (received_data[OUTX_H_A] << 8 | received_data[OUTX_L_A]);
	int16_t raw_A_y = (int16_t) (received_data[OUTY_H_A] << 8 | received_data[OUTY_L_A]);
	int16_t raw_A_z = (int16_t) (received_data[OUTZ_H_A] << 8 | received_data[OUTZ_L_A]);

	int16_t raw_G_x = (int16_t) (received_data[OUTX_H_G] << 8 | received_data[OUTX_L_G]);
	int16_t raw_G_y = (int16_t) (received_data[OUTY_H_G] << 8 | received_data[OUTY_L_G]);
	int16_t raw_G_z = (int16_t) (received_data[OUTZ_H_G] << 8 | received_data[OUTZ_L_G]);

	current_measurement.acc_x = (float)raw_A_x * acc_sensitivity_g;
	current_measurement.acc_y = (float)raw_A_y * acc_sensitivity_g;
	current_measurement.acc_z = (float)raw_A_z * acc_sensitivity_g;

	current_measurement.gyro_x = (float)raw_G_x * gyro_sensitivity_dps;
	current_measurement.gyro_y = (float)raw_G_y * gyro_sensitivity_dps;
	current_measurement.gyro_z = (float)raw_G_z * gyro_sensitivity_dps;

	data_ready = true;

	return true;
}

void IMU::Update_Sensitivities()
{
	switch (acc_scale)
	{
		case IMU_ACC_SCALE::G_8:
			acc_sensitivity_g = 0.244f * 0.001f;
			break;
		case IMU_ACC_SCALE::G_16:
			acc_sensitivity_g = 0.488f * 0.001;
			break;
	}
	switch (gyro_scale)
	{
		case IMU_GYRO_SCALE::DPS_500:
			gyro_sensitivity_dps = 17.50f * 0.001;
			break;
		case IMU_GYRO_SCALE::DPS_1000:
			gyro_sensitivity_dps = 35.0f * 0.001;
			break;
	}

}


void IMU::Task_Perform_Transaction()
{
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive_DMA(spi, current_transmit_buffer, current_receive_buffer, IMU_BUFFER_SIZE);
}

bool IMU::Get_Measurement(IMU_Measurement &meas)
{
	if (data_ready)
	{
		meas = current_measurement;
		data_ready = false;
		return true;
	}
	return false;
}
