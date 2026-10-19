#include "States.hpp"

void Flight_Operation_State::Enter()
{

}

void Flight_Operation_State::Exit()
{

}

void Flight_Operation_State::Receive_Callback(ESP_Packet& esp_packet)
{

}

void Flight_Operation_State::Handle_Serial_Message(uint8_t command, const float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER], uint8_t value_count)
{
    switch (command) {
        case static_cast<uint8_t>(SERIAL_COMMANDS::HEARTBEAT):
            if (value_count == 0) {
                ESP_MESSAGES::Send_Ground_Station_Heartbeat();
                Serial.printf("Heartbeat\n");
            }
            break;

        case static_cast<uint8_t>(SERIAL_COMMANDS::QUADCOPTER_INIT):
            if (value_count == 0) {
                ESP_MESSAGES::Send_Ground_Station_Quadcopter_Init();
                Serial.printf("Init quadcopter\n");
            }
            break;

        default:
            Serial.println("Unknown command");
            break;
    }
}