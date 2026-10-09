#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

#include "Configuration.hpp"

class State_Machine
{
private:
    Initial_State initial_state;
    Request_LGHS_Positions_State request_lghs_positions_state;
    Flight_State flight_state;

    State* current_state;

    void change_state(State &new_state);
public:
    void State_Machine_Initialize();
    void Handle_State_Change_Request();

    void Task_1ms();
    void Task_5ms();
    void Task_20ms();
    void Task_100ms();
    void Task_1000ms();
};
// extern State current_state;


#endif