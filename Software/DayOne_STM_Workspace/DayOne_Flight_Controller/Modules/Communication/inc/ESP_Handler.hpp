#ifndef COMMUNICATION_INC_ESP_HANDLER_HPP_
#define COMMUNICATION_INC_ESP_HANDLER_HPP_

#include <cstdint>
#include "stm32f4xx_hal.h"

constexpr uint8_t I2C_RX_PACKET_SIZE = 16;
constexpr uint8_t I2C_TX_PACKET_SIZE = 28;

class ESP_Handler
{
private:
	I2C_HandleTypeDef* hi2c;
	GPIO_TypeDef* esp_request_port;
	uint16_t esp_request_pin;

	uint8_t rx_buffer[I2C_RX_PACKET_SIZE];
	uint8_t tx_buffer[I2C_TX_PACKET_SIZE];
public:
	void Setup(I2C_HandleTypeDef* i2c_handle, GPIO_TypeDef* GPIO_port_esp_request, uint16_t GPIO_pin_esp_request);
	void Initialize();

	void I2C_Listen_Callback(uint8_t TransferDirection);
	void I2C_Slave_RX_Callback();
	void I2C_Slave_TX_Callback();
};



#endif /* COMMUNICATION_INC_ESP_HANDLER_HPP_ */
