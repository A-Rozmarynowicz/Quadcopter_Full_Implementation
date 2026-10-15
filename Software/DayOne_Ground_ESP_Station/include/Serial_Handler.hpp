#ifndef SERIAL_HANDLER_HPP
#define SERIAL_HANDLER_HPP

#include <Arduino.h>
#include "State_Machine.hpp"


enum SERIAL_COMMANDS
{
    RELAY = 1,
    REQUEST_MEASUREMENTS,
    SEND_POSITIONS,
    ACK_RECEIVED_POSITIONS,

    ERROR = 99,
};

void Update_Serial();
bool Parse_Message(const String& message, int& command, float values[12]);

void Send_Distances();
void Relay_Message();

void Handle_Position_Transfer(float (&values)[12])

#endif