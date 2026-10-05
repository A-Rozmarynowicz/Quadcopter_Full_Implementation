#include "States.hpp"


void State::Request_Change_State(STATES new_state)
{
    next_state = new_state;
}

void State::Reset_Requested_State()
{
    next_state = STATES::NONE_STATE;
}

STATES State::Get_Requested_State()
{
    return next_state;
}
