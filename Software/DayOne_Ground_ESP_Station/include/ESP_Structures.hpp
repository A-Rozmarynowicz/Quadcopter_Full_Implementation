#ifndef ESP_STRUCTURES_HPP
#define ESP_STRUCTURES_HPP

#include <Arduino.h>

#define ESP_DATA_SIZE 20


struct ESP_Packet
{
    uint8_t data[ESP_DATA_SIZE];
};


enum ESP_Data_Setup
{
  RECEIVER_ID = 0,
  TRANSMITTER_ID = 1,
  COMMAND = 2,
  SINGLE_0 = 3,
  QUAD_0 = 4,
  QUAD_1 = 8,
  QUAD_2 = 12,
  QUAD_3 = 16,
};

#endif