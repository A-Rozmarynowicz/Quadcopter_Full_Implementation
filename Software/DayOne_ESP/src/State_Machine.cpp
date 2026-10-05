#include "State_Machine.hpp"


void State_Machine::State_Machine_Initialize()
{
    current_state = &initial_state;
    current_state->Enter();
}

#pragma State transitions
void State_Machine::change_state(State &new_state)
{
    current_state->Exit();
    current_state = &new_state;
    current_state->Enter();
}

void State_Machine::Handle_State_Change_Request()
{
    switch (current_state->Get_Requested_State())
    {
    case STATES::NONE_STATE:
        break;
    case STATES::INITIAL_STATE:
        change_state(initial_state);
        break;
    case STATES::REQUEST_LGHS_POSITIONS_STATE:
        change_state(request_lghs_positions_state);
        break;
    default:
        break;
    }
}

#pragma endregion

#pragma region Tasks

void State_Machine::Task_1ms()
{
    current_state->Task_1ms();
}

void State_Machine::Task_5ms()
{
    current_state->Task_5ms();
}

void State_Machine::Task_20ms()
{
    current_state->Task_20ms();
}

void State_Machine::Task_100ms()
{
    current_state->Task_100ms();
}

void State_Machine::Task_1000ms()
{
    current_state->Task_1000ms();
}

#pragma endregion