#include "States.hpp"

#pragma region State

void State::Request_Change_State(STATES new_state)
{
    next_state = new_state;
}

void State::Reset_State()
{
    next_state = STATES::NONE_STATE;
}

STATES State::Get_Requested_State()
{
    return next_state;
}

#pragma endregion


#pragma region Initial State

void Initial_State::Enter()
{
    Reset_State();
};

void Initial_State::Exit()
{
    Reset_State();
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

}

#pragma endregion