#pragma once

#include "Configuration.hpp"
#include <SPI.h>
#include "DW1000Ranging.h"
#include "Buffers.hpp"

#define UWB_ADDRESS_LENGTH 8
#define UWB_RESPONSE_DELAY_TIME 750
#define UWB_MEASUREMENT_STACK_SIZE 128

const uint16_t BASE_ANTENNA_DELAY_VALUE = 16350;
const float ERROR_COMPENSATION_PARAMETER_A = 0.9539;
const float ERROR_COMPENSATION_PARAMETER_B = -0.5259;
const float MINIMUM_RANGE = 0.05;
const float MAXIMUM_RANGE = 20.0;

const int8_t PIN_RST = 22;  // reset pin
const int8_t PIN_IRQ = 17;   // irq pin
const int8_t PIN_SS = 5;    // spi select pin

const int8_t PIN_MOSI = 23;
const int8_t PIN_MISO = 19;
const int8_t PIN_SCK = 18;

extern const uint8_t uwb_addresses_from_LGH[NUMBER_OF_LIGHTHOUSES][UWB_ADDRESS_LENGTH];
extern const uint8_t drone_address[UWB_ADDRESS_LENGTH];


extern bool uwb_enable;
const byte CHANNEL = DW1000.CHANNEL_5;
extern const byte* UWB_TRANSMIT_MODE;

struct UWB_Measurement
{
    float range = 0.0f;
    uint8_t lgh_index = 0;
};

struct Position
{
    float x, y, z;
    Position operator-(const Position& p2) const
    {
        return {
            x - p2.x,
            y - p2.y,
            z - p2.z
        };
    }
};

extern Position lighthouse_positions_by_indices[NUMBER_OF_LIGHTHOUSES];

extern Stack<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE> uwb_measurement_stacks[NUMBER_OF_LIGHTHOUSES];

void Initialize_UWB();
void Update_UWB();
void Restart_UWB_As_Tag();

bool Is_UWB_Enabled();
void Disable_UWB();
void Enable_UWB();

uint16_t Get_Short_Address_From_Long(const uint8_t* address);
int8_t Get_LGH_From_Short_Address(const uint16_t short_address);
int8_t Get_LGH_From_Address(const uint8_t* address);
bool Are_Addresses_Equal(uint8_t* first, uint8_t* second);
void Update_LGH_Position(Position &position, uint8_t index);

float Get_Biased_Range_Value(float range);

void _new_range();
void _add_range(float range, uint8_t lgh_index);
void _new_blink(DW1000Device* device);
void _new_device(DW1000Device* device);
void _inactive_device(DW1000Device* device);
void _reset_DW1000();

void _format_lgh_address_to_string(uint8_t lgh_index, char address_str[24]);
void _format_drone_address_to_string(char address_str[24]);

