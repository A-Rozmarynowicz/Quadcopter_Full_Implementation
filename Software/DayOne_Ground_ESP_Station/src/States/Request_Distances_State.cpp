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
        Request_Change_State(STATES::MATLAB_EXCHANGE_STATE);
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

        default:
            Serial.println("Unknown command");
            break;
    }
}

void Request_Distances_State::Update_Distances(ESP_Packet& packet)
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