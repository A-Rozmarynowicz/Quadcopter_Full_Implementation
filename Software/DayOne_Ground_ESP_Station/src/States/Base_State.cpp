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


void State::Update()
{
    uint8_t command;
    float values[MAX_NUMBER_OF_VALUES_IN_TRANSFER];
    uint8_t value_count;
    if (Update_Serial(command, values, value_count))
    {
        Handle_Serial_Message(command, values, value_count);
        Serial.printf("YES YES\n");
    }
}