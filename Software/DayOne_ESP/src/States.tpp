#include "States.hpp"

#pragma region Initial State

void Initial_State::Enter()
{
    next_state = STATES::NONE_STATE;
};

void Initial_State::Exit()
{
    next_state = STATES::NONE_STATE;
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