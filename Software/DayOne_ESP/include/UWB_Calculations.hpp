#pragma once

#include "Configuration.hpp"
#include "Buffers.hpp"
#include "UWB.hpp"

#define MAX_NUMBER_OF_RANGES_PER_LIGHTHOUSE 16

struct Position
{
    float x, y, z;
};


extern Position current_calculated_position;


bool Calculate_Position(Queue<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE>(&uwb_queues)[NUMBER_OF_LIGHTHOUSES]);

void _get_average_ranges(Queue<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE>(&uwb_queues)[NUMBER_OF_LIGHTHOUSES],
    float* range_accumulator);