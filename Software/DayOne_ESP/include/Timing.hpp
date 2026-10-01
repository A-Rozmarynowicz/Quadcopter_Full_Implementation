#pragma once

#include "esp32-hal-timer.h"
#include <stddef.h>

class Timer_Handler
{
public:
    using Callback = void (*)();

    struct Timer_Config
    {
        uint32_t period_ms;
        Callback callback;
    };

    static constexpr size_t MAX_TIMERS = 16;

    Timer_Handler(
        uint8_t hardware_timer_number,
        const Timer_Config* configs,
        size_t count
    );

    bool Initialize();

    void Update();

private:
    struct Software_Timer
    {
        uint32_t period_ms;
        uint32_t elapsed_ms;

        Callback callback;

        volatile bool pending;
    };

    hw_timer_t* timer;

    uint8_t timer_number;

    Software_Timer timers[MAX_TIMERS];
    size_t timer_count;

    static Timer_Handler* instance;

    static void IRAM_ATTR TimerISR();

    void IRAM_ATTR OnTimerInterrupt();
};

#include "Timing.tpp"