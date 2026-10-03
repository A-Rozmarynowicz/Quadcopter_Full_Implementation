#include "ESP_Communication.hpp"

Queue<Packet, RECEIVE_QUEUE_SIZE> receive_queue;
Queue<Packet, TRANSMIT_QUEUE_SIZE> transmit_queue;


void initialize_esp_communication(){
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() == ESP_OK)
  {
    esp_now_register_recv_cb(_receive_callback);
    esp_now_register_send_cb(_sent_callback);
  }
  else
  {
    _communication_error(Communication_Errors::PROTOCOL_INIT_FAIL);
  }
//   transmit_buffer[Data_Setup::TRANSMITTER_ID] = DRONE_ID; <-------------------
};


#pragma region Messages

bool ESP_MESSAGES::Send_Query_Position(uint8_t receiver)
{
  Packet packet{};

  packet.data[Data_Setup::RECEIVER_ID] = receiver;
  packet.data[Data_Setup::TRANSMITTER_ID] = DRONE_ID;
  packet.data[Data_Setup::COMMAND] = Data_Commands::OBSERVER_QUERY_POSITION;

  return transmit_queue.push(packet);
}

bool ESP_MESSAGES::Send_Wakeup_Reckon(uint8_t receiver)
{
  Packet packet{};

  packet.data[Data_Setup::RECEIVER_ID] = receiver;
  packet.data[Data_Setup::TRANSMITTER_ID] = DRONE_ID;
  packet.data[Data_Setup::COMMAND] = Data_Commands::OBSERVER_WAKEUP_RECKON;

  return transmit_queue.push(packet);
}

bool ESP_MESSAGES::Send_Ready(uint8_t receiver)
{
  Packet packet{};

  packet.data[Data_Setup::RECEIVER_ID] = receiver;
  packet.data[Data_Setup::TRANSMITTER_ID] = DRONE_ID;
  packet.data[Data_Setup::COMMAND] = Data_Commands::OBSERVER_READY;

  return transmit_queue.push(packet);
}

#pragma endregion

#pragma region ESP_NOW
void _send_esp()
{
  if (transmit_queue.empty())
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

  Packet transmit_packet;
  if (!transmit_queue.pop(transmit_packet))
  {
      return;
  }

  esp_err_t result = esp_now_send(broadcastAddress, transmit_packet.data, DATA_SIZE);
  if (result == ESP_OK) {}
  else
  {
      _communication_error(Communication_Errors::MESSAGE_SEND_FAIL);
  }
};


void _receive_callback(const uint8_t* macAddr, const uint8_t* data, int dataLen)
{
  uint8_t receiver_id = data[RECEIVER_ID];
  if ((receiver_id != DRONE_ID) && (receiver_id != BROADCAST_RECEIVER_ID))
  {
    return;
  }
   Packet packet;

   std::copy(data, data + dataLen, packet.data);

   receive_queue.push(packet);
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
    _communication_error(Communication_Errors::DELIVERY_FAIL);
  }
};

void _communication_error(Communication_Errors error)
{};

#pragma endregion