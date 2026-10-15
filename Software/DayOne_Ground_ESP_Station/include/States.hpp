#ifndef STATE_HPP
#define STATE_HPP

#include <Arduino.h>
#include "ESP_Structures.hpp"

enum class STATES
{
    INITIAL_STATE,
    REQUEST_DISTANCES_STATE,
    MATLAB_EXCHANGE_STATE,
    SEND_POSITIONS_STATE,

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
    virtual void Receive_Callback(ESP_Packet& esp_packet) = 0;
};


class Initial_State : public State
{
public:
    STATES Get_State() const override
    {
        return STATES::INITIAL_STATE;
    }

    void Enter() override;
    void Exit() override;

    void Receive_Callback(ESP_Packet& esp_packet) override;
};


class Request_Distances_State : public State
{
public:
    STATES Get_State() const override
    {
        return STATES::REQUEST_DISTANCES_STATE;
    }

    void Enter() override;
    void Exit() override;

    void Receive_Callback(ESP_Packet& esp_packet) override;
};

class Matlab_Exchange_State : public State
{
public:
    STATES Get_State() const override
    {
        return STATES::MATLAB_EXCHANGE_STATE;
    }

    void Enter() override;
    void Exit() override;

    void Receive_Callback(ESP_Packet& esp_packet) override;
};


class Send_Positions_State : public State
{
public:
    STATES Get_State() const override
    {
        return STATES::SEND_POSITIONS_STATE;
    }

    void Enter() override;
    void Exit() override;

    void Receive_Callback(ESP_Packet& esp_packet) override;
};


#include "States/Initial_State.tpp"

#endif