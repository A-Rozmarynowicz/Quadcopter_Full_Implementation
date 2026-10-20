
#include "STM_Communication.hpp"

#include <cstring>

Queue<I2C_Packet, I2C_RECEIVE_QUEUE_SIZE> i2c_receive_queue;
Queue<I2C_Packet, I2C_TRANSMIT_QUEUE_SIZE> i2c_transmit_queue;


bool Initialize_I2C()
{
    if (!Wire.begin(SDA_PIN, SCL_PIN, 100000UL))
    {
        return false;
    }

    Wire.setTimeOut(I2C_TIMEOUT_MS);

    return true;
}


bool Send_I2C()
{
    I2C_Packet packet{};

    if (!i2c_transmit_queue.pop(packet))
    {
        return false;
    }

    if (packet.length == 0 || packet.length > I2C_DATA_SIZE)
    {
        return false;
    }

    const uint8_t command =
        packet.data[
            static_cast<std::size_t>(I2C_Data_Setup::COMMAND)
        ];

    Wire.beginTransmission(STM_I2C_ADDRESS);

    const size_t written = Wire.write(
        packet.data,
        packet.length
    );

    const uint8_t error = Wire.endTransmission();

    if (error != 0 || written != packet.length)
    {
        i2c_transmit_queue.push(packet);
        return false;
    }


    if (command != REQUEST_FULL_TELEMETRY)
    {
        return true;
    }

    // Here we only handle receiving answers.
    const size_t received = Wire.requestFrom(
        static_cast<uint8_t>(STM_I2C_ADDRESS),
        static_cast<size_t>(I2C_TELEMETRY_SIZE)
    );

    if (received != I2C_TELEMETRY_SIZE)
    {
        // Discard any partial response.
        while (Wire.available() > 0)
        {
            Wire.read();
        }

        return false;
    }

    I2C_Packet response{};
    response.length = I2C_TELEMETRY_SIZE;

    for (std::size_t i = 0; i < I2C_TELEMETRY_SIZE; ++i)
    {
        if (Wire.available() <= 0)
        {
            while (Wire.available() > 0)
            {
                Wire.read();
            }
            return false;
        }

        response.data[i] = static_cast<uint8_t>(Wire.read());
    }

    if (!i2c_receive_queue.push(response))
    {
        return false;
    }

    return true;
}


// =============================================================
// Outgoing message helpers
// =============================================================

bool I2C_MESSAGES::Send_UWB_Ready()
{
    I2C_Packet packet{};

    packet.data[
        static_cast<std::size_t>(I2C_Data_Setup::COMMAND)
    ] = UWB_READY;

    packet.length = 1;

    return i2c_transmit_queue.push(packet);
}


bool I2C_MESSAGES::Send_Current_Position(Position &position)
{
    I2C_Packet packet{};

    packet.data[
        static_cast<std::size_t>(I2C_Data_Setup::COMMAND)
    ] = CURRENT_POSITION_TRANSFER;

    std::memcpy(
        &packet.data[static_cast<std::size_t>(I2C_Data_Setup::QUAD_0)],
        &position.x,
        sizeof(float)
    );

    std::memcpy(
        &packet.data[static_cast<std::size_t>(I2C_Data_Setup::QUAD_1)],
        &position.y,
        sizeof(float)
    );

    std::memcpy(
        &packet.data[static_cast<std::size_t>(I2C_Data_Setup::QUAD_2)],
        &position.z,
        sizeof(float)
    );

    packet.length = 16;

    return i2c_transmit_queue.push(packet);
}


bool I2C_MESSAGES::Send_Heartbeat()
{
    I2C_Packet packet{};

    packet.data[
        static_cast<std::size_t>(I2C_Data_Setup::COMMAND)
    ] = HEARTBEAT;

    packet.length = 1;

    return i2c_transmit_queue.push(packet);
}


bool I2C_MESSAGES::Send_Emergency_Total_Stop()
{
    I2C_Packet packet{};

    packet.data[
        static_cast<std::size_t>(I2C_Data_Setup::COMMAND)
    ] = EMERGENCY_TOTAL_STOP;

    packet.length = 1;

    return i2c_transmit_queue.push(packet);
}


bool I2C_MESSAGES::Send_Emergency_Landing()
{
    I2C_Packet packet{};

    packet.data[
        static_cast<std::size_t>(I2C_Data_Setup::COMMAND)
    ] = EMERGENCY_LANDING;

    packet.length = 1;

    return i2c_transmit_queue.push(packet);
}


bool I2C_MESSAGES::Send_Hover(float height)
{
    I2C_Packet packet{};

    packet.data[
        static_cast<std::size_t>(I2C_Data_Setup::COMMAND)
    ] = HOVER;

    std::memcpy(
        &packet.data[static_cast<std::size_t>(I2C_Data_Setup::QUAD_0)],
        &height,
        sizeof(float)
    );

    packet.length = 8;

    return i2c_transmit_queue.push(packet);
}


bool I2C_MESSAGES::Send_Land()
{
    I2C_Packet packet{};

    packet.data[
        static_cast<std::size_t>(I2C_Data_Setup::COMMAND)
    ] = LAND;

    packet.length = 1;

    return i2c_transmit_queue.push(packet);
}


// =============================================================
// Telemetry request
// =============================================================

bool I2C_MESSAGES::Request_Full_Telemetry()
{
    I2C_Packet packet{};

    packet.data[
        static_cast<std::size_t>(I2C_Data_Setup::COMMAND)
    ] = REQUEST_FULL_TELEMETRY;

    // The command is one byte. Send_I2C() recognizes it and
    // follows the write with a read of the complete 28-byte reply.
    packet.length = 1;

    return i2c_transmit_queue.push(packet);
}
