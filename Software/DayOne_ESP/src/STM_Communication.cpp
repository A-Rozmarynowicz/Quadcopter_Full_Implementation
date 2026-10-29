
#include "STM_Communication.hpp"

#include <cstring>

Queue<I2C_Packet<I2C_RX_DATA_SIZE>, I2C_RECEIVE_QUEUE_SIZE> i2c_receive_queue;
Queue<I2C_Packet<I2C_TX_DATA_SIZE>, I2C_TRANSMIT_QUEUE_SIZE> i2c_transmit_queue;


bool Initialize_I2C()
{
    if (!Wire.begin(SDA_PIN, SCL_PIN, 100000UL))
    {
        return false;
    }

    Wire.setTimeOut(I2C_TIMEOUT_MS);

    return true;
}

void Flush_Unused_STM_Received_Packets()
{
  for (uint8_t i = 0; i<i2c_receive_queue.size(); i++)
  {
    if (i2c_receive_queue[i2c_receive_queue.get_head_offset_index(i)].Is_For_Flush())
    {
      I2C_Packet<I2C_RX_DATA_SIZE> proxy;
      i2c_receive_queue.pop(proxy);
    }
    else
    {
      break;
    }
  }
};

bool Send_I2C()
{
    I2C_Packet<I2C_TX_DATA_SIZE> packet{};

    if (!i2c_transmit_queue.pop(packet))
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

    switch (command)
    {
    case REQUEST_FULL_TELEMETRY:
        return _read_I2C_full_telemetry();
    }

    return true;
}


bool _read_I2C_full_telemetry()
{
    const size_t received = Wire.requestFrom(
        static_cast<uint8_t>(STM_I2C_ADDRESS),
        static_cast<size_t>(I2C_RX_DATA_SIZE)
    );

    if (received != I2C_RX_DATA_SIZE)
    {
        // Discard any partial response.
        while (Wire.available() > 0)
        {
            Wire.read();
        }

        return false;
    }

    I2C_Packet<I2C_RX_DATA_SIZE> response{};

    for (std::size_t i = 0; i < I2C_RX_DATA_SIZE; ++i)
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


#pragma region I2C Messages

bool I2C_MESSAGES::Send_Quadcopter_Init()
{
    I2C_Packet<I2C_TX_DATA_SIZE> packet{};

    packet.data[
        static_cast<std::size_t>(I2C_Data_Setup::COMMAND)
    ] = I2C_Data_Commands::QUADCOPTER_INIT;

    return i2c_transmit_queue.push(packet);
}


bool I2C_MESSAGES::Send_Current_Position(Position &position)
{
    I2C_Packet<I2C_TX_DATA_SIZE> packet{};

    packet.data[
        static_cast<std::size_t>(I2C_Data_Setup::COMMAND)
    ] = I2C_Data_Commands::CURRENT_POSITION_TRANSFER;

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

    return i2c_transmit_queue.push(packet);
}


bool I2C_MESSAGES::Send_Heartbeat()
{
    I2C_Packet<I2C_TX_DATA_SIZE> packet{};

    packet.data[
        static_cast<std::size_t>(I2C_Data_Setup::COMMAND)
    ] = I2C_Data_Commands::HEARTBEAT;

    return i2c_transmit_queue.push(packet);
}


bool I2C_MESSAGES::Send_Emergency_Total_Stop()
{
    I2C_Packet<I2C_TX_DATA_SIZE> packet{};

    packet.data[
        static_cast<std::size_t>(I2C_Data_Setup::COMMAND)
    ] = I2C_Data_Commands::EMERGENCY_TOTAL_STOP;

    return i2c_transmit_queue.push(packet);
}


bool I2C_MESSAGES::Send_Emergency_Landing()
{
    I2C_Packet<I2C_TX_DATA_SIZE> packet{};

    packet.data[
        static_cast<std::size_t>(I2C_Data_Setup::COMMAND)
    ] = I2C_Data_Commands::EMERGENCY_LANDING;

    return i2c_transmit_queue.push(packet);
}


bool I2C_MESSAGES::Send_Hover(float height)
{
    I2C_Packet<I2C_TX_DATA_SIZE> packet{};

    packet.data[
        static_cast<std::size_t>(I2C_Data_Setup::COMMAND)
    ] = I2C_Data_Commands::HOVER;

    std::memcpy(
        &packet.data[static_cast<std::size_t>(I2C_Data_Setup::QUAD_0)],
        &height,
        sizeof(float)
    );

    return i2c_transmit_queue.push(packet);
}


bool I2C_MESSAGES::Send_Land()
{
    I2C_Packet<I2C_TX_DATA_SIZE> packet{};

    packet.data[
        static_cast<std::size_t>(I2C_Data_Setup::COMMAND)
    ] = I2C_Data_Commands::LAND;

    return i2c_transmit_queue.push(packet);
}

bool I2C_MESSAGES::Request_Full_Telemetry()
{
    I2C_Packet<I2C_TX_DATA_SIZE> packet{};

    packet.data[
        static_cast<std::size_t>(I2C_Data_Setup::COMMAND)
    ] = I2C_Data_Commands::REQUEST_FULL_TELEMETRY;

    return i2c_transmit_queue.push(packet);
}

bool I2C_MESSAGES::Send_Reset_STM()
{
    I2C_Packet<I2C_TX_DATA_SIZE> packet{};

    packet.data[
        static_cast<std::size_t>(I2C_Data_Setup::COMMAND)
    ] = I2C_Data_Commands::RESET_STM;

    return i2c_transmit_queue.push(packet);
}

#pragma endregion