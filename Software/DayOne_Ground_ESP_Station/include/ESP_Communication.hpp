#ifndef ESP_COMMUNICATION_HPP
#define ESP_COMMUNICATION_HPP

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "Buffers.hpp"
#include "ESP_Structures.hpp"
#include "Measurements.hpp"

#define DRONE_ESP_ID 171
#define GROUND_STATION_ID 169
#define BROADCAST_RECEIVER_ID 255
#define ESP_TRANSMIT_QUEUE_SIZE 16

enum ESP_Communication_Errors
{
  PROTOCOL_INIT_FAIL,
  MESSAGE_SEND_FAIL,
  DELIVERY_FAIL,
  ACK_FAIL,
};

namespace ESP_MESSAGES
{
  bool Send_Ground_Station_Query_Distances();
  bool Send_Ground_Station_Update_Position(uint8_t lghs_id, Position& position);
  bool Send_Ground_Station_Position_Transfer_Done();
  bool Send_Ground_Station_Request_Current_State(uint8_t id);
  bool Send_Ground_Station_Heartbeat();

  bool Send_Init_Quadcopter();
};



extern Queue<ESP_Packet, ESP_TRANSMIT_QUEUE_SIZE> esp_transmit_queue;

void Initialize_ESP_Communication();
void ESP_Transmit();

void _send_esp();
void _receive_callback(const uint8_t* macAddr, const uint8_t* data, int dataLen);
void _sent_callback(const uint8_t *macAddr, esp_now_send_status_t status);
void _communication_error(ESP_Communication_Errors error);


#endif