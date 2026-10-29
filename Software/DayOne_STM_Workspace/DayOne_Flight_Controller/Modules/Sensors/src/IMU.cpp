#include "Sensors/inc/IMU.hpp"
#include "stm32f4xx_hal.h"

#pragma region Variables

enum class IMU_REGISTERS : uint8_t
{
    IMU_WHOAMI_REG = 0x0F,          // Product ID
    IMU_CTRL1_ACC_REG = 0x10,       // Accelerometer settings
    IMU_CTRL2_GYRO_REG = 0x11,      // Gyroscope settings
    IMU_CTRL3_REG = 0x12,      // Reset and other settings

    IMU_TEMP_REG = 0x20,            // Temperature read register
    IMU_MEASUREMENT_REG = 0x22,     // Measurement read register
};

constexpr uint8_t TARGET_WHOAMI_VALUE = 0x6C;

enum class IMU_MEASUREMENT_DATA_LAYOUT : uint8_t
{
    OUTX_L_G = 1,
    OUTX_H_G,
    OUTY_L_G,
    OUTY_H_G,
    OUTZ_L_G,
    OUTZ_H_G,

    OUTX_L_A,
    OUTX_H_A,
    OUTY_L_A,
    OUTY_H_A,
    OUTZ_L_A,
    OUTZ_H_A,
};

#pragma endregion

SENSOR_STATUS IMU::Initialize(
    IMU_UPDATE_RATES p_update_rate,
    IMU_ACC_SCALE p_acc_scale,
    IMU_GYRO_SCALE p_gyro_scale)
{
    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);

    update_rate = p_update_rate;
    acc_scale = p_acc_scale;
    gyro_scale = p_gyro_scale;

    bool id_ok = Assert_ID_Register();
    HAL_StatusTypeDef acc_code = Handle_Acc_Control_Registers();
    HAL_StatusTypeDef gyro_code = Handle_Gyro_Control_Registers();

    Update_Sensitivities();

    if (acc_code == HAL_OK &&
        gyro_code == HAL_OK &&
        id_ok)
    {
        return SENSOR_STATUS::INIT_OK;
    }

    return SENSOR_STATUS::INIT_FAIL;
}

#pragma region Callbacks

void IMU::DMA_TxRxCompleteCallback()
{
    Parse_Measurement(current_receive_buffer, IMU_BUFFER_SIZE);
    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
}

void IMU::DMA_ErrorCallback()
{
    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
}

void IMU::DMA_TxCompleteCallback()
{
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
}

#pragma endregion

#pragma region Setup

HAL_StatusTypeDef IMU::Handle_Acc_Control_Registers()
{
    current_transmit_buffer[0] =
        Get_Write_Command(
            static_cast<uint8_t>(IMU_REGISTERS::IMU_CTRL1_ACC_REG));

    current_transmit_buffer[1] =
        static_cast<uint8_t>(update_rate) |
        static_cast<uint8_t>(acc_scale);

    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);

    HAL_StatusTypeDef code =
        HAL_SPI_Transmit(
            spi,
            current_transmit_buffer,
            2,
            HAL_MAX_DELAY);

    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);

    Change_Buffer_Pointers();

    return code;
}

HAL_StatusTypeDef IMU::Handle_Gyro_Control_Registers()
{
    current_transmit_buffer[0] =
        Get_Write_Command(
            static_cast<uint8_t>(IMU_REGISTERS::IMU_CTRL2_GYRO_REG));

    current_transmit_buffer[1] =
        static_cast<uint8_t>(update_rate) |
        static_cast<uint8_t>(gyro_scale);

    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);

    HAL_StatusTypeDef code =
        HAL_SPI_Transmit(
            spi,
            current_transmit_buffer,
            2,
            HAL_MAX_DELAY);

    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);

    Change_Buffer_Pointers();

    return code;
}

bool IMU::Assert_ID_Register()
{
    current_transmit_buffer[0] =
        Get_Read_Command(
            static_cast<uint8_t>(IMU_REGISTERS::IMU_WHOAMI_REG));

    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);

    HAL_StatusTypeDef code =
        HAL_SPI_TransmitReceive(
            spi,
            current_transmit_buffer,
            current_receive_buffer,
            2,
            HAL_MAX_DELAY);

    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);

    bool ok = current_receive_buffer[1] == TARGET_WHOAMI_VALUE;

    Change_Buffer_Pointers();

    return (code == HAL_OK) && ok;
}

void IMU::Update_Sensitivities()
{
    switch (acc_scale)
    {
        case IMU_ACC_SCALE::G_8:
            acc_sensitivity_g = 0.244f * 0.001f;
            break;

        case IMU_ACC_SCALE::G_16:
            acc_sensitivity_g = 0.488f * 0.001f;
            break;

        default:
            break;
    }

    switch (gyro_scale)
    {
        case IMU_GYRO_SCALE::DPS_500:
            gyro_sensitivity_dps = 17.50f * 0.001f;
            break;

        case IMU_GYRO_SCALE::DPS_1000:
            gyro_sensitivity_dps = 35.0f * 0.001f;
            break;

        default:
            break;
    }
}

#pragma endregion

#pragma region Master Calls

void IMU::Prepare_For_Measurements()
{
    for (uint8_t i = 0; i < IMU_BUFFER_SIZE; ++i)
    {
        transmit_buffer_1[i] = 0;
        transmit_buffer_2[i] = 0;
    }

    transmit_buffer_1[0] =
        Get_Read_Command(
            static_cast<uint8_t>(IMU_REGISTERS::IMU_MEASUREMENT_REG));

    transmit_buffer_2[0] =
        Get_Read_Command(
            static_cast<uint8_t>(IMU_REGISTERS::IMU_MEASUREMENT_REG));
}

void IMU::Measurement_Cycle_Finished()
{
    Change_Buffer_Pointers();
}

void IMU::Task_Perform_Transaction()
{
    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);

    HAL_StatusTypeDef code =
        HAL_SPI_TransmitReceive_DMA(
            spi,
            current_transmit_buffer,
            current_receive_buffer,
            IMU_BUFFER_SIZE);

    if (code != HAL_OK)
    {
        HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
    }
}

HAL_StatusTypeDef IMU::Reset_Chip()
{
    current_transmit_buffer[0] =
        Get_Write_Command(
            static_cast<uint8_t>(IMU_REGISTERS::IMU_CTRL3_REG));

    current_transmit_buffer[1] = 0b0000'0101;

    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);

    HAL_StatusTypeDef code =
        HAL_SPI_Transmit(
            spi,
            current_transmit_buffer,
            2,
            HAL_MAX_DELAY);

    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);

    Change_Buffer_Pointers();

    return code;
}

bool IMU::Get_Measurement(IMU_Measurement &meas)
{
    if (data_ready)
    {
        meas = current_measurement;
        data_ready = false;
        return true;
    }

    return false;
}

#pragma endregion

#pragma region Data Parsing

bool IMU::Parse_Measurement(uint8_t* received_data, uint8_t size)
{
    if (size != IMU_BUFFER_SIZE)
    {
        return false;
    }

    const int16_t raw_A_x = static_cast<int16_t>(
        (static_cast<uint16_t>(
            received_data[static_cast<uint8_t>(
                IMU_MEASUREMENT_DATA_LAYOUT::OUTX_H_A)]) << 8) |
        received_data[static_cast<uint8_t>(
            IMU_MEASUREMENT_DATA_LAYOUT::OUTX_L_A)]);

    const int16_t raw_A_y = static_cast<int16_t>(
        (static_cast<uint16_t>(
            received_data[static_cast<uint8_t>(
                IMU_MEASUREMENT_DATA_LAYOUT::OUTY_H_A)]) << 8) |
        received_data[static_cast<uint8_t>(
            IMU_MEASUREMENT_DATA_LAYOUT::OUTY_L_A)]);

    const int16_t raw_A_z = static_cast<int16_t>(
        (static_cast<uint16_t>(
            received_data[static_cast<uint8_t>(
                IMU_MEASUREMENT_DATA_LAYOUT::OUTZ_H_A)]) << 8) |
        received_data[static_cast<uint8_t>(
            IMU_MEASUREMENT_DATA_LAYOUT::OUTZ_L_A)]);

    const int16_t raw_G_x = static_cast<int16_t>(
        (static_cast<uint16_t>(
            received_data[static_cast<uint8_t>(
                IMU_MEASUREMENT_DATA_LAYOUT::OUTX_H_G)]) << 8) |
        received_data[static_cast<uint8_t>(
            IMU_MEASUREMENT_DATA_LAYOUT::OUTX_L_G)]);

    const int16_t raw_G_y = static_cast<int16_t>(
        (static_cast<uint16_t>(
            received_data[static_cast<uint8_t>(
                IMU_MEASUREMENT_DATA_LAYOUT::OUTY_H_G)]) << 8) |
        received_data[static_cast<uint8_t>(
            IMU_MEASUREMENT_DATA_LAYOUT::OUTY_L_G)]);

    const int16_t raw_G_z = static_cast<int16_t>(
        (static_cast<uint16_t>(
            received_data[static_cast<uint8_t>(
                IMU_MEASUREMENT_DATA_LAYOUT::OUTZ_H_G)]) << 8) |
        received_data[static_cast<uint8_t>(
            IMU_MEASUREMENT_DATA_LAYOUT::OUTZ_L_G)]);

    current_measurement.acc_x =
        static_cast<float>(raw_A_x) * acc_sensitivity_g;

    current_measurement.acc_y =
        static_cast<float>(raw_A_y) * acc_sensitivity_g;

    current_measurement.acc_z =
        static_cast<float>(raw_A_z) * acc_sensitivity_g;

    current_measurement.gyro_x =
        static_cast<float>(raw_G_x) * gyro_sensitivity_dps;

    current_measurement.gyro_y =
        static_cast<float>(raw_G_y) * gyro_sensitivity_dps;

    current_measurement.gyro_z =
        static_cast<float>(raw_G_z) * gyro_sensitivity_dps;

    data_ready = true;

    return true;
}

#pragma endregion
