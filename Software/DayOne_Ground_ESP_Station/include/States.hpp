#ifndef STATE_HPP
#define STATE_HPP

#include <Arduino.h>

enum class STATES
{
    INITIAL_STATE,
    REQUEST_LGHS_POSITIONS_STATE,
    FLIGHT_STATE,

    NONE_STATE,
};

constexpr size_t State_To_Index(STATES state)
{
    return static_cast<size_t>(state);
}

class State
{
private:
    STATES next_state = STATES::NONE_STATE;
protected:
    void Reset_Requested_State();
    void Request_Change_State(STATES new_state);
public:
    STATES Get_Requested_State();

    virtual ~State() = default;

    virtual STATES Get_State() const = 0;

    virtual void Enter() = 0;
    virtual void Exit() = 0;

    
};

#endif