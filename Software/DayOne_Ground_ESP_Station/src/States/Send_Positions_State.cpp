#include "States.hpp"

void Send_Positions_State::Enter()
{
    current_lgh_index = 0;
    ESP_MESSAGES::Send_Ground_Station_Update_Position(current_lgh_index, LGHS_Positions[current_lgh_index]);
    Serial.printf("Updating position nr. %d\n", current_lgh_index);
}

void Send_Positions_State::Exit()
{

}

void Send_Positions_State::Receive_Callback(ESP_Packet& esp_packet)
{
    if (esp_packet.data[ESP_Data_Setup::COMMAND] == ESP_Data_Commands::GROUND_STATION_POSITION_UPDATE_ACK)
    {
        current_lgh_index++;
        if (current_lgh_index == NUMBER_OF_LIGHTHOUSES)
        {
            Serial.print("All positions updated\n");
            Request_Change_State(STATES::MANUAL_OPERATION_STATE);
        }
        else
        {
            ESP_MESSAGES::Send_Ground_Station_Update_Position(current_lgh_index, LGHS_Positions[current_lgh_index]);
            Serial.printf("Updating position nr. %d\n", current_lgh_index);
        }
    }
}

void Send_Positions_State::Handle_Serial_Message(uint8_t command, const float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER], uint8_t value_count)
{
    switch (command) {
        case static_cast<uint8_t>(SERIAL_COMMANDS::ENABLE_ESP_SEND_POSITIONS):
            if (value_count == 0) {
                current_lgh_index = 0;
                ESP_MESSAGES::Send_Ground_Station_Update_Position(current_lgh_index, LGHS_Positions[current_lgh_index]);
                Serial.printf("Updating position nr. %d\n", current_lgh_index);
            }
            break;

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