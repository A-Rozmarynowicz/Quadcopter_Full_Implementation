#include "States.hpp"

void Request_Distances_State::Enter()
{
}

void Request_Distances_State::Exit()
{

}

void Request_Distances_State::Receive_Callback(ESP_Packet& esp_packet)
{
    if (esp_packet.data[ESP_Data_Setup::COMMAND] == ESP_Data_Commands::GROUND_STATION_ANSWER_DISTANCES) {
        Update_Distances(esp_packet);
        Serial.println("Received distances from master LGHS\n");
        Request_Change_State(STATES::MATLAB_EXCHANGE_STATE);
    }
    else if (esp_packet.data[ESP_Data_Setup::COMMAND] == ESP_Data_Commands::MASTER_READY_FOR_GROUND_STATION_EXCHANGE) {
        Serial.println("MASTER READY FOR DATA EXCHANGE\n");
    }
}

void Request_Distances_State::Handle_Serial_Message(uint8_t command, const float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER], uint8_t value_count)
{
    switch (command) {
        case static_cast<uint8_t>(SERIAL_COMMANDS::ENABLE_ESP_REQUEST_DISTANCES):
            if (value_count == 0) {
                ESP_MESSAGES::Send_Ground_Station_Query_Distances();
                Serial.print("Quering distances\n");
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

void Request_Distances_State::Update_Distances(ESP_Packet& packet)
{
    if (NUMBER_OF_LIGHTHOUSES == 4)
    {
        _update_distances_4LGHS(packet);
    }
    else if (NUMBER_OF_LIGHTHOUSES == 5)
    {
        _update_distances_5LGHS(packet);
    }
}

void Request_Distances_State::_update_distances_4LGHS(ESP_Packet& packet)
{
    float d01, d02, d03, d12, d13, d23;
    memcpy(&d01, &(packet.data[ESP_Data_Setup::QUAD_0]), sizeof(float));
    memcpy(&d02, &(packet.data[ESP_Data_Setup::QUAD_1]), sizeof(float));
    memcpy(&d03, &(packet.data[ESP_Data_Setup::QUAD_2]), sizeof(float));
    memcpy(&d12, &(packet.data[ESP_Data_Setup::QUAD_3]), sizeof(float));
    memcpy(&d13, &(packet.data[ESP_Data_Setup::QUAD_4]), sizeof(float));
    memcpy(&d23, &(packet.data[ESP_Data_Setup::QUAD_5]), sizeof(float));
    Store_LGHS_Distance(0, 1, d01);
    Store_LGHS_Distance(0, 2, d02);
    Store_LGHS_Distance(0, 3, d03);
    Store_LGHS_Distance(1, 2, d12);
    Store_LGHS_Distance(1, 3, d13);
    Store_LGHS_Distance(2, 3, d23);
}

void Request_Distances_State::_update_distances_5LGHS(ESP_Packet& packet)
{
    float d01, d02, d03, d04, d12, d13, d14, d23, d24, d34;
    memcpy(&d01, &(packet.data[ESP_Data_Setup::QUAD_0]), sizeof(float));
    memcpy(&d02, &(packet.data[ESP_Data_Setup::QUAD_1]), sizeof(float));
    memcpy(&d03, &(packet.data[ESP_Data_Setup::QUAD_2]), sizeof(float));
    memcpy(&d04, &(packet.data[ESP_Data_Setup::QUAD_3]), sizeof(float));
    memcpy(&d12, &(packet.data[ESP_Data_Setup::QUAD_4]), sizeof(float));
    memcpy(&d13, &(packet.data[ESP_Data_Setup::QUAD_5]), sizeof(float));
    memcpy(&d14, &(packet.data[ESP_Data_Setup::QUAD_6]), sizeof(float));
    memcpy(&d23, &(packet.data[ESP_Data_Setup::QUAD_7]), sizeof(float));
    memcpy(&d24, &(packet.data[ESP_Data_Setup::QUAD_8]), sizeof(float));
    memcpy(&d34, &(packet.data[ESP_Data_Setup::QUAD_9]), sizeof(float));
    Store_LGHS_Distance(0, 1, d01);
    Store_LGHS_Distance(0, 2, d02);
    Store_LGHS_Distance(0, 3, d03);
    Store_LGHS_Distance(0, 4, d04);
    Store_LGHS_Distance(1, 2, d12);
    Store_LGHS_Distance(1, 3, d13);
    Store_LGHS_Distance(1, 4, d14);
    Store_LGHS_Distance(2, 3, d23);
    Store_LGHS_Distance(2, 4, d24);
    Store_LGHS_Distance(3, 4, d34);
}
