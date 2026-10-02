#include "UWB_Calculations.hpp"

Position current_calculated_position;

uint8_t Calculate_Position(Queue<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE> (&uwb_stacks)[NUMBER_OF_LIGHTHOUSES])
{
    uint8_t present_lighthouses = Get_Number_Of_Present_Lighthouses(uwb_stacks);
    if (present_lighthouses < NUMBER_OF_LIGHTHOUSES - 1)
    {
        return _get_penalty_from_missing_lighthouses(present_lighthouses);
    }

    float range_accumulator[NUMBER_OF_LIGHTHOUSES] = {0.0f};
    _get_average_ranges(uwb_stacks, range_accumulator);
    UWB_Measurement average_distances[NUMBER_OF_LIGHTHOUSES] = {};

    for (uint8_t i=0; i<NUMBER_OF_LIGHTHOUSES; i++)
    {
        average_distances[i].range = range_accumulator[i];
        average_distances[i].lgh_index = i;
    }

    return true;
}

uint8_t Get_Number_Of_Present_Lighthouses(Queue<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE>(&uwb_stacks)[NUMBER_OF_LIGHTHOUSES])
{
    uint8_t present_lighthouses = 0;
    for (uint8_t i=0; i<NUMBER_OF_LIGHTHOUSES; i++)
    {
        if (!uwb_stacks[i].empty())
        {
            present_lighthouses += 1;
        }
    }
    return present_lighthouses;
}

uint8_t _get_penalty_from_missing_lighthouses(uint8_t present_lighthouses)
{
    uint8_t missing_lighthouses = NUMBER_OF_LIGHTHOUSES - present_lighthouses;
    switch (missing_lighthouses)
    {
        case 0:
            return 0;
        case 1:
            return 10;
        case 2:
            return 255;
        case 3:
            return 255;
    }
    return 255;
}

void _get_average_ranges(Queue<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE> (&uwb_stacks)[NUMBER_OF_LIGHTHOUSES],
    float* range_accumulator)
{
    uint8_t count_accumulator[NUMBER_OF_LIGHTHOUSES] = {0};
    for (uint8_t i=0; i<NUMBER_OF_LIGHTHOUSES; i++)
    {
        if (uwb_stacks[i].empty())
        {
            continue;
        }
        for (uint8_t j=0; j<MAX_NUMBER_OF_RANGES_PER_LIGHTHOUSE; j++)
        {
            UWB_Measurement uwb;
            if (!uwb_stacks[i].pop(uwb))
            {
                break;
            }
            range_accumulator[i] += uwb.range;
            count_accumulator[i]++;
        };
        uwb_stacks[i].flush(); // @todo: for sure?
        if (count_accumulator[i] == 0)
        {
            range_accumulator[i] = 0.0f;
            continue;
        }
        range_accumulator[i] = range_accumulator[i] / count_accumulator[i];
    };

}
