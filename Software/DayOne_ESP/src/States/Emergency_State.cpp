#include "States.hpp"
#include "STM_Communication.hpp"

void Emergency_State::Enter()
{
    Reset_Requested_State();
    I2C_MESSAGES::Send_Emergency_Total_Stop();
};

void Emergency_State::Exit()
{
    Reset_Requested_State();
}

void Emergency_State::Task_1ms()
{

}


void Emergency_State::Task_5ms()
{

}

void Emergency_State::Task_10ms()
{

}

void Emergency_State::Task_20ms()
{

}

void Emergency_State::Task_100ms()
{

}

void Emergency_State::Task_1000ms()
{
    I2C_MESSAGES::Send_Emergency_Total_Stop();
}

