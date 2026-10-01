#include "ESP_Communication.h"

void Initialize_Communication(){
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() == ESP_OK) {
    esp_now_register_recv_cb(_receive_callback);
    esp_now_register_send_cb(_sent_callback);
  }
  else {
    _communication_error(Communication_Errors::PROTOCOL_INIT_FAIL);
  }
  transmit_buffer[Data_Setup::TRANSMITTER_ID] = DRONE_ID;
};