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
    Serial.printf("NEW STATE: %d \n --------------------------------------- \n", new_state.Get_State());
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
    case STATES::REQUEST_DISTANCES_STATE:
        change_state(request_distances_state);
        break;
    case STATES::MATLAB_EXCHANGE_STATE:
        change_state(matlab_exchange_state);
        break;
    case STATES::SEND_POSITIONS_STATE:
        change_state(send_positions_state);
        break;
    case STATES::MANUAL_OPERATION_STATE:
        change_state(manual_operation_state);
        break;

    default:
        break;
    }
}


void State_Machine::State_Receive_Callback(ESP_Packet& esp_packet)
{
    current_state->Receive_Callback(esp_packet);
}

void State_Machine::State_Update()
{
    current_state->Update();
}

