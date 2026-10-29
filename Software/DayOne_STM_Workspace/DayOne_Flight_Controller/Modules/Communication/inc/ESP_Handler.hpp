/*
 * ESP_Handler.h
 *
 *  Created on: Oct 29, 2026
 *      Author: arekr
 */

#ifndef COMMUNICATION_INC_ESP_HANDLER_HPP_
#define COMMUNICATION_INC_ESP_HANDLER_HPP_


class ESP_Handler
{
private:
	I2C_HandleTypeDef* hi2c;
	GPIO_TypeDef* esp_request_port;
	uint16_t esp_request_pin;
};


#endif /* COMMUNICATION_INC_ESP_HANDLER_HPP_ */
