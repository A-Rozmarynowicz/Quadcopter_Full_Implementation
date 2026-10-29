#include "States.hpp"

void Manual_Operation_State::Enter()
{
    Serial.print("Entered manual operation state\n");
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
    switch (command) {
        case static_cast<uint8_t>(SERIAL_COMMANDS::REQUEST_CURRENT_LGH_STATE):
            if (value_count == 1) {
                uint8_t id = static_cast<uint8_t>(round(values[0]));
                ESP_MESSAGES::Send_Ground_Station_Request_Current_State(id);
                Serial.printf("Requested state of lighthouse number %d\n", id);
            }
            break;

        case static_cast<uint8_t>(SERIAL_COMMANDS::LGHS_UWB_SETTINGS_SET):
            if (value_count == 4) {
                uint8_t receiver = static_cast<uint8_t>(values[0]);
                uint8_t channel = static_cast<uint8_t>(values[1]);
                uint16_t response_delay_time = static_cast<uint16_t>(values[2]);
                uint32_t reset_time = static_cast<uint32_t>(values[3]);
                ESP_MESSAGES::Send_UWB_Settings(receiver, response_delay_time, reset_time, channel);
                Serial.printf("UWB new settings: receiver: %d, channel: %d, response_delay_time: %d, reset_time: %d\n", receiver, channel, response_delay_time, reset_time);
            }
            break;

        default:
            Serial.println("Unknown command");
            break;
    }
}