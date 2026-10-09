#include "States.hpp"
#include "UWB_Calculations.hpp"

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
    if (Get_Number_Of_Present_Lighthouses(uwb_measurement_stacks) == NUMBER_OF_LIGHTHOUSES)
    {
        Update_Measurements();
    }
}

void Flight_State::Task_100ms()
{

}

void Flight_State::Task_1000ms()
{

}

uint8_t Flight_State::Update_Measurements()
{
    uint8_t penalty = Calculate_Position(uwb_measurement_stacks, current_calculated_position);
    I2C_MESSAGES::Send_Current_Position(current_calculated_position);
    return penalty;
}
