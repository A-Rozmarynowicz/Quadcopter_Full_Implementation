#include "UWB_Calculations.hpp"

Position current_calculated_position;

bool Calculate_Position(Queue<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE> (&uwb_queues)[NUMBER_OF_LIGHTHOUSES])
{

    for (uint8_t i=0;i<NUMBER_OF_LIGHTHOUSES;i++)
    {
        if (uwb_queues[i].empty())
        {
            return false;
        }
    }

    float range_accumulator[NUMBER_OF_LIGHTHOUSES] = {0.0f};
    _get_average_ranges(uwb_queues, range_accumulator);

    return true;
}

void _get_average_ranges(Queue<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE> (&uwb_queues)[NUMBER_OF_LIGHTHOUSES],
    float* range_accumulator)
{
    uint8_t count_accumulator[NUMBER_OF_LIGHTHOUSES] = {0};
    for (uint8_t i=0; i<NUMBER_OF_LIGHTHOUSES; i++)
    {
        bool empty = false;
        for (uint8_t j=0; j<MAX_NUMBER_OF_RANGES_PER_LIGHTHOUSE; j++)
        {
            UWB_Measurement uwb;
            if (!uwb_queues[i].pop(uwb))
            {
                break;
            }
            range_accumulator[i] += uwb.range;
            count_accumulator[i]++;
        };
        uwb_queues[i].flush(); // @todo: for sure?
        range_accumulator[i] = range_accumulator[i] / count_accumulator[i];
    };

}
