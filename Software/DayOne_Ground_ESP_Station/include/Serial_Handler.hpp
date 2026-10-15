#ifndef SERIAL_HANDLER_HPP
#define SERIAL_HANDLER_HPP

#include <Arduino.h>
#include "Measurements.hpp"

#define MAX_NUMBER_OF_VALUES_IN_TRANSFER 12

enum SERIAL_COMMANDS
{
    RELAY = 1,
    REQUEST_MEASUREMENTS,
    SEND_POSITIONS,
    ACK_RECEIVED_POSITIONS,
    SEND_DISTANCES,

    ERROR = 99,
};

bool Update_Serial(uint8_t& command, float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER], uint8_t& value_count);
bool Parse_Message(const String& message, uint8_t& command, float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER], uint8_t& value_count);


#endif