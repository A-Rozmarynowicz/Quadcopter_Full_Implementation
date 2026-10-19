#include <Arduino.h>

#include "State_Machine.hpp"
#include "ESP_Communication.hpp"

State_Machine state_machine;

void setup() {
  Serial.begin(115200);
  Initialize_ESP_Communication();
  state_machine.State_Machine_Initialize();
  Serial.print("Initialization complete\n");
}

void loop() {
  state_machine.State_Update();
  state_machine.Handle_State_Change_Request();
  ESP_Transmit();
}
