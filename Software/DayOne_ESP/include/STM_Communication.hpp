
#ifndef STM_COMMUNICATION_HPP
#define STM_COMMUNICATION_HPP

#include "Configuration.hpp"
#include <Wire.h>
#include "UWB.hpp"
#include "Buffers.hpp"

#include <cstddef>
#include <cstdint>

#define STM_I2C_ADDRESS 0x67
#define ESP_I2C_ADDRESS 0x12

#define I2C_TX_DATA_SIZE 16
#define I2C_RX_DATA_SIZE 28

#define I2C_RECEIVE_QUEUE_SIZE 33
#define I2C_TRANSMIT_QUEUE_SIZE 33

#define SDA_PIN 25
#define SCL_PIN 26

#define STM_GPIO_PIN 14

constexpr uint16_t I2C_TIMEOUT_MS = 4;

enum I2C_Data_Commands
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

template <std::size_t N>
struct I2C_Packet
{
    uint8_t data[N];
    uint8_t length = N;
    uint8_t flush_value;
    static constexpr uint8_t max_flush_value = 8;
    void Mark_To_Flush(){flush_value=max_flush_value;}
    void Increase_Flush_Value()
      {if (flush_value >= max_flush_value){return;}
      flush_value++;}
    uint8_t Get_Flush_Value(){return flush_value;}
    bool Is_For_Flush(){return flush_value>=max_flush_value;}
};

extern Queue<I2C_Packet<I2C_RX_DATA_SIZE>, I2C_RECEIVE_QUEUE_SIZE>
    i2c_receive_queue;

extern Queue<I2C_Packet<I2C_TX_DATA_SIZE>, I2C_TRANSMIT_QUEUE_SIZE>
    i2c_transmit_queue;

bool Initialize_I2C();

bool Send_I2C();
void Flush_Unused_STM_Received_Packets();

namespace I2C_MESSAGES
{
    bool Send_Quadcopter_Init();
    bool Send_Current_Position(Position &position);
    bool Send_Heartbeat();
    bool Send_Emergency_Total_Stop();
    bool Send_Emergency_Landing();
    bool Send_Hover(float height);
    bool Send_Land();

    bool Request_Full_Telemetry();
    bool Send_Reset_STM();
}

bool _read_I2C_full_telemetry();

void Task_1ms();
void Task_5ms();
void Task_20ms();
void Task_100ms();
void Task_1000ms();

#endif
