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

}

void Flight_State::Task_1000ms()
{

}

inline bool Flight_State::Check_If_Enough_Measurements_Made()
{
    return false;
}
