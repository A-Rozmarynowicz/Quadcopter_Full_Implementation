#include "Serial_Handler.hpp"

#include <stdlib.h>
#include <math.h>
#include <string.h>

// Include the headers that define:
// SERIAL_COMMANDS, LGHS_Distances, LGHS_Positions,
// and Store_LGHS_Position().

bool Update_Serial(uint8_t& command, float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER], uint8_t& value_count)
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
        return false;
    }
    return true;
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
