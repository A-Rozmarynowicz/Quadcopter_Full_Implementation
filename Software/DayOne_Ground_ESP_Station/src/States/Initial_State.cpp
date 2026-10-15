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

        default:
            Serial.println("Unknown command");
            break;
    }


}