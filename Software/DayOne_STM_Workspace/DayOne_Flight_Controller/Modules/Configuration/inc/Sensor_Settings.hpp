#ifndef CONFIGURATION_INC_SENSOR_SETTINGS_HPP_
#define CONFIGURATION_INC_SENSOR_SETTINGS_HPP_

#pragma region IMU Settings

enum class IMU_UPDATE_RATES_ENUM : uint8_t
{
	Hz_833 = 0b0111'0000, Hz_1660 = 0b1000'0000,
};

enum class IMU_ACC_SCALES_ENUM : uint8_t
{
	G_8 = 0b0000'1100, G_16 = 0b0000'0100,
};

enum class IMU_GYRO_SCALES_ENUM : uint8_t
{
	DPS_500 = 0b0000'0100, DPS_1000 = 0b0000'1000,
};

#pragma endregion

#pragma region Magnetometer Settings
enum class MAGNETOMETER_UPDATE_RATES_ENUM
{
	Hz_100, Hz_225, Hz_580,
};

#pragma endregion


#endif /* CONFIGURATION_INC_SENSOR_CONFIG_HPP_ */
