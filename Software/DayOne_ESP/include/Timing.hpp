#pragma once

#include "esp32-hal-timer.h"

class Timer
{
public:
    using Callback = void (*)(void*);

    Timer(uint16_t t_period_ms, Callback callback, void* context)
        : period_ms(t_period_ms),
          callback(callback),
          context(context),
          timer(nullptr)
    {}

    bool Initialize();

private:
    const uint16_t period_ms;
    hw_timer_t* timer;

    Callback callback;
    void* context;

    static void IRAM_ATTR TimerCallback(void* arg);
};


class Timer_Handler
{
public:
    Timer_Handler(uint16_t period_ms)
        : timer(period_ms, &Timer_Handler::TimerCallback, this)
    {}

    bool Initialize()
    {
        return timer.Initialize();
    }

private:
    Timer timer;

    static void IRAM_ATTR TimerCallback(void* arg)
    {
        auto* handler = static_cast<Timer_Handler*>(arg);
        handler->OnTimer();
    }

    void OnTimer()
    {
        // Your Timer_Handler code
    }
};

#include "Timing.tpp"