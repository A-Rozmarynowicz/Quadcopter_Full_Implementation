#include "States.hpp"

void Initial_State::Enter()
{
    Reset_Requested_State();
    // ESP_MESSAGES::Send_Wakeup_Reckon(BROADCAST_RECEIVER_ID);
};

void Initial_State::Exit()
{
    Reset_Requested_State();
}

void Initial_State::Task_1ms()
{

}

void Initial_State::Task_5ms()
{
    // I2C_MESSAGES::Send_Heartbeat();
}

void Initial_State::Task_10ms()
{

}

void Initial_State::Task_20ms()
{

}

void Initial_State::Task_100ms()
{
    // Check_For_Wakeup_Response();
}

void Initial_State::Task_1000ms()
{
    Position p;
    p.x = 67.0;
    p.y = 69.0;
    p.z = 420.0;
    // I2C_MESSAGES::Send_Current_Position(p);
    _read_I2C_full_telemetry();
    // ESP_MESSAGES::Send_Wakeup_Reckon(BROADCAST_RECEIVER_ID);
}

void Initial_State::Check_For_Wakeup_Response()
{
    ESP_Packet proxy;
    Serial.printf("ESP rec stack Size: %d\n", esp_receive_queue.size());
    if (Search_For_ESP_Command(proxy, ESP_Data_Commands::READY_FOR_OBSERVER))
    {
        Request_Change_State(STATES::REQUEST_LGHS_POSITIONS_STATE);
    }
}