#include "ESP_Communication.hpp"
#include "State_Machine.hpp"

Queue<ESP_Packet, ESP_TRANSMIT_QUEUE_SIZE> esp_transmit_queue;


#pragma region Messages
bool ESP_MESSAGES::Send_Ground_Station_Query_Distances()
{
  ESP_Packet packet{};
  packet.data[ESP_Data_Setup::RECEIVER_ID] = 0;
  packet.data[ESP_Data_Setup::TRANSMITTER_ID] = GROUND_STATION_ID;
  packet.data[ESP_Data_Setup::COMMAND] = ESP_Data_Commands::GROUND_STATION_QUERY_DISTANCES;

  return esp_transmit_queue.push(packet);
}

bool ESP_MESSAGES::Send_Ground_Station_Update_Position(uint8_t lghs_id, Position& position)
{
  ESP_Packet packet{};
  packet.data[ESP_Data_Setup::RECEIVER_ID] = 0;
  packet.data[ESP_Data_Setup::TRANSMITTER_ID] = GROUND_STATION_ID;
  packet.data[ESP_Data_Setup::COMMAND] = ESP_Data_Commands::GROUND_STATION_UPDATE_POSITION;
  packet.data[ESP_Data_Setup::SINGLE_0] = lghs_id;

  memcpy(&(packet.data[ESP_Data_Setup::QUAD_0]), &(position.x), sizeof(float));
  memcpy(&(packet.data[ESP_Data_Setup::QUAD_1]), &(position.y), sizeof(float));
  memcpy(&(packet.data[ESP_Data_Setup::QUAD_2]), &(position.z), sizeof(float));

  return esp_transmit_queue.push(packet);

}

bool ESP_MESSAGES::Send_Ground_Station_Position_Transfer_Done()
{
  ESP_Packet packet{};
  packet.data[ESP_Data_Setup::RECEIVER_ID] = 0;
  packet.data[ESP_Data_Setup::TRANSMITTER_ID] = GROUND_STATION_ID;
  packet.data[ESP_Data_Setup::COMMAND] = ESP_Data_Commands::GROUND_STATION_POSITION_TRANSFER_DONE;

  return esp_transmit_queue.push(packet);
}

bool ESP_MESSAGES::Send_Ground_Station_Request_Current_State(uint8_t id)
{
  ESP_Packet packet{};
  packet.data[ESP_Data_Setup::RECEIVER_ID] = id;
  packet.data[ESP_Data_Setup::TRANSMITTER_ID] = GROUND_STATION_ID;
  packet.data[ESP_Data_Setup::COMMAND] = ESP_Data_Commands::GROUND_STATION_REQUEST_CURRENT_STATE;

  return esp_transmit_queue.push(packet);
}


#pragma endregion

#pragma region ESP_NOW

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

void ESP_Transmit()
{
  _send_esp();
}

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

  state_machine.State_Receive_Callback(packet);

  if (data[ESP_Data_Setup::COMMAND] == ESP_Data_Commands::LGHS_WAS_RESET)
  {
    Serial.printf("LGH WAS RESET: nr. %d\n", data[ESP_Data_Setup::TRANSMITTER_ID]);
  }
};


void _sent_callback(const uint8_t *macAddr, esp_now_send_status_t status)
{
  if (status == ESP_NOW_SEND_SUCCESS)
  {
  }
  else
  {
    _communication_error(ESP_Communication_Errors::DELIVERY_FAIL);
  }
};

void _communication_error(ESP_Communication_Errors error)
{};

#pragma endregion