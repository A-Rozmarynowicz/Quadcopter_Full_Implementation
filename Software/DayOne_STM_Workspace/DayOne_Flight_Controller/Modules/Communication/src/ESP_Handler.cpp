#include "Communication/inc/ESP_Handler.hpp"

uint8_t proxy_buffer[8];

#pragma region Callbacks

void ESP_Handler::I2C_Listen_Callback(uint8_t TransferDirection)
{
	if (TransferDirection == I2C_DIRECTION_TRANSMIT)
	{
		// Master is transmitting TO the slave.
		HAL_I2C_Slave_Receive_DMA(
			hi2c, proxy_buffer, 8);
	}
	else
	{
		// Master is receiving FROM the slave.
		HAL_I2C_Slave_Transmit_DMA(
			hi2c, proxy_buffer, 8);
	}
}

void ESP_Handler::Setup(I2C_HandleTypeDef *i2c_handle,
		GPIO_TypeDef *GPIO_port_esp_request,
		uint16_t GPIO_pin_esp_request)
{
	hi2c = i2c_handle;
	esp_request_port = GPIO_port_esp_request;
	esp_request_pin = GPIO_pin_esp_request;
}

void ESP_Handler::I2C_Slave_RX_Callback() {
}

void ESP_Handler::I2C_Slave_TX_Callback() {
}

void ESP_Handler::Initialize()
{
	HAL_I2C_EnableListen_IT(hi2c);
}

#pragma endregion
