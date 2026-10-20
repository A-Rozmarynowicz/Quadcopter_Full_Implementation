#ifndef UWB_PRECISION_MANAGER_HPP
#define UWB_PRECISION_MANAGER_HPP

#include "Configuration.hpp"
#include "Buffers.hpp"

#define HISTORY_RANGES_QUEUE_SIZE 8

extern Queue<float, HISTORY_RANGES_QUEUE_SIZE> history_ranges[NUMBER_OF_LIGHTHOUSES];

void Get_Weights_For_Optimizer(float (&weights)[NUMBER_OF_LIGHTHOUSES])
{
    for (uint8_t i=0; i<NUMBER_OF_LIGHTHOUSES; i++)
    {
        weights[i] = {1.0};
    }
}

#endif