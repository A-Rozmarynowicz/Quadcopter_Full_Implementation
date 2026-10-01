#ifndef ESPCOMMUNICATION_H
#define ESPCOMMUNICATION_H

#include "Configuration.h"

#define DATA_SIZE 20
const uint8_t BROADCAST_RECEIVER_ID = 255;
const uint8_t ACK_MESSAGE_COUNT = 5;
const 

enum Data_Commands {
  READY_FOR_OBSERVER = 24,
  OBSERVER_QUERY_POSITION,
  OBSERVER_RESPONSE_POSITION,
  OBSERVER_WAKEUP_RECKON,
};

enum Data_Setup {
  RECEIVER_ID = 0,
  TRANSMITTER_ID = 1,
  COMMAND = 2,
  SINGLE_0 = 3,
  QUAD_0 = 4,
  QUAD_1 = 8,
  QUAD_2 = 12,
  QUAD_3 = 16,
};

extern uint8_t transmit_buffer[DATA_SIZE];

#endif