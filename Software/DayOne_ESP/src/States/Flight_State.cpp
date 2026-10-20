#include "States.hpp"
#include "STM_Communication.hpp"
#include "UWB.hpp"
#include "UWB_Calculations.hpp"

void Flight_State::Enter()
{
    Reset_Requested_State();
    Enable_UWB();
};

void Flight_State::Exit()
{
    Reset_Requested_State();
}

void Flight_State::Task_1ms()
{

}

void Flight_State::Task_5ms()
{

}

void Flight_State::Task_10ms()
{

}

void Flight_State::Task_20ms()
{

}

void Flight_State::Task_100ms()
{
    Handle_Ready_Measurements();
}

void Flight_State::Task_1000ms()
{

}

void Flight_State::Handle_Ready_Measurements()
{
    Serial.printf("Present lighthouses: %d\n", Get_Number_Of_Present_Lighthouses(uwb_measurement_stacks));
    uint8_t penalty = Parse_Measurements();
    if (penalty == 0)
    {
        total_penalty = 0;
    }
    else
    {
        total_penalty += penalty;
    }
}

uint8_t Flight_State::Parse_Measurements()
{
    uint8_t penalty = Calculate_Position(uwb_measurement_stacks, current_calculated_position);
    if (penalty < MAX_MISSING_LGHS_PENALTY)
    {
        I2C_MESSAGES::Send_Current_Position(current_calculated_position);
        Serial.printf("SENT: x = %0.2f, y = %0.2f, z = %0.2f \n", current_calculated_position.x, current_calculated_position.y, current_calculated_position.z);
    }
    return penalty;
}


void Flight_State::Parse_ESP_Messages()
{
    if (esp_receive_queue.empty()) {return;}
    ESP_Packet packet;
    ESP_Data_Commands command = static_cast<ESP_Data_Commands>
                        (esp_receive_queue[esp_receive_queue.get_head_offset_index(0)].data[ESP_Data_Setup::COMMAND]);

    switch (command)
    {
    case ESP_Data_Commands::QUADCOPTER_EMERGENCY_TOTAL_STOP:
        esp_receive_queue.pop(packet);
        Request_Change_State(STATES::EMERGENCY_STATE);
        break;

    case ESP_Data_Commands::QUADCOPTER_HOVER:
        esp_receive_queue.pop(packet);
        float height;
        memcpy(&height, &(packet.data[ESP_Data_Setup::QUAD_0]), sizeof(float));
        I2C_MESSAGES::Send_Hover(height);
        break;

    case ESP_Data_Commands::QUADCOPTER_LAND:
        esp_receive_queue.pop(packet);
        I2C_MESSAGES::Send_Land();
        break;

    case ESP_Data_Commands::GROUND_STATION_ENABLE_FULL_TELEMETRY_REQUESTS:
        esp_receive_queue.pop(packet);
        full_telemetry_requests_enabled = true;
        break;

    default:
        esp_receive_queue[esp_receive_queue.get_head_offset_index(0)].Increase_Flush_Value();
    }

    return;
}

void Flight_State::Parse_STM_Telemetry()
{
    I2C_Packet response;
    if (!i2c_receive_queue.pop(response))
    {
        return;
    }
    if (response.length != I2C_TELEMETRY_SIZE)
    {
        return;
    }

    uint8_t single[3];

    float telemetry[6];

    // Extract three singles.
    for (std::size_t i = 0; i < 3; ++i)
    {
        const std::size_t offset =
            static_cast<std::size_t>(I2C_Data_Setup::SINGLE_0) + i;

        single[i] = response.data[offset];
    }

    // Extract six floats.
    for (std::size_t i = 0; i < 6; ++i)
    {
        const std::size_t offset =
            static_cast<std::size_t>(I2C_Data_Setup::QUAD_0)
            + i * sizeof(float);

        memcpy(&telemetry[i], &response.data[offset], sizeof(float));
    }
}


