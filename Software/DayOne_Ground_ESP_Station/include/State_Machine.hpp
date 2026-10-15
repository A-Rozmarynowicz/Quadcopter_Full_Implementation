#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

#include <Arduino.h>
#include "ESP_Structures.hpp"
#include "States.hpp"

class State_Machine
{
private:
    Initial_State initial_state;
    Request_Distances_State request_distances_state;
    Matlab_Exchange_State matlab_exchange_state;
    Send_Positions_State send_positions_state;

    State* current_state;

    void change_state(State &new_state);
public:
    void State_Machine_Initialize();
    void Handle_State_Change_Request();
    void State_Receive_Callback(ESP_Packet& esp_packet);
    void State_Update();

};

extern State_Machine state_machine;

#endif