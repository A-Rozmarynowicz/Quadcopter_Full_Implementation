#include "Sensors/inc/Magnetometer.hpp"
#include "stm32f4xx_hal.h"


enum REGISTERS : uint8_t
{
	MEASUREMENT = 0x00,
	STATUS = 0x08,

	CONTROL_0 = 0x09,
	CONTROL_1 = 0x0A,

	PRODUCT_ID = 0x2F,
};

enum STATUS_COMMANDS : uint8_t
{
    OTP_RD_DONE = 0b0001'0000,
    MEAS_TEMP_DONE = 0b0000'0010,
    MEAS_MAGNETOMETER_DONE = 0b0000'0001,
};

enum CONTROL_0_COMMANDS : uint8_t
{
    INIT_TEMP_MEASUREMENT = 0b0000'0010,
    INIT_MAGNETOMETER_MEASUREMENT = 0b0000'0001,
    INTERRUPT = 0b0000'0100,
    SET = 0b0000'1000,
    RESET = 0b0001'0000,
};

enum CONTROL_1_COMMANDS : uint8_t
{
    BW_100HZ = 0b0000'0000,
    BW_200HZ = 0b0000'0001,
    BW_400HZ = 0b0000'0010,
    CHIP_RESET = 0b1000'0000,
};


constexpr uint8_t PRODUCT_ID_ADDRESS = 0x2F;
constexpr uint8_t MEASUREMENT_FIRST_ADDRESS = 0x00;
constexpr uint8_t STATUS_REG_ADDRESS = 0x08;


constexpr uint8_t TARGET_PRODUCT_ID = 0x30;

void Magnetometer::DMA_TxRxCompleteCallback()
{
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
}

void Magnetometer::DMA_ErrorCallback()
{
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
}

bool Magnetometer::Assert_ID_Register()
{
	return false;
}

void Magnetometer::Measurement_Cycle_Finished()
{
}

void Magnetometer::Prepare_For_Measurements() {
}

void Magnetometer::Task_Perform_Transaction()
{
//	HAL_SPI_TransmitReceive_DMA(hspi, current_transmit_buffer, current_receive_buffer, Size)
}
