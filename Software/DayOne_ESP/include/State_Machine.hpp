#pragma once

#include "Configuration.hpp"
#include "States.hpp"


class State_Machine
{
private:
    Initial_State initial_state;
    Request_LGHS_Positions_State request_lghs_positions_state;

    State* current_state;

    void change_state(State &new_state);
public:
    void State_Machine_Initialize();
    void Handle_State_Change_Request();
};
// extern State current_state;
