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
                Serial.printf("Quadcopter INIT\n");
            }
            break;

        case static_cast<uint8_t>(SERIAL_COMMANDS::QUADCOPTER_ENABLE_FULL_TELEMETRY_REQUESTS):
            if (value_count == 1) {
                bool enable = static_cast<bool>(values[0]);
                ESP_MESSAGES::Send_Ground_Station_Enable_Full_Telemetry_Requests(enable);
                Serial.printf("Quadcopter TELEMETRY: %d\n", enable);
            }
            break;

        case static_cast<uint8_t>(SERIAL_COMMANDS::QUADCOPTER_HOVER):
            if (value_count == 1) {
                float height = static_cast<float>(values[0]);
                ESP_MESSAGES::Send_Quadcopter_Hover(height);
                Serial.printf("Quadcopter HOVER at %0.2fm\n", height);
            }
            break;

        case static_cast<uint8_t>(SERIAL_COMMANDS::QUADCOPTER_LAND):
            if (value_count == 0) {
                ESP_MESSAGES::Send_Quadcopter_Land();
                Serial.printf("Quadcopter LAND\n");
            }
            break;

        case static_cast<uint8_t>(SERIAL_COMMANDS::QUADCOPTER_EMERGENCY_TOTAL_STOP):
            if (value_count == 0) {
                ESP_MESSAGES::Send_Quadcopter_Emergency_Total_Stop();
                Serial.printf("Quadcopter EMERGENCY TOTAL STOP\n");
            }
            break;

        case static_cast<uint8_t>(SERIAL_COMMANDS::QUADCOPTER_EMERGENCY_LANDING):
            if (value_count == 0) {
                ESP_MESSAGES::Send_Quadcopter_Emergency_Landing();
                Serial.printf("Quadcopter EMERGENCY LANDING\n");
            }
            break;

        case static_cast<uint8_t>(SERIAL_COMMANDS::QUADCOPTER_ESP_RESET):
            if (value_count == 0) {
                ESP_MESSAGES::Send_Quadcopter_ESP_Reset();
                Serial.printf("Quadcopter RESET ESP\n");
            }
            break;

        case static_cast<uint8_t>(SERIAL_COMMANDS::QUADCOPTER_STM_RESET):
            if (value_count == 0) {
                ESP_MESSAGES::Send_Quadcopter_Emergency_Landing();
                Serial.printf("Quadcopter RESET STM\n");
            }
            break;

        default:
            Serial.println("Unknown command");
            break;
    }
}