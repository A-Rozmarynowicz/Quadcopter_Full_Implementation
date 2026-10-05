#pragma once

#include "Configuration.hpp"
#include "ESP_Communication.hpp"
#include "STM_Communication.hpp"

#define STATE_MAX_QUEUE_SEARCH_DEPTH 16

enum class STATES
{
    INITIAL_STATE,
    REQUEST_LGHS_POSITIONS_STATE,

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
    uint8_t Get_Receive_Queue_Search_Depth();
    bool Search_For_ESP_Command(ESP_Packet &packet, ESP_Data_Commands command);
public:
    STATES Get_Requested_State();

    virtual ~State() = default;

    virtual STATES Get_State() const = 0;

    virtual void Enter() = 0;
    virtual void Exit() = 0;

    virtual void Task_1ms() = 0;
    virtual void Task_5ms() = 0;
    virtual void Task_20ms() = 0;
    virtual void Task_100ms() = 0;
    virtual void Task_1000ms() = 0;
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

    void Task_1ms() override;
    void Task_5ms() override;
    void Task_20ms() override;
    void Task_100ms() override;
    void Task_1000ms() override;

private:
    void Check_For_Wakeup_Response();
};

class Request_LGHS_Positions_State : public State
{
    public:
    STATES Get_State() const override
    {
        return STATES::REQUEST_LGHS_POSITIONS_STATE;
    }

    void Enter() override;
    void Exit() override;

    void Task_1ms() override;
    void Task_5ms() override;
    void Task_20ms() override;
    void Task_100ms() override;
    void Task_1000ms() override;

    private:
    uint8_t current_request_lgh_index = 0;
    
    bool Check_For_Position_Response();
    void Handle_Increment_Next_LGH_Query();
};


#include "States/Base_State.tpp"
#include "States/Initial_State.tpp"
#include "States/Request_LGHS_Positions_State.tpp"