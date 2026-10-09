#include "States.hpp"
#include "UWB.hpp"
#include "Positioning_Algebra_4LGHS.hpp"

void Request_LGHS_Positions_State::Enter()
{
    Reset_Requested_State();
    current_request_lgh_index = 0;
    ESP_MESSAGES::Send_Query_Position(current_request_lgh_index);
    Restart_UWB_As_Tag();
};

void Request_LGHS_Positions_State::Exit()
{
    current_request_lgh_index = 0;
    Reset_Requested_State();
}

void Request_LGHS_Positions_State::Task_1ms()
{

}

void Request_LGHS_Positions_State::Task_5ms()
{

}

void Request_LGHS_Positions_State::Task_20ms()
{

}

void Request_LGHS_Positions_State::Task_100ms()
{

}

void Request_LGHS_Positions_State::Task_1000ms()
{
    missed_responses_counter++;
    if (missed_responses_counter >= 2)
    {
        ESP_MESSAGES::Send_Query_Position(current_request_lgh_index);
        missed_responses_counter = 0;
    }
}

bool Request_LGHS_Positions_State::Check_For_Position_Response()
{
    if (Get_Requested_State() != STATES::NONE_STATE){return false;}
    ESP_Packet position_packet;
    if (!Search_For_ESP_Command(position_packet, ESP_Data_Commands::OBSERVER_RESPONSE_POSITION))
    { return false; }
    if (position_packet.data[ESP_Data_Setup::TRANSMITTER_ID] != current_request_lgh_index)
    { return false; }
    missed_responses_counter = 0;
    Position position;
    memcpy(&position.x, &position_packet.data[QUAD_0], sizeof(float));
    memcpy(&position.y, &position_packet.data[QUAD_1], sizeof(float));
    memcpy(&position.z, &position_packet.data[QUAD_2], sizeof(float));
    Update_LGH_Position(position, current_request_lgh_index);

    Handle_Increment_Next_LGH_Query();
    return true;
}

void Request_LGHS_Positions_State::Handle_Increment_Next_LGH_Query()
{
    if (current_request_lgh_index >= NUMBER_OF_LIGHTHOUSES-1)
    {
        ALGEBRA_4LGHS::Build_Constant_Matrices(lighthouse_positions_by_indices);
        Request_Change_State(STATES::FLIGHT_STATE);
        return;
    }

    current_request_lgh_index++;
    ESP_MESSAGES::Send_Query_Position(current_request_lgh_index);
}
