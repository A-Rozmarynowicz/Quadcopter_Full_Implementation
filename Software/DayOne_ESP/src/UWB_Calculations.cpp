#include "UWB_Calculations.hpp"
#include "Positioning_Algebra_4LGHS.hpp"
#include "Positioning_Algebra_5LGHS.hpp"
#include "Position_Optimizer.hpp"

Position current_calculated_position;
Position position_zero;
float last_averaged_ranges[NUMBER_OF_LIGHTHOUSES] = {0.0f};

uint8_t Calculate_Position(
    Stack<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE> (&uwb_stacks)[NUMBER_OF_LIGHTHOUSES],
    Position &position
)
{
    Position estimated_position;
    UWB_Measurement average_distances[NUMBER_OF_LIGHTHOUSES] = {};
    float weights[NUMBER_OF_LIGHTHOUSES] = {};

    uint8_t missing_lghs_penalty = Get_Averaged_Results(uwb_stacks, average_distances);
    Estimate_Position_Initial_Point(average_distances, estimated_position);
    Optimize_Position(average_distances, lighthouse_positions_by_indices, weights, estimated_position, position);
    return missing_lghs_penalty;
}


bool Estimate_Position_Initial_Point(
    const UWB_Measurement (&average_distances)[NUMBER_OF_LIGHTHOUSES],
    Position &estimated_position)
{
    Position estimated_position;
    bool success = _estimate_position_from_average_ranges(average_distances, estimated_position);
    estimated_position = estimated_position;
    return success;
}


uint8_t Get_Averaged_Results(
    Stack<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE> (&uwb_stacks)[NUMBER_OF_LIGHTHOUSES],
    UWB_Measurement (&average_distances)[NUMBER_OF_LIGHTHOUSES]
)
{
    uint8_t present_lighthouses = Get_Number_Of_Present_Lighthouses(uwb_stacks);
    uint8_t missing_lghs_penalty = _get_penalty_from_missing_lighthouses(present_lighthouses);

    float range_accumulator[NUMBER_OF_LIGHTHOUSES] = {0.0f};
    _get_average_ranges(uwb_stacks, range_accumulator);

    for (uint8_t i=0; i<NUMBER_OF_LIGHTHOUSES; i++)
    {
        average_distances[i].range = range_accumulator[i];
        average_distances[i].lgh_index = i;
        last_averaged_ranges[i] = range_accumulator[i];
        Serial.printf("Range to %d = %0.2f \n", i, range_accumulator[i]);
    }

    return missing_lghs_penalty;
}

uint8_t Get_Number_Of_Present_Lighthouses(const Stack<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE>(&uwb_stacks)[NUMBER_OF_LIGHTHOUSES])
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
            return 200;
        case 3:
            return 255;
    }
    return 255;
}

void _get_average_ranges(
    Stack<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE> (&uwb_stacks)[NUMBER_OF_LIGHTHOUSES],
    float* range_accumulator
)
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


bool _estimate_position_from_average_ranges(const UWB_Measurement (&measurements)[NUMBER_OF_LIGHTHOUSES], Position& position)
{
    if (NUMBER_OF_LIGHTHOUSES == 5)
    {
        return _estimate_position_from_5_measurements(measurements, position);
    }
    else if (NUMBER_OF_LIGHTHOUSES == 4)
    {
        return _estimate_position_from_4_measurements(measurements, position);
    }
}

bool _estimate_position_from_5_measurements(const UWB_Measurement (&measurements)[NUMBER_OF_LIGHTHOUSES], Position &position)
{
    #if (NUMBER_OF_LIGHTHOUSES == 5)
        ALGEBRA_5LGHS::Estimate_Position(measurements, position);
    #endif
    return true;
}

bool _estimate_position_from_4_measurements(const UWB_Measurement (&measurements)[NUMBER_OF_LIGHTHOUSES], Position& position)
{
    #if (NUMBER_OF_LIGHTHOUSES == 4)
        ALGEBRA_4LGHS::Estimate_Position(measurements, position);
    #endif
    return true;
}

bool _estimate_position_from_3_measurements(const UWB_Measurement (&measurements)[NUMBER_OF_LIGHTHOUSES], Position& position)
{
    return false;
}