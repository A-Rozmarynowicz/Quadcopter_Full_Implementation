#include "States.hpp"

void Flight_State::Enter()
{
    Reset_Requested_State();
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
    if (Get_Number_Of_Present_Lighthouses(uwb_measurement_stacks) == NUMBER_OF_LIGHTHOUSES)
    {
        uint8_t penalty = Parse_Measurements();
        if (penalty == 0)
        {
            total_penalty = 0;
        }
        half_periods_passed = 0;
        return;
    }

    if (half_periods_passed >= 1)
    {
        total_penalty += Parse_Measurements();
    }

    half_periods_passed++;
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
