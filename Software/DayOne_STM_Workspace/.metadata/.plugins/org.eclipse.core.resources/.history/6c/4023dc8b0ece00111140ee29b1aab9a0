#include "Sensors/inc/Sensor.hpp"



Sensor::Sensor(SPI_HandleTypeDef& hspi,
        GPIO_TypeDef* cs_port_spi,
        uint16_t cs_pin_spi) : spi(hspi), cs_port(cs_pin_spi), cs_pin(cs_pin_spi)
{}

SPI_HandleTypeDef& Sensor::Get_SPI()
{
	return spi;
}

