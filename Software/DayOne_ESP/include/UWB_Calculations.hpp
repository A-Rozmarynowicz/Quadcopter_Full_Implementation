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


uint8_t Calculate_Position(Queue<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE>(&uwb_stacks)[NUMBER_OF_LIGHTHOUSES]);
uint8_t Get_Number_Of_Present_Lighthouses(Queue<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE>(&uwb_stacks)[NUMBER_OF_LIGHTHOUSES]);

uint8_t _get_penalty_from_missing_lighthouses(uint8_t present_lighthouses);
void _get_average_ranges(Queue<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE>(&uwb_stacks)[NUMBER_OF_LIGHTHOUSES],
    float* range_accumulator);