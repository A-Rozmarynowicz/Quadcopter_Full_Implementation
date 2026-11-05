#ifndef SENSOR_SRC_HPP
#define SENSOR_SRC_HPP

template<size_t BUFFER_SIZE>
void Sensor<BUFFER_SIZE>::Setup(SPI_BUS_STRUCTURE &spi_bus)
{
	spi = spi_bus.hspi;
	cs_port = spi_bus.cs_port;
	cs_pin = spi_bus.cs_pin;
}

template<size_t BUFFER_SIZE>
void Sensor<BUFFER_SIZE>::Change_Buffer_Pointers()
{
	if (current_receive_buffer == receive_buffer_1)
	{
		current_receive_buffer = receive_buffer_2;
		current_transmit_buffer = transmit_buffer_2;
	}
	else
	{
		current_receive_buffer = receive_buffer_1;
		current_transmit_buffer = transmit_buffer_1;
	}
}

template<size_t BUFFER_SIZE>
bool Sensor<BUFFER_SIZE>::Is_Data_Ready()
{
	return data_ready;
}

template<size_t BUFFER_SIZE>
uint8_t Sensor<BUFFER_SIZE>::Get_Write_Command(uint8_t message)
{
	return message & 0x7F;
}

template<size_t BUFFER_SIZE>
uint8_t Sensor<BUFFER_SIZE>::Get_Read_Command(uint8_t message)
{
	return message | 0x80;
}

#endif
