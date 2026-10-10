#ifndef ESP_COMMUNICATION_HPP
#define ESP_COMMUNICATION_HPP

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "Buffers.hpp"

#define DRONE_ESP_ID 171
#define GROUND_STATION_ID 169
#define ESP_DATA_SIZE 20
#define BROADCAST_RECEIVER_ID 255
#define ESP_RECEIVE_QUEUE_SIZE 33
#define ESP_TRANSMIT_QUEUE_SIZE 33

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

enum ESP_Communication_Errors
{
  PROTOCOL_INIT_FAIL,
  MESSAGE_SEND_FAIL,
  DELIVERY_FAIL,
  ACK_FAIL,
};

namespace ESP_MESSAGES
{
  bool Send_Query_Distances();
  bool Send_Position(uint8_t lghs_id);
};

struct ESP_Packet
{
    uint8_t data[ESP_DATA_SIZE];
    uint8_t flush_value;
    static constexpr uint8_t max_flush_value = 16;
    void Mark_To_Flush(){flush_value=max_flush_value;}
    void Increase_Flush_Value()
      {if (flush_value >= max_flush_value){return;}
      flush_value++;}
    uint8_t Get_Flush_Value(){return flush_value;}
    bool Is_For_Flush(){return flush_value>=max_flush_value;}
};

extern Queue<ESP_Packet, ESP_RECEIVE_QUEUE_SIZE> esp_receive_queue;
extern Queue<ESP_Packet, ESP_TRANSMIT_QUEUE_SIZE> esp_transmit_queue;

void Initialize_ESP_Communication();

void _send_esp();
void _receive_callback(const uint8_t* macAddr, const uint8_t* data, int dataLen);
void _sent_callback(const uint8_t *macAddr, esp_now_send_status_t status);
void _communication_error(ESP_Communication_Errors error);


#endif