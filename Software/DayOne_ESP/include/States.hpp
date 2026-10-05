#pragma once

#include "Configuration.hpp"
#include "ESP_Communication.hpp"
#include "STM_Communication.hpp"


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
};


#include "States/Base_State.tpp"
#include "States/Initial_State.tpp"
#include "States/Request_LGHS_Positions_State.tpp"