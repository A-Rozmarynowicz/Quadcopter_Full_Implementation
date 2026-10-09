#include "States.hpp"

void State::Request_Change_State(STATES new_state)
{
    next_state = new_state;
}

void State::Reset_Requested_State()
{
    next_state = STATES::NONE_STATE;
}

STATES State::Get_Requested_State()
{
    return next_state;
}

uint8_t State::Get_Receive_Queue_Search_Depth()
{
    uint8_t search_depth = STATE_MAX_QUEUE_SEARCH_DEPTH;
    if (esp_receive_queue.size() < search_depth)
    {
        search_depth = esp_receive_queue.size();
    }

    return search_depth;
}

bool State::Search_For_ESP_Command(ESP_Packet &packet, ESP_Data_Commands command)
{
    if (esp_receive_queue.empty()) {return false;}
    uint8_t upper_index_limit = Get_Receive_Queue_Search_Depth();
    for (uint8_t i=0; i < upper_index_limit; i++)
    {
        ESP_Packet receive = esp_receive_queue[esp_receive_queue.get_head_offset_index(i)];
        esp_receive_queue[esp_receive_queue.get_head_offset_index(i)].Increase_Flush_Value();
        if (receive.data[ESP_Data_Setup::COMMAND] == command)
        {
            packet = receive;
            if (i==0)
            {
                ESP_Packet proxy;
                esp_receive_queue.pop(proxy);
            }
            return true;
        }
    }
    return false;
}