#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <cstdint>
#include "stm32f4xx_hal.h"

template <size_t BUFFER_SIZE>
class Sensor
{
protected:
    SPI_HandleTypeDef* spi;
    GPIO_TypeDef* cs_port;
    uint16_t cs_pin;

    uint8_t transmit_buffer_1[BUFFER_SIZE];
    uint8_t receive_buffer_1[BUFFER_SIZE];
    uint8_t transmit_buffer_2[BUFFER_SIZE];
    uint8_t receive_buffer_2[BUFFER_SIZE];

    uint8_t* current_transmit_buffer = transmit_buffer_1;
    uint8_t* current_receive_buffer = receive_buffer_1;

    bool data_ready = false;

    virtual void Read_ID_Register() = 0;
    virtual uint8_t Get_Write_Command(uint8_t message);
    virtual uint8_t Get_Read_Command(uint8_t message);

public:
    Sensor() = default;
    virtual ~Sensor() = default;

    void Setup(
        SPI_HandleTypeDef* hspi,
        GPIO_TypeDef* cs_port_spi,
        uint16_t cs_pin_spi
    );

    virtual void DMA_TxRxCompleteCallback() = 0;
    virtual void DMA_ErrorCallback() = 0;

    virtual void Measurement_Cycle_Finished() = 0;
    virtual void Task_Perform_Transaction() = 0;

    virtual bool Is_Data_Ready();
};

#include "Sensors/src/Sensor_src.hpp"

#endif
