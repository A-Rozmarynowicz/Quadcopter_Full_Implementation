#include "Configuration.hpp"
#include "Timing.hpp"
#include "State_Machine.hpp"
#include "UWB.hpp"
#include "ESP_Communication.hpp"
#include "STM_Communication.hpp"

void Task_1ms();
void Task_5ms();
void Task_20ms();
void Task_100ms();
void Task_1000ms();


Timer_Handler::Timer_Config configs[] =
{
    { 1,    Task_1ms   },
    { 5,    Task_5ms   },
    { 20,   Task_20ms  },
    { 100,  Task_100ms },
    { 1000, Task_1000ms    }
};

Timer_Handler timer_handler(
    0,
    configs,
    sizeof(configs) / sizeof(configs[0])
);

State_Machine state_machine;


void setup() {
    Serial.begin(115200);
    timer_handler.Initialize();
    Initialize_ESP_Communication();
    Initialize_I2C();
    Initialize_UWB();
    state_machine.State_Machine_Initialize();
    Serial.println("Initialization complete\n");
}

void loop() {
    timer_handler.Update();
    Update_UWB();
}

void Task_1ms()
{
    state_machine.Handle_State_Change_Request();
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

void Task_1000ms()
{
    Flush_Unused_ESP_Received_Packets();
}