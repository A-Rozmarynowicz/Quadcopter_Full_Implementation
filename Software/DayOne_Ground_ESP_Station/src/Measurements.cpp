#include "Measurements.hpp"

Position LGHS_Positions[NUMBER_OF_LIGHTHOUSES];
float LGHS_Distances[NUMBER_OF_LIGHTHOUSES][NUMBER_OF_LIGHTHOUSES];

void Store_LGHS_Position(uint8_t lgh_index, float x, float y, float z)
{
    LGHS_Positions[lgh_index].x = x;
    LGHS_Positions[lgh_index].y = y;
    LGHS_Positions[lgh_index].z = z;
}

void Store_LGHS_Distance(uint8_t target_lgh_index, uint8_t source_lgh_index, float d)
{
    LGHS_Distances[target_lgh_index][source_lgh_index] = d;
}