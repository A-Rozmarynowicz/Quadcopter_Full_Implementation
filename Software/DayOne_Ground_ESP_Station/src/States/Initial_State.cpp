#include "States.hpp"

void Initial_State::Enter()
{
    Request_Change_State(STATES::MATLAB_EXCHANGE_STATE);
}

void Initial_State::Exit()
{

}

void Initial_State::Receive_Callback(ESP_Packet& esp_packet)
{

}

void Initial_State::Handle_Serial_Message(uint8_t command, const float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER], uint8_t value_count)
{

}