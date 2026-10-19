#include "States.hpp"

void Matlab_Exchange_State::Enter()
{
    Serial.print("MATLAB state ready\n");
}

void Matlab_Exchange_State::Exit()
{

}

void Matlab_Exchange_State::Receive_Callback(ESP_Packet& esp_packet)
{

}

void Matlab_Exchange_State::Handle_Serial_Message(uint8_t command, const float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER], uint8_t value_count)
{
    switch (command) {
        case static_cast<uint8_t>(SERIAL_COMMANDS::SEND_POSITIONS):
            if (value_count == 12) {
                Handle_Serial_Position_Transfer(values);
            }
            break;

        case static_cast<uint8_t>(SERIAL_COMMANDS::REQUEST_MEASUREMENTS):
            if (value_count == 0) {
                Send_Serial_Distances();
            }
            break;

        case static_cast<uint8_t>(SERIAL_COMMANDS::MATLAB_TRANSMISSION_CORRECT):
            if (value_count == 0) {
                Request_Change_State(STATES::SEND_POSITIONS_STATE);
            }
            break;

            case static_cast<uint8_t>(SERIAL_COMMANDS::MATLAB_TRANSMISSION_WRONG):
            if (value_count == 0) {
                Request_Change_State(STATES::MATLAB_EXCHANGE_STATE);
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

void Matlab_Exchange_State::Handle_Serial_Position_Transfer(const float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER])
{
    for (size_t i = 0; i < NUMBER_OF_LIGHTHOUSES; ++i) {
        Store_LGHS_Position(
            i,
            values[i * 3],
            values[i * 3 + 1],
            values[i * 3 + 2]
        );
    }

    Serial.println("Received positions:");

    for (size_t i = 0; i < NUMBER_OF_LIGHTHOUSES; ++i) {
        Serial.printf(
            "P%u: %.2f, %.2f, %.2f\n",
            static_cast<unsigned>(i + 1),
            LGHS_Positions[i].x,
            LGHS_Positions[i].y,
            LGHS_Positions[i].z
        );
    }
}

void Matlab_Exchange_State::Send_Serial_Distances()
{
    Serial.printf("%d", static_cast<int>(
        SERIAL_COMMANDS::SEND_DISTANCES));

    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = i + 1; j < NUMBER_OF_LIGHTHOUSES; ++j) {
            Serial.printf(",%.2f", LGHS_Distances[i][j]);
        }
    }

    Serial.println();
}
