#pragma once

#include "Configuration.hpp"
#include "Buffers.hpp"
#include "UWB.hpp"

#define MAX_NUMBER_OF_RANGES_PER_LIGHTHOUSE 16

extern Position current_calculated_position;

uint8_t Calculate_Position(Stack<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE>(&uwb_stacks)[NUMBER_OF_LIGHTHOUSES]);
uint8_t Get_Number_Of_Present_Lighthouses(Stack<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE>(&uwb_stacks)[NUMBER_OF_LIGHTHOUSES]);

uint8_t _get_penalty_from_missing_lighthouses(uint8_t present_lighthouses);
void _get_average_ranges(Stack<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE>(&uwb_stacks)[NUMBER_OF_LIGHTHOUSES],
            float* range_accumulator);

bool _estimate_position_from_average_ranges(UWB_Measurement (&measurements)[NUMBER_OF_LIGHTHOUSES], Position& position);
bool _estimate_position_from_4_measurements(UWB_Measurement (&measurements)[NUMBER_OF_LIGHTHOUSES], Position& position);
bool _estimate_position_from_3_measurements(UWB_Measurement (&measurements)[NUMBER_OF_LIGHTHOUSES], Position& position);