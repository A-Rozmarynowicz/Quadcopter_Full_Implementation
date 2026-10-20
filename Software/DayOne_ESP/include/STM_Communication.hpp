
#ifndef STM_COMMUNICATION_HPP
#define STM_COMMUNICATION_HPP

#include "Configuration.hpp"
#include <Wire.h>
#include "UWB.hpp"
#include "Buffers.hpp"

#include <cstddef>
#include <cstdint>

#define STM_I2C_ADDRESS 0x77
#define ESP_I2C_ADDRESS 0x12

// Complete packet: command + 3 single bytes + 6 floats.
#define I2C_DATA_SIZE 28
#define I2C_TELEMETRY_SIZE I2C_DATA_SIZE

#define I2C_RECEIVE_QUEUE_SIZE 33
#define I2C_TRANSMIT_QUEUE_SIZE 33

#define SDA_PIN 25
#define SCL_PIN 26

#define STM_GPIO_PIN 14

constexpr uint16_t I2C_TIMEOUT_MS = 10;

enum I2C_Data_Commands
{
    UWB_READY,
    CURRENT_POSITION_TRANSFER,
    HEARTBEAT,
    EMERGENCY_TOTAL_STOP,
    EMERGENCY_LANDING,
    HOVER,
    LAND,

    REQUEST_FULL_TELEMETRY,
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

struct I2C_Packet
{
    uint8_t data[I2C_DATA_SIZE]{};
    uint8_t length = 0;
};

extern Queue<I2C_Packet, I2C_RECEIVE_QUEUE_SIZE>
    i2c_receive_queue;

extern Queue<I2C_Packet, I2C_TRANSMIT_QUEUE_SIZE>
    i2c_transmit_queue;

bool Initialize_I2C();

bool Send_I2C();

namespace I2C_MESSAGES
{
    bool Send_UWB_Ready();
    bool Send_Current_Position(Position &position);
    bool Send_Heartbeat();
    bool Send_Emergency_Total_Stop();
    bool Send_Emergency_Landing();
    bool Send_Hover(float height);
    bool Send_Land();

    bool Request_Full_Telemetry();
}


void Task_1ms();
void Task_5ms();
void Task_20ms();
void Task_100ms();
void Task_1000ms();

#endif
