#include "States.hpp"

void Initial_State::Enter()
{
    Request_Change_State(STATES::REQUEST_DISTANCES_STATE);
}

void Initial_State::Exit()
{

}

void Initial_State::Receive_Callback(ESP_Packet& esp_packet)
{

}

void Initial_State::Handle_Serial_Message(uint8_t command, const float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER], uint8_t value_count)
{

    switch (command) {
        case static_cast<uint8_t>(SERIAL_COMMANDS::CHANGE_TO_MANUAL_OPERATION_STATE):
            if (value_count == 0) {
                Request_Change_State(STATES::MANUAL_OPERATION_STATE);
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
            Serial.println("Unknown command. Currently in Initial state.");
            break;
    }


}