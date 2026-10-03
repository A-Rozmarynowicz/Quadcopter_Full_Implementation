#include "STM_Communication.hpp"

Queue<I2C_Packet, I2C_RECEIVE_QUEUE_SIZE> i2c_receive_queue;
Queue<I2C_Packet, I2C_TRANSMIT_QUEUE_SIZE> i2c_transmit_queue;

bool Initialize_I2C()
{
    return Wire.begin(SDA_PIN, SCL_PIN);
}

bool Send_I2C()
{
    I2C_Packet packet{};

    if (!i2c_transmit_queue.pop(packet))
    {
        return false;
    }

    Wire.beginTransmission(STM_I2C_ADDRESS);

    Wire.write(packet.data, packet.length);

    uint8_t error = Wire.endTransmission();

    return error == 0;
}

namespace I2C_MESSAGES
{
    bool I2C_MESSAGES::Send_UWB_Ready()
        {
            I2C_Packet packet{};

            packet.data[I2C_Data_Setup::COMMAND] = UWB_Ready;
            packet.length = 1;

            return i2c_transmit_queue.push(packet);
        }
}