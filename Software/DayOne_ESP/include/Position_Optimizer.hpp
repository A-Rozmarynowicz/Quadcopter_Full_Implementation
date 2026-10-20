
#ifndef POSITION_OPTIMIZER_HPP
#define POSITION_OPTIMIZER_HPP

#include "Configuration.hpp"
#include "UWB.hpp"

bool Optimize_Position(
    UWB_Measurement (&measurements)[NUMBER_OF_LIGHTHOUSES],
    Position (&LGHS_positions)[NUMBER_OF_LIGHTHOUSES],
    float (&weights)[NUMBER_OF_LIGHTHOUSES],
    Position& starting_point,
    Position& estimated_position);

#endif
