#include "Timing.hpp"



static constexpr uint16_t TIMER_PRESCALER = 8000;

// 80 MHz / 8000 = 10 kHz
// 1 tick = 0.1 ms
Timer_Handler* Timer_Handler::instance = nullptr;


Timer_Handler::Timer_Handler(
    uint8_t hardware_timer_number,
    const Timer_Config* configs,
    size_t count
)
    : timer(nullptr),
      timer_number(hardware_timer_number),
      timer_count(0)
{
    if (count > MAX_TIMERS)
        count = MAX_TIMERS;

    for (size_t i = 0; i < count; ++i)
    {
        timers[i].period_ms = configs[i].period_ms;
        timers[i].elapsed_ms = 0;
        timers[i].callback = configs[i].callback;
        timers[i].pending = false;
    }

    timer_count = count;
}


bool Timer_Handler::Initialize()
{
    instance = this;

    timer = timerBegin(
        timer_number,
        TIMER_PRESCALER,
        true
    );

    if (timer == nullptr)
        return false;

    timerAttachInterrupt(
        timer,
        &Timer_Handler::TimerISR,
        true
    );

    // 1 ms hardware tick
    // 10 ticks × 0.1 ms = 1 ms
    timerAlarmWrite(
        timer,
        10,
        true
    );

    timerAlarmEnable(timer);

    return true;
}


void IRAM_ATTR Timer_Handler::TimerISR()
{
    if (instance != nullptr)
    {
        instance->OnTimerInterrupt();
    }
}


void IRAM_ATTR Timer_Handler::OnTimerInterrupt()
{
    for (size_t i = 0; i < timer_count; ++i)
    {
        Software_Timer& t = timers[i];

        t.elapsed_ms++;

        if (t.elapsed_ms >= t.period_ms)
        {
            t.elapsed_ms = 0;
            t.pending = true;
        }
    }
}


void Timer_Handler::Update()
{
    for (size_t i = 0; i < timer_count; ++i)
    {
        Software_Timer& t = timers[i];

        if (t.pending)
        {
            t.pending = false;

            if (t.callback != nullptr)
            {
                t.callback();
            }
        }
    }
}