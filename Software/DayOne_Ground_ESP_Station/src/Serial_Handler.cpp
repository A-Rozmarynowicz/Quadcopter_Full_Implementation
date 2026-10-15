#include "Serial_Handler.hpp"

#include <stdlib.h>
#include <math.h>
#include <string.h>

// Include the headers that define:
// SERIAL_COMMANDS, LGHS_Distances, LGHS_Positions,
// and Store_LGHS_Position().

bool Update_Serial(uint8_t& command, float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER], uint8_t value_count)
{
    if (!Serial.available()) {
        return;
    }

    String message = Serial.readStringUntil('\n');
    message.trim();

    if (message.length() == 0) {
        return;
    }

    Serial.print("Received: ");
    Serial.println(message);

    if (!Parse_Message(message, command, values, value_count)) {
        Serial.println("Invalid message");
        return;
    }
}

bool Parse_Message(
    const String& message,
    uint8_t& command,
    float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER],
    uint8_t& value_count)
{
    char buffer[256];

    if (message.length() == 0 ||
        message.length() >= sizeof(buffer)) {
        return false;
    }

    message.toCharArray(buffer, sizeof(buffer));

    char* token = strtok(buffer, ",");
    if (token == nullptr) {
        return false;
    }

    // Parse and validate the command.
    char* endPtr = nullptr;
    long parsedCommand = strtol(token, &endPtr, 10);

    if (endPtr == token || *endPtr != '\0' ||
        parsedCommand < 0 || parsedCommand > UINT8_MAX) {
        return false;
    }

    command = static_cast<uint8_t>(parsedCommand);
    value_count = 0;

    // Determine the expected number of values for this command.
    size_t expectedCount;

    switch (command) {
        case static_cast<uint8_t>(SERIAL_COMMANDS::SEND_POSITIONS):
            expectedCount = 12;
            break;

        case static_cast<uint8_t>(SERIAL_COMMANDS::REQUEST_MEASUREMENTS):
            expectedCount = 0;
            break;

        default:
            // Unknown commands are rejected.
            return false;
    }

    // Parse exactly the expected number of values.
    for (uint8_t i = 0; i < expectedCount; ++i) {
        token = strtok(nullptr, ",");

        if (token == nullptr) {
            return false;
        }

        char* valueEnd = nullptr;
        float value = strtof(token, &valueEnd);

        if (valueEnd == token || *valueEnd != '\0' ||
            !isfinite(value)) {
            return false;
        }

        values[i] = value;
        ++value_count;
    }

    // Reject any extra fields.
    if (strtok(nullptr, ",") != nullptr) {
        return false;
    }

    return true;
}

void Handle_Message(
    uint8_t command,
    const float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER],
    uint8_t value_count)
{
    switch (command) {
        case static_cast<uint8_t>(SERIAL_COMMANDS::SEND_POSITIONS):
            if (value_count == 12) {
                Handle_Position_Transfer(values);
            }
            break;

        case static_cast<uint8_t>(SERIAL_COMMANDS::REQUEST_MEASUREMENTS):
            if (value_count == 0) {
                Send_Distances();
            }
            break;

        default:
            Serial.println("Unknown command");
            break;
    }
}

void Handle_Position_Transfer(
    const float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER])
{
    for (size_t i = 0; i < 4; ++i) {
        Store_LGHS_Position(
            i,
            values[i * 3],
            values[i * 3 + 1],
            values[i * 3 + 2]
        );
    }

    Serial.println("Received positions:");

    for (size_t i = 0; i < 4; ++i) {
        Serial.printf(
            "P%u: %.2f, %.2f, %.2f\n",
            static_cast<unsigned>(i + 1),
            LGHS_Positions[i].x,
            LGHS_Positions[i].y,
            LGHS_Positions[i].z
        );
    }
}

void Send_Distances()
{
    // Replace SEND_DISTANCES if your enum uses another name.
    Serial.printf("%d", static_cast<int>(
        SERIAL_COMMANDS::SEND_DISTANCES));

    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = i + 1; j < 4; ++j) {
            Serial.printf(",%.2f", LGHS_Distances[i][j]);
        }
    }

    Serial.println();
}

void Relay_Message()
{
    // Implement when required.
}