#include "UWB_Precision_Manager.hpp"

Queue<float, HISTORY_RANGES_QUEUE_SIZE> history_ranges[NUMBER_OF_LIGHTHOUSES] = {};
