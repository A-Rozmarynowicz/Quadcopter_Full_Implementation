#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "Configuration.hpp"

#define LED_PIN 27

void Init();
void LED_On();
void LED_Off();
void LED_Toggle();
void LED_Blink();

void LED_Blink_Task();

#endif