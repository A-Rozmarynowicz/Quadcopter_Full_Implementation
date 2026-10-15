#ifndef STATE_HPP
#define STATE_HPP

#include <Arduino.h>
#include "ESP_Structures.hpp"
#include "Serial_Handler.hpp"
#include "Measurements.hpp"
#include "ESP_Communication.hpp"


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

    void Update();
    virtual void Handle_Serial_Message(uint8_t command, const float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER], uint8_t value_count) = 0;
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
private:
    void Handle_Serial_Message(uint8_t command, const float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER], uint8_t value_count) override;
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
private:
    void Handle_Serial_Message(uint8_t command, const float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER], uint8_t value_count) override;
    void Update_Distances(ESP_Packet& packet);
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
private:
    void Handle_Serial_Message(uint8_t command, const float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER], uint8_t value_count) override;
    void Handle_Serial_Position_Transfer(const float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER]);
    void Send_Serial_Distances();
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
private:
    uint8_t current_lgh_index = 0;
    void Handle_Serial_Message(uint8_t command, const float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER], uint8_t value_count) override;
};

class Manual_Operation_State : public State
{
    public:
    STATES Get_State() const override
    {
        return STATES::SEND_POSITIONS_STATE;
    }

    void Enter() override;
    void Exit() override;

    void Receive_Callback(ESP_Packet& esp_packet) override;
private:
    void Handle_Serial_Message(uint8_t command, const float (&values)[MAX_NUMBER_OF_VALUES_IN_TRANSFER], uint8_t value_count) override;
};


#endif