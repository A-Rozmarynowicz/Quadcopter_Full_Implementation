#include "Communication/inc/ESP_Handler.hpp"
#include <cstring>

enum class I2C_Data_Commands
{
    QUADCOPTER_INIT,
    CURRENT_POSITION_TRANSFER,
    HEARTBEAT,
    EMERGENCY_TOTAL_STOP,
    EMERGENCY_LANDING,
    HOVER,
    LAND,

    REQUEST_FULL_TELEMETRY,
    ANSWER_FULL_TELEMETRY,

    RESET_STM,
};

enum class I2C_Data_Setup
{
    COMMAND = 0,

    SINGLE_0 = 1,
    SINGLE_1 = 2,
    SINGLE_2 = 3,

    QUAD_0 = 4,
    QUAD_1 = 8,
    QUAD_2 = 12,
    QUAD_3 = 16,
    QUAD_4 = 20,
    QUAD_5 = 24,
};

void ESP_Handler::Setup(I2C_HandleTypeDef *i2c_handle,
		GPIO_TypeDef *GPIO_port_esp_request,
		uint16_t GPIO_pin_esp_request)
{
	hi2c = i2c_handle;
	esp_request_port = GPIO_port_esp_request;
	esp_request_pin = GPIO_pin_esp_request;
}

void ESP_Handler::Initialize()
{
	HAL_I2C_EnableListen_IT(hi2c);
}

#pragma region Callbacks

void ESP_Handler::I2C_Listen_Callback(uint8_t TransferDirection)
{
    HAL_StatusTypeDef status;

    if (TransferDirection == I2C_DIRECTION_TRANSMIT)
    {
        // ESP32 writes to STM32.
        status = HAL_I2C_Slave_Seq_Receive_DMA(
            hi2c,
            rx_buffer,
            I2C_RX_PACKET_SIZE,
            I2C_FIRST_AND_LAST_FRAME
        );
    }
    else
    {
        // ESP32 reads from STM32.
        status = HAL_I2C_Slave_Seq_Transmit_DMA(
            hi2c,
            tx_buffer,
            I2C_TX_PACKET_SIZE,
            I2C_FIRST_AND_LAST_FRAME
        );
    }

    if (status != HAL_OK)
    {
        uint8_t check = 1+2;
        HAL_I2C_EnableListen_IT(hi2c);
    }
}

void ESP_Handler::I2C_Slave_RX_Callback()
{
	I2C_Data_Commands command = static_cast<I2C_Data_Commands>(
			rx_buffer[static_cast<uint8_t>(I2C_Data_Setup::COMMAND)]
					  );

	float x, y, z;

	memcpy(&x, &rx_buffer[static_cast<uint8_t>(I2C_Data_Setup::QUAD_0)], sizeof(float));
	memcpy(&y, &rx_buffer[static_cast<uint8_t>(I2C_Data_Setup::QUAD_1)], sizeof(float));
	memcpy(&z, &rx_buffer[static_cast<uint8_t>(I2C_Data_Setup::QUAD_2)], sizeof(float));

	uint8_t check = 1+2;
}

void ESP_Handler::I2C_Slave_TX_Callback()
{
}



#pragma endregion
