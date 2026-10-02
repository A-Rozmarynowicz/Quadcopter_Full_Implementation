#include "UWB.hpp"

#pragma region Variables
bool uwb_enable = false;

const byte* UWB_TRANSMIT_MODE = DW1000Class::MODE_SHORTDATA_FAST_ACCURACY;

const uint8_t uwb_addresses_from_LGH[NUMBER_OF_LIGHTHOUSES][UWB_ADDRESS_LENGTH] =
{
    {0x82, 0x17, 0x5B, 0xD5, 0xA9, 0x9A, 0xE2, 0x9C},
    {0x7D, 0x00, 0x22, 0xEA, 0x82, 0x60, 0x3B, 0x9C},
    {0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6, 0x07, 0x18},
    {0x3C, 0x9A, 0x44, 0x10, 0xFE, 0x02, 0x8D, 0x6F}
};

extern const uint8_t drone_address[UWB_ADDRESS_LENGTH] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF1};

Stack<UWB_Measurement, UWB_MEASUREMENT_STACK_SIZE> uwb_measurement_stacks[NUMBER_OF_LIGHTHOUSES];

void Initialize_UWB()
{
    pinMode(PIN_RST, OUTPUT);
    digitalWrite(PIN_RST, HIGH);
}

void Update_UWB()
{
    DW1000Ranging.loop();
}

bool Is_UWB_Enabled(){return uwb_enable;}

#pragma endregion

#pragma region Chip interface

void Restart_UWB_As_Tag()
{
    _reset_DW1000();

    SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_SS);
    SPI.setFrequency(4000000);
    DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ);

    DW1000Ranging.attachNewRange(_new_range);
    DW1000Ranging.attachNewDevice(_new_device);
    DW1000Ranging.attachInactiveDevice(_inactive_device);
    DW1000.setAntennaDelay(BASE_ANTENNA_DELAY_VALUE);

    char address_str[24] = {0};
    _format_drone_address_to_string(address_str);
    DW1000Ranging.startAsTag(
        address_str,
        UWB_TRANSMIT_MODE,
        false
    );

    DW1000.setChannel(CHANNEL);
    DW1000.useSmartPower(false);
    uint32_t maxPower = 0x26486A6A;
    DW1000.writeBytes(0x1E, 0x00, (byte*)&maxPower, 4);
    DW1000.commitConfiguration();
}

void Disable_UWB()
{
    uwb_enable = false;
}

void Enable_UWB()
{
    uwb_enable = true;
}

void _reset_DW1000()
{
    digitalWrite(PIN_RST, LOW);
    delay(50);
    digitalWrite(PIN_RST, HIGH);
    delay(50);
}

#pragma endregion

#pragma region Calculations

float Get_Biased_Range_Value(float range)
{
    float result = range*ERROR_COMPENSATION_PARAMETER_A + ERROR_COMPENSATION_PARAMETER_B;
    if (result < MINIMUM_RANGE)
    {
        return MINIMUM_RANGE;
    }
    else if (result > MAXIMUM_RANGE)
    {
        return MAXIMUM_RANGE;
    }
    return result;
}

#pragma endregion

#pragma region Callbacks

void _new_blink(DW1000Device* device) {}

void _new_range()
{
    uint16_t device = DW1000Ranging.getDistantDevice()->getShortAddress();
    float range = DW1000Ranging.getDistantDevice()->getRange();
    range = Get_Biased_Range_Value(range);
    float rx_power = DW1000Ranging.getDistantDevice()->getRXPower();
    int8_t lgh_index = Get_LGH_From_Short_Address(device);

    if (lgh_index < 0 || lgh_index > NUMBER_OF_LIGHTHOUSES - 1)
    {
        return; // @todo: error
    }

    UWB_Measurement uwb_measurement;
    uwb_measurement.lgh_index = lgh_index;
    uwb_measurement.range = range;

    uwb_measurement_stacks[lgh_index].push(uwb_measurement);
    // State_UWB_New_Range(device, range, rx_power);
}

void _new_device(DW1000Device* device) {}

void _inactive_device(DW1000Device* device) {}

#pragma endregion

#pragma region Address handling

uint16_t Get_Short_Address_From_Long(const uint8_t* address)
{
    uint16_t short_address = address[1]*256 + address[0];
    return short_address;
}


int8_t Get_LGH_From_Short_Address(const uint16_t short_address)
{
    for (uint8_t i=0;i<NUMBER_OF_LIGHTHOUSES;i++){
        const uint8_t* potential_address = uwb_addresses_from_LGH[i];
        if (Get_Short_Address_From_Long(potential_address) == short_address){
            return i;
        }
    }
    return -1;
}

int8_t Get_LGH_From_Address(const uint8_t* address)
{
    for (uint8_t i=0;i<NUMBER_OF_LIGHTHOUSES;i++){
        const uint8_t* potential_address = uwb_addresses_from_LGH[i];
        bool same_address = true;
        for (uint8_t j=0;j<UWB_ADDRESS_LENGTH;j++){
            if (potential_address[j] != address[j]){
                same_address = false;
                break;
            }
        }
        if (same_address){
            return i;
        }
    }
    return -1;
}

bool Are_Addresses_Equal(uint8_t* first, uint8_t* second)
{
    for (uint8_t i =0; i<UWB_ADDRESS_LENGTH; i++){
        if (first[i] != second[i]){
            return false;
        }
    }
    return true;
}

void _format_address_to_string(uint8_t lgh_index, char address_str[24])
{
    snprintf(
        address_str,
        24,
        "%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X",
        uwb_addresses_from_LGH[lgh_index][0],
        uwb_addresses_from_LGH[lgh_index][1],
        uwb_addresses_from_LGH[lgh_index][2],
        uwb_addresses_from_LGH[lgh_index][3],
        uwb_addresses_from_LGH[lgh_index][4],
        uwb_addresses_from_LGH[lgh_index][5],
        uwb_addresses_from_LGH[lgh_index][6],
        uwb_addresses_from_LGH[lgh_index][7]
    );
}

void _format_drone_address_to_string(char address_str[24])
{
    snprintf(
        address_str,
        24,
        "%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X",
        drone_address[0],
        drone_address[1],
        drone_address[2],
        drone_address[3],
        drone_address[4],
        drone_address[5],
        drone_address[6],
        drone_address[7]
    );
}
#pragma endregion