#ifndef UWB_PRECISION_MANAGER_HPP
#define UWB_PRECISION_MANAGER_HPP

#include "Configuration.hpp"
#include "Buffers.hpp"

#define HISTORY_RANGES_QUEUE_SIZE 8

extern Queue<float, HISTORY_RANGES_QUEUE_SIZE> history_ranges[NUMBER_OF_LIGHTHOUSES];


#endif