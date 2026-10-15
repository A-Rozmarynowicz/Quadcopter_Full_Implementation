#include "States.hpp"

void Manual_Operation_State::Enter()
{

}

void Manual_Operation_State::Exit()
{

}

void Manual_Operation_State::Receive_Callback(ESP_Packet& esp_packet)
{
    if (esp_packet.data[ESP_Data_Setup::COMMAND] == ESP_Data_Commands::ANSWER_CURRENT_STATE)
    {
        Serial.printf("Current state of lgh: %d is %d\n", esp_packet.data[ESP_Data_Setup::TRANSMITTER_ID], esp_packet.data[ESP_Data_Setup::SINGLE_0]);
    }
}

void Manual_Operation_State::Handle_Serial_Message(uint8_t command, const float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER], uint8_t value_count)
{

}