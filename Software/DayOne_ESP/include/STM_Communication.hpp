#ifndef STM_COMMUNICATION_HPP
#define STM_COMMUNICATION_HPP


#include "Configuration.hpp"
#include <Wire.h>
#include "UWB.hpp"
#include "Buffers.hpp"

#define STM_I2C_ADDRESS 0x77
#define ESP_I2C_ADDRESS 0x12
#define I2C_DATA_SIZE 20

#define I2C_RECEIVE_QUEUE_SIZE 33
#define I2C_TRANSMIT_QUEUE_SIZE 33

#define SDA_PIN 25
#define SCL_PIN 26

enum I2C_Data_Commands
{
    UWB_READY,
    CURRENT_POSITION_TRANSFER,
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
};

struct I2C_Packet
{
    uint8_t data[I2C_DATA_SIZE];
    uint8_t length;
};

extern Queue<I2C_Packet, I2C_RECEIVE_QUEUE_SIZE> i2c_receive_queue;
extern Queue<I2C_Packet, I2C_TRANSMIT_QUEUE_SIZE> i2c_transmit_queue;

bool Initialize_I2C();
bool Send_I2C();

namespace I2C_MESSAGES
{
    bool Send_UWB_Ready();
    bool Send_Current_Position(Position &position);
};


#endif