#include "UWB_Calculations.hpp"
#include "Positioning_Algebra_4LGHS.hpp"

Position current_calculated_position;
Position position_zero;
float last_averaged_ranges[NUMBER_OF_LIGHTHOUSES] = {0.0f};

uint8_t Calculate_Position(Stack<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE> (&uwb_stacks)[NUMBER_OF_LIGHTHOUSES], Position &position)
{
    uint8_t present_lighthouses = Get_Number_Of_Present_Lighthouses(uwb_stacks);
    uint8_t missing_lghs_penalty = _get_penalty_from_missing_lighthouses(present_lighthouses);
    if (present_lighthouses < NUMBER_OF_LIGHTHOUSES - 1)
    {
        return missing_lghs_penalty;
    }

    float range_accumulator[NUMBER_OF_LIGHTHOUSES] = {0.0f};
    _get_average_ranges(uwb_stacks, range_accumulator);
    UWB_Measurement average_distances[NUMBER_OF_LIGHTHOUSES] = {};

    for (uint8_t i=0; i<NUMBER_OF_LIGHTHOUSES; i++)
    {
        average_distances[i].range = range_accumulator[i];
        average_distances[i].lgh_index = i;
        last_averaged_ranges[i] = range_accumulator[i];
        Serial.printf("Range to %d = %0.2f \n", i, range_accumulator[i]);
    }

    Position estimated_position;
    bool success = _estimate_position_from_average_ranges(average_distances, estimated_position, present_lighthouses);
    if (success)
    {
        position = estimated_position;
        return missing_lghs_penalty;
    }
    return 255;
}

uint8_t Get_Number_Of_Present_Lighthouses(Stack<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE>(&uwb_stacks)[NUMBER_OF_LIGHTHOUSES])
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

void _get_average_ranges(Stack<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE> (&uwb_stacks)[NUMBER_OF_LIGHTHOUSES],
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
            range_accumulator[i] = last_averaged_ranges[i];
            continue;
        }
        range_accumulator[i] = range_accumulator[i] / count_accumulator[i];
    };

}


bool _estimate_position_from_average_ranges(UWB_Measurement (&measurements)[NUMBER_OF_LIGHTHOUSES], Position& position, uint8_t present_lighthouses)
{
    return _estimate_position_from_4_measurements(measurements, position);
}

bool _estimate_position_from_4_measurements(UWB_Measurement (&measurements)[NUMBER_OF_LIGHTHOUSES], Position& position)
{
    ALGEBRA_4LGHS::Estimate_Position(measurements, position);
    return true;
}

bool _estimate_position_from_3_measurements(UWB_Measurement (&measurements)[NUMBER_OF_LIGHTHOUSES], Position& position)
{
    return false;
}