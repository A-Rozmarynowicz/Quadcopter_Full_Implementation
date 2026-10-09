#include "ESP_Communication.hpp"

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

bool ESP_MESSAGES::Send_Ready(uint8_t receiver)
{
  ESP_Packet packet{};
  packet.data[ESP_Data_Setup::RECEIVER_ID] = receiver;
  packet.data[ESP_Data_Setup::TRANSMITTER_ID] = DRONE_ESP_ID;
  packet.data[ESP_Data_Setup::COMMAND] = ESP_Data_Commands::OBSERVER_READY;

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
  uint8_t receiver_id = data[RECEIVER_ID];
  if ((receiver_id != DRONE_ESP_ID) && (receiver_id != BROADCAST_RECEIVER_ID))
  {
    return;
  }
   ESP_Packet packet;

   std::copy(data, data + dataLen, packet.data);

   esp_receive_queue.push(packet);
//   State_ReceiveCallback(data);
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

#pragma endregion