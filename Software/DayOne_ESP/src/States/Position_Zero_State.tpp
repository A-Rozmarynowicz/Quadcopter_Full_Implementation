#include "States.hpp"


void Position_Zero_State::Enter()
{
    Reset_Requested_State();
    Restart_UWB_As_Tag();
};

void Position_Zero_State::Exit()
{
    Reset_Requested_State();
}

void Position_Zero_State::Task_1ms()
{

}

void Position_Zero_State::Task_5ms()
{

}

void Position_Zero_State::Task_20ms()
{

}

void Position_Zero_State::Task_100ms()
{

}

void Position_Zero_State::Task_1000ms()
{

}