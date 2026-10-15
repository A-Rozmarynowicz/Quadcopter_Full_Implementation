#include "Serial_Handler.hpp"

void Update_Serial()
{
    if (Serial.available()) {
        String message = Serial.readStringUntil('\n');
        message.trim();

        if (message.length() == 0) return;

        Serial.print("Received: ");
        Serial.println(message);

        int command;
        float values[12];

        if (!Parse_Message(message, command, values)) {
            Serial.println("Invalid message\n");
            return;
        }

        if (command == SERIAL_COMMANDS::SEND_POSITIONS) {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 3; j++) {
                    P[i][j] = values[i * 3 + j];
                }
            }

            Serial.println("Received positions:");
            for (int i = 0; i < 4; i++) {
                Serial.printf("P%d: %.2f, %.2f, %.2f\n",
                    i + 1, P[i][0], P[i][1], P[i][2]);
            }

            // Example response: echo the 12 values.
            sendMessage(Serial, CMD_RETURN_POSITIONS, values);
        }
    }
}

bool Parse_Message(const String& message, int& command, float values[12]);

void Send_Distances();

void Relay_Message()
{

}