#include "ESP_Communication.hpp"
#include "STM_Communication.hpp"

Queue<ESP_Packet, ESP_RECEIVE_QUEUE_SIZE> esp_receive_queue;
Queue<ESP_Packet, ESP_TRANSMIT_QUEUE_SIZE> esp_transmit_queue;


void Initialize_ESP_Communication()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() == ESP_OK)
  {
    esp_now_register_recv_cb(_receive_callback);
    esp_now_register_send_cb(_sent_callback);
  }
  else
  {
    _communication_error(ESP_Communication_Errors::PROTOCOL_INIT_FAIL);
  }
}

void Flush_Unused_ESP_Received_Packets()
{
  for (uint8_t i = 0; i<esp_receive_queue.size(); i++)
  {
    if (esp_receive_queue[esp_receive_queue.get_head_offset_index(i)].Is_For_Flush())
    {
      ESP_Packet proxy;
      esp_receive_queue.pop(proxy);
    }
    else
    {
      break;
    }
  }
};

#pragma region Messages

bool ESP_MESSAGES::Send_Query_Position(uint8_t receiver)
{
  ESP_Packet packet{};
  // Serial.printf("Sending query to %d ------------------------------------------- \n", receiver);
  packet.data[ESP_Data_Setup::RECEIVER_ID] = receiver;
  packet.data[ESP_Data_Setup::TRANSMITTER_ID] = DRONE_ESP_ID;
  packet.data[ESP_Data_Setup::COMMAND] = ESP_Data_Commands::OBSERVER_QUERY_POSITION;

  return esp_transmit_queue.push(packet);
}

bool ESP_MESSAGES::Send_Wakeup_Reckon(uint8_t receiver)
{
  ESP_Packet packet{};
  packet.data[ESP_Data_Setup::RECEIVER_ID] = receiver;
  packet.data[ESP_Data_Setup::TRANSMITTER_ID] = DRONE_ESP_ID;
  packet.data[ESP_Data_Setup::COMMAND] = ESP_Data_Commands::OBSERVER_WAKEUP_RECKON;

  return esp_transmit_queue.push(packet);
}

bool ESP_MESSAGES::Send_Full_Telemetry(float (&float_telemetry)[6], uint8_t (&int_telemetry)[3])
{
  ESP_Packet packet{};
  packet.data[ESP_Data_Setup::RECEIVER_ID] = GROUND_STATION_ID;
  packet.data[ESP_Data_Setup::TRANSMITTER_ID] = DRONE_ESP_ID;
  packet.data[ESP_Data_Setup::COMMAND] = ESP_Data_Commands::GROUND_STATION_FULL_TELEMETRY_TRANSFER;

  packet.data[ESP_Data_Setup::SINGLE_0] = int_telemetry[0];
  packet.data[ESP_Data_Setup::SINGLE_1] = int_telemetry[1];
  packet.data[ESP_Data_Setup::SINGLE_2] = int_telemetry[2];

  memcpy(&(packet.data[ESP_Data_Setup::QUAD_0]), &(float_telemetry[0]), sizeof(float));
  memcpy(&(packet.data[ESP_Data_Setup::QUAD_1]), &(float_telemetry[1]), sizeof(float));
  memcpy(&(packet.data[ESP_Data_Setup::QUAD_2]), &(float_telemetry[2]), sizeof(float));
  memcpy(&(packet.data[ESP_Data_Setup::QUAD_3]), &(float_telemetry[3]), sizeof(float));
  memcpy(&(packet.data[ESP_Data_Setup::QUAD_4]), &(float_telemetry[4]), sizeof(float));
  memcpy(&(packet.data[ESP_Data_Setup::QUAD_5]), &(float_telemetry[5]), sizeof(float));

  return esp_transmit_queue.push(packet);
}

#pragma endregion

#pragma region ESP_NOW
void _send_esp()
{
  if (esp_transmit_queue.empty())
  {
      return;
  }
  uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  esp_now_peer_info_t peerInfo = {};
  memcpy(&peerInfo.peer_addr, broadcastAddress, 6);
  if (!esp_now_is_peer_exist(broadcastAddress))
  {
      esp_now_add_peer(&peerInfo);
  }

  ESP_Packet transmit_packet;
  if (!esp_transmit_queue.pop(transmit_packet))
  {
      return;
  }

  esp_err_t result = esp_now_send(broadcastAddress, transmit_packet.data, ESP_DATA_SIZE);
  if (result == ESP_OK) {}
  else
  {
      _communication_error(ESP_Communication_Errors::MESSAGE_SEND_FAIL);
  }
};


void _receive_callback(const uint8_t* macAddr, const uint8_t* data, int dataLen)
{
  uint8_t receiver_id = data[ESP_Data_Setup::RECEIVER_ID];

  if ((receiver_id != DRONE_ESP_ID) && (receiver_id != BROADCAST_RECEIVER_ID))
  {
    return;
  }
  Serial.printf("TRANSMITTTER: %d\n", data[ESP_Data_Setup::TRANSMITTER_ID]);
  ESP_Packet packet;

  std::copy(data, data + dataLen, packet.data);

  if (_handle_urgent_esp_message(packet))
  {
    return;
  }
  esp_receive_queue.push(packet);
};


void _sent_callback(const uint8_t *macAddr, esp_now_send_status_t status)
{
  if (status == ESP_NOW_SEND_SUCCESS)
  {
    // State_SentCallback();
  }
  else
  {
    _communication_error(ESP_Communication_Errors::DELIVERY_FAIL);
  }
};

void _communication_error(ESP_Communication_Errors error)
{};

bool _handle_urgent_esp_message(ESP_Packet &packet)
{
  switch (packet.data[ESP_Data_Setup::COMMAND])
  {
  case ESP_Data_Commands::GROUND_STATION_HEARTBEAT:
    I2C_MESSAGES::Send_Heartbeat();
    return true;
  case ESP_Data_Commands::QUADCOPTER_EMERGENCY_TOTAL_STOP:
    I2C_MESSAGES::Send_Emergency_Total_Stop();
    return false;
  case ESP_Data_Commands::QUADCOPTER_EMERGENCY_LANDING:
    I2C_MESSAGES::Send_Emergency_Landing();
    return true;
  default:
    break;
  }

  return false;
}

#pragma endregion
