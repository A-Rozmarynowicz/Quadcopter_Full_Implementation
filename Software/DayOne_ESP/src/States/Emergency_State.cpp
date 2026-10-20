#include "States.hpp"
#include "STM_Communication.hpp"

void Initial_State::Enter()
{
    Reset_Requested_State();
    I2C_MESSAGES::Send_Emergency_Total_Stop();
};

void Initial_State::Exit()
{
    Reset_Requested_State();
}

void Initial_State::Task_1ms()
{

}

void Initial_State::Task_5ms()
{

}

void Initial_State::Task_20ms()
{

}

void Initial_State::Task_100ms()
{

}

void Initial_State::Task_1000ms()
{
    I2C_MESSAGES::Send_Emergency_Total_Stop();
}

