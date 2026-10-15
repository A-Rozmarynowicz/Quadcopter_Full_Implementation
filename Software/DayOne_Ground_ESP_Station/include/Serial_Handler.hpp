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

void Update_Serial();
bool Parse_Message(const String& message, int& command, float values[MAX_NUMBER_OF_VALUES_IN_TRANSFER]);
void Handle_Message(uint8_t command, float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER]);


void Send_Distances();
void Relay_Message();

void Handle_Position_Transfer(float (&values)[12]);

#endif