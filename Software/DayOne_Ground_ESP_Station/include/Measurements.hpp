#ifndef MEASUREMENTS_HPP
#define MEASUREMENTS_HPP

#include <Arduino.h>

#define NUMBER_OF_LIGHTHOUSES 4

#define POSITION_TRANSFER_DATA_LENGTH (3*NUMBER_OF_LIGHTHOUSES)
#define DISTANCES_TRANSFER_DATA_LENGTH ((NUMBER_OF_LIGHTHOUSES-1)*(NUMBER_OF_LIGHTHOUSES)/2)

struct Position
{
    float x, y, z;
};

extern Position LGHS_Positions[NUMBER_OF_LIGHTHOUSES];
extern float LGHS_Distances[NUMBER_OF_LIGHTHOUSES][NUMBER_OF_LIGHTHOUSES];

void Store_LGHS_Position(uint8_t lgh_index, float x, float y, float z);
void Store_LGHS_Distance(uint8_t target_lgh_index, uint8_t source_lgh_index, float d);

#endif