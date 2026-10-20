#ifndef UWB_CALCULATIONS_HPP
#define UWB_CALCULATIONS_HPP

#include "Configuration.hpp"
#include "UWB.hpp"
#include "Buffers.hpp"


#define MAX_NUMBER_OF_RANGES_PER_LIGHTHOUSE 16


const uint8_t MAX_MISSING_LGHS_PENALTY = 255;

extern Position current_calculated_position;
extern Position position_zero;
extern float last_averaged_ranges[NUMBER_OF_LIGHTHOUSES];

uint8_t Calculate_Position(
    Stack<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE> (&uwb_stacks)[NUMBER_OF_LIGHTHOUSES],
    Position &position
);

bool Estimate_Position_Initial_Point(
    const UWB_Measurement (&average_distances)[NUMBER_OF_LIGHTHOUSES],
    Position &estimated_position
);

uint8_t Get_Averaged_Results(
    Stack<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE> (&uwb_stacks)[NUMBER_OF_LIGHTHOUSES],
    UWB_Measurement (&average_distances)[NUMBER_OF_LIGHTHOUSES]
);

uint8_t Get_Number_Of_Present_Lighthouses(const Stack<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE>(&uwb_stacks)[NUMBER_OF_LIGHTHOUSES]);

uint8_t _get_penalty_from_missing_lighthouses(uint8_t present_lighthouses);
void _get_average_ranges(Stack<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE>(&uwb_stacks)[NUMBER_OF_LIGHTHOUSES],
            float* range_accumulator);

bool _estimate_position_from_average_ranges(const UWB_Measurement (&measurements)[NUMBER_OF_LIGHTHOUSES], Position& position);
bool _estimate_position_from_5_measurements(const UWB_Measurement (&measurements)[NUMBER_OF_LIGHTHOUSES], Position& position);
bool _estimate_position_from_4_measurements(const UWB_Measurement (&measurements)[NUMBER_OF_LIGHTHOUSES], Position& position);
bool _estimate_position_from_3_measurements(const UWB_Measurement (&measurements)[NUMBER_OF_LIGHTHOUSES], Position& position);

#endif