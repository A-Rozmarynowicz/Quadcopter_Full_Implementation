#include "Configuration.hpp"
#include "Timing.hpp"
#include "ESP_Communication.hpp"
#include "UWB_Calculations.hpp"
#include "UWB.hpp"

void Task_1ms();
void Task_5ms();
void Task_20ms();
void Task_100ms();
void Task_1s();


Timer_Handler::Timer_Config configs[] =
{
    { 1,    Task_1ms   },
    { 5,    Task_5ms   },
    { 20,   Task_20ms  },
    { 100,  Task_100ms },
    { 1000, Task_1s    }
};

Timer_Handler timer_handler(
    0,
    configs,
    sizeof(configs) / sizeof(configs[0])
);


void setup() {
    Serial.begin(115200);
    timer_handler.Initialize();
    initialize_esp_communication();

    Serial.println("Initialization complete\n");
}

void loop() {
    timer_handler.Update();
}

void Task_1ms()
{
    // Serial.println("1ms");
}

void Task_5ms()
{
    // Serial.println("5ms");
}

void Task_20ms()
{
    // Serial.println("20ms");
}

void Task_100ms()
{
    // Serial.println("100ms");
}

void Task_1s()
{
    // Serial.println("1 second");
}