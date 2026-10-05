#include "States.hpp"


void Initial_State::Enter()
{
    Reset_Requested_State();
    ESP_MESSAGES::Send_Wakeup_Reckon(BROADCAST_RECEIVER_ID);
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

}

void Initial_State::Task_20ms()
{

}

void Initial_State::Task_100ms()
{
    if (esp_receive_queue.empty()) {return;}
    uint8_t upper_index_limit = STATE_MAX_QUEUE_SEARCH_DEPTH;
    if (esp_receive_queue.size() < upper_index_limit)
    {
        upper_index_limit = esp_receive_queue.size();
    }
    for (uint8_t i=0; i < upper_index_limit; i++)
    {
        ESP_Packet receive = esp_receive_queue[esp_receive_queue.get_head_offset_index(i)];
        if (receive.data[ESP_Data_Setup::COMMAND] == ESP_Data_Commands::READY_FOR_OBSERVER)
        {
            Request_Change_State(STATES::REQUEST_LGHS_POSITIONS_STATE);
        }
    }
}

void Initial_State::Task_1000ms()
{

}