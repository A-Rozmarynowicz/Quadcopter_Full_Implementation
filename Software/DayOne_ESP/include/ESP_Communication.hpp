#pragma once

#include "Configuration.hpp"
#include "Buffers.hpp"

#define DRONE_ID 171
#define DATA_SIZE 20
#define RECEIVE_QUEUE_SIZE 33

const uint8_t BROADCAST_RECEIVER_ID = 255;
const uint8_t ACK_MESSAGE_COUNT = 5;

enum Data_Commands
{
  READY_FOR_OBSERVER = 24,
  OBSERVER_QUERY_POSITION,
  OBSERVER_RESPONSE_POSITION,
  OBSERVER_WAKEUP_RECKON,
};

enum Data_Setup
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

enum Communication_Errors
{
  PROTOCOL_INIT_FAIL,
  MESSAGE_SEND_FAIL,
  DELIVERY_FAIL,
  ACK_FAIL,
};

namespace ESP_MESSAGES
{

};

struct Receive_Packet
{
    uint8_t data[DATA_SIZE];
    uint16_t length;
};

extern uint8_t transmit_buffer[DATA_SIZE];
extern Queue<Receive_Packet, RECEIVE_QUEUE_SIZE> receive_queue;

void initialize_communication();

void _send_esp();
void _receive_callback(const uint8_t* macAddr, const uint8_t* data, int dataLen);
void _sent_callback(const uint8_t *macAddr, esp_now_send_status_t status);
void _communication_error(Communication_Errors error);
