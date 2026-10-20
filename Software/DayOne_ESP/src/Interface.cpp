#include "Interface.hpp"

bool blinking = false;

void Init()
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, 0);
}

void LED_On()
{
    digitalWrite(LED_PIN, 1);
}

void LED_Off()
{
    digitalWrite(LED_PIN, 0);
}

void LED_Toggle()
{
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

void LED_Blink()
{
    blinking = true;
    LED_Toggle();
}


void LED_Blink_Task()
{
    if (blinking)
    {
        LED_Toggle();
    }
}
