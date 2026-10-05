#pragma once

#include "Configuration.hpp"

enum class STATES
{
    NONE_STATE,
    INITIAL_STATE,
    REQUEST_LGHS_POSITIONS_STATE,
};

class State
{
protected:
    STATES next_state = STATES::NONE_STATE;
public:
    virtual ~State() = default;

    virtual STATES GetState() const = 0;

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
    STATES GetState() const override
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


#include "States.tpp"