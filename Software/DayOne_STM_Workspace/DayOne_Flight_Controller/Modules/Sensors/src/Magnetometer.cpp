#include "Sensors/inc/Magnetometer.hpp"
#include "stm32f4xx_hal.h"

constexpr float MAG_SENSITIVITY = 4096.0;

enum class MAGNETOMETER_REGISTERS : uint8_t
{
    MEASUREMENT_REG = 0x00,
    STATUS = 0x08,

    CONTROL_0 = 0x09,
    CONTROL_1 = 0x0A,

    PRODUCT_ID = 0x2F,
};

enum class MAGNETOMETER_STATUS_COMMANDS : uint8_t
{
    OTP_RD_DONE = 0b0001'0000,
    MEAS_TEMP_DONE = 0b0000'0010,
    MEAS_MAGNETOMETER_DONE = 0b0000'0001,
};

enum class MAGNETOMETER_CONTROL_0_COMMANDS : uint8_t
{
    INIT_TEMP_MEASUREMENT = 0b0000'0010,
    INIT_MAGNETOMETER_MEASUREMENT = 0b0000'0001,
    INTERRUPT_ENABLE = 0b0000'0100,
    SET = 0b0000'1000,
    RESET = 0b0001'0000,
};

enum class MAGNETOMETER_CONTROL_1_COMMANDS : uint8_t
{
    BW_100HZ = 0b0000'0001,
    BW_225HZ = 0b0000'0010,
    BW_580HZ = 0b0000'0011,
    CHIP_RESET = 0b1000'0000,
};

constexpr uint8_t TARGET_PRODUCT_ID = 0x30;

SENSOR_STATUS Magnetometer::Initialize(MAGNETOMETER_UPDATE_RATES p_update_rate)
{
    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);

    update_rate = p_update_rate;

    bool id_ok = Assert_ID_Register();
    HAL_StatusTypeDef update_rate_code = Handle_Update_Rate_Registers();
    HAL_StatusTypeDef interrupt_code = Enable_Interrupt();

    if (id_ok == true &&
        update_rate_code == HAL_OK &&
        interrupt_code == HAL_OK)
    {
        current_state = MAGNETOMETER_INTERNAL_STATES::SET;
        return SENSOR_STATUS::INIT_OK;
    }

    return SENSOR_STATUS::INIT_FAIL;
}

#pragma region Callbacks

void Magnetometer::DMA_TxRxCompleteCallback()
{
    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);

    switch (current_state) {
		case MAGNETOMETER_INTERNAL_STATES::READ_MEASUREMENT_1:
			Store_Measurement(
				current_receive_buffer,
				MAGNETOMETER_BUFFER_SIZE,
				set_raw_measurement);

			current_state = MAGNETOMETER_INTERNAL_STATES::CLEAR_INTERRUPT_1;
		    Change_Buffer_Pointers();
			break;
		case MAGNETOMETER_INTERNAL_STATES::READ_MEASUREMENT_2:
	        Store_Measurement(
	            current_receive_buffer,
	            MAGNETOMETER_BUFFER_SIZE,
	            reset_raw_measurement);

	        Parse_Data();
	        current_state = MAGNETOMETER_INTERNAL_STATES::CLEAR_INTERRUPT_2;
	        Change_Buffer_Pointers();
	        break;
		default:
			break;
	}
}

void Magnetometer::DMA_TxCompleteCallback()
{
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
}

void Magnetometer::DMA_ErrorCallback()
{
    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
}

void Magnetometer::Data_Ready_EXTI_Handler()
{
    chip_output_ready = true;
}

#pragma endregion

#pragma region Master_Calls

void Magnetometer::Measurement_Cycle_Finished()
{
}

void Magnetometer::Prepare_For_Measurements()
{
}

void Magnetometer::Task_Perform_Transaction()
{
    switch (current_state)
    {
    	case MAGNETOMETER_INTERNAL_STATES::RECOVER:
			Recover();
			break;

    	case MAGNETOMETER_INTERNAL_STATES::SET:
            Perform_Set();
            break;

        case MAGNETOMETER_INTERNAL_STATES::ENABLE_MEASUREMENT_1:
            Enable_Measurement();
            break;

        case MAGNETOMETER_INTERNAL_STATES::READ_MEASUREMENT_1:
            Read_Measurement();
        	break;

        case MAGNETOMETER_INTERNAL_STATES::CLEAR_INTERRUPT_1:
            Clear_Interrupt();
            break;

        case MAGNETOMETER_INTERNAL_STATES::RESET:
            Perform_Reset();
            break;

        case MAGNETOMETER_INTERNAL_STATES::ENABLE_MEASUREMENT_2:
            Enable_Measurement();
            break;

        case MAGNETOMETER_INTERNAL_STATES::READ_MEASUREMENT_2:
            Read_Measurement();
            break;

        case MAGNETOMETER_INTERNAL_STATES::CLEAR_INTERRUPT_2:
            Clear_Interrupt();
            break;

        default:
            break;
    }
}

#pragma region Data_Transmission

HAL_StatusTypeDef Magnetometer::Reset_Chip()
{
	current_transmit_buffer[0] =
		Get_Write_Command(
			static_cast<uint8_t>(MAGNETOMETER_REGISTERS::CONTROL_1));

	current_transmit_buffer[1] = static_cast<uint8_t>(MAGNETOMETER_CONTROL_1_COMMANDS::CHIP_RESET);

	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);
	HAL_StatusTypeDef code =
		HAL_SPI_Transmit_DMA(spi, current_transmit_buffer, 2);

	Change_Buffer_Pointers();

	return code;
}

HAL_StatusTypeDef Magnetometer::Handle_Update_Rate_Registers()
{
    current_transmit_buffer[0] =
        Get_Write_Command(
            static_cast<uint8_t>(MAGNETOMETER_REGISTERS::CONTROL_1));

    current_transmit_buffer[1] = Get_Update_Rate_Command();

    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);
    HAL_StatusTypeDef code =
        HAL_SPI_Transmit(spi, current_transmit_buffer, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);

    Change_Buffer_Pointers();

    return code;
}

HAL_StatusTypeDef Magnetometer::Enable_Interrupt()
{
    current_transmit_buffer[0] =
        Get_Write_Command(
            static_cast<uint8_t>(MAGNETOMETER_REGISTERS::CONTROL_0));

    current_transmit_buffer[1] =
        static_cast<uint8_t>(
            MAGNETOMETER_CONTROL_0_COMMANDS::INTERRUPT_ENABLE);

    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);
    HAL_StatusTypeDef code =
        HAL_SPI_Transmit(spi, current_transmit_buffer, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);

    Change_Buffer_Pointers();

    return code;
}

HAL_StatusTypeDef Magnetometer::Recover()
{
	current_transmit_buffer[0] =
		Get_Write_Command(
			static_cast<uint8_t>(MAGNETOMETER_REGISTERS::CONTROL_0));

	current_transmit_buffer[1] =
		static_cast<uint8_t>(
			MAGNETOMETER_CONTROL_0_COMMANDS::INTERRUPT_ENABLE);

	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);
	HAL_StatusTypeDef code =
		HAL_SPI_Transmit_DMA(spi, current_transmit_buffer, 2);

	if (code != HAL_OK)
	{
		return code;
	}

	Change_Buffer_Pointers();
	current_state = MAGNETOMETER_INTERNAL_STATES::CLEAR_INTERRUPT_2;
	return code;
}

HAL_StatusTypeDef Magnetometer::Perform_Set()
{
    if (current_state != MAGNETOMETER_INTERNAL_STATES::SET)
    {
        return HAL_ERROR;
    }

    current_transmit_buffer[0] =
        Get_Write_Command(
            static_cast<uint8_t>(MAGNETOMETER_REGISTERS::CONTROL_0));

    current_transmit_buffer[1] =
        static_cast<uint8_t>(
            MAGNETOMETER_CONTROL_0_COMMANDS::INTERRUPT_ENABLE) |
        static_cast<uint8_t>(
            MAGNETOMETER_CONTROL_0_COMMANDS::SET);

    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);
    HAL_StatusTypeDef code =
        HAL_SPI_Transmit_DMA(spi, current_transmit_buffer, 2);

    if (code != HAL_OK)
    {
    	return code;
    }

    Change_Buffer_Pointers();

    current_state = MAGNETOMETER_INTERNAL_STATES::ENABLE_MEASUREMENT_1;

    return code;
}

HAL_StatusTypeDef Magnetometer::Perform_Reset()
{
    if (current_state != MAGNETOMETER_INTERNAL_STATES::RESET)
    {
        return HAL_ERROR;
    }

    current_transmit_buffer[0] =
        Get_Write_Command(
            static_cast<uint8_t>(MAGNETOMETER_REGISTERS::CONTROL_0));

    current_transmit_buffer[1] =
        static_cast<uint8_t>(
            MAGNETOMETER_CONTROL_0_COMMANDS::INTERRUPT_ENABLE) |
        static_cast<uint8_t>(
            MAGNETOMETER_CONTROL_0_COMMANDS::RESET);

    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);
    HAL_StatusTypeDef code =
        HAL_SPI_Transmit_DMA(spi, current_transmit_buffer, 2);

    if (code != HAL_OK)
	{
		return code;
	}

    Change_Buffer_Pointers();

    current_state = MAGNETOMETER_INTERNAL_STATES::ENABLE_MEASUREMENT_2;

    return code;
}

HAL_StatusTypeDef Magnetometer::Enable_Measurement()
{
    current_transmit_buffer[0] =
        Get_Write_Command(
            static_cast<uint8_t>(MAGNETOMETER_REGISTERS::CONTROL_0));

    current_transmit_buffer[1] =
        static_cast<uint8_t>(
            MAGNETOMETER_CONTROL_0_COMMANDS::INTERRUPT_ENABLE) |
        static_cast<uint8_t>(
            MAGNETOMETER_CONTROL_0_COMMANDS::INIT_MAGNETOMETER_MEASUREMENT);

    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);
    HAL_StatusTypeDef code =
        HAL_SPI_Transmit_DMA(spi, current_transmit_buffer, 2);

    if (code != HAL_OK)
	{
		return code;
	}

    Change_Buffer_Pointers();

	if (current_state == MAGNETOMETER_INTERNAL_STATES::ENABLE_MEASUREMENT_1)
	{
		current_state = MAGNETOMETER_INTERNAL_STATES::READ_MEASUREMENT_1;
	}
	else if (current_state == MAGNETOMETER_INTERNAL_STATES::ENABLE_MEASUREMENT_2)
	{
		current_state = MAGNETOMETER_INTERNAL_STATES::READ_MEASUREMENT_2;
	}
    return code;
}

HAL_StatusTypeDef Magnetometer::Read_Measurement()
{
    if (!chip_output_ready)
    {
    	read_chip_safety_counter++;
    	bool sra = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);
        if (read_chip_safety_counter >= 10)
        {
        	current_state = MAGNETOMETER_INTERNAL_STATES::RECOVER;
        	read_chip_safety_counter = 0;
        }
    	return HAL_ERROR;
    }
    read_chip_safety_counter = 0;

    chip_output_ready = false;

    current_transmit_buffer[0] =
        Get_Read_Command(
            static_cast<uint8_t>(MAGNETOMETER_REGISTERS::MEASUREMENT_REG));

    current_transmit_buffer[1] = 0x00;

    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);

    HAL_StatusTypeDef code =
        HAL_SPI_TransmitReceive_DMA(
            spi,
            current_transmit_buffer,
            current_receive_buffer,
            MAGNETOMETER_BUFFER_SIZE);

    if (code != HAL_OK)
	{
		chip_output_ready = true;
	}

    return code;
}

HAL_StatusTypeDef Magnetometer::Clear_Interrupt()
{
    current_transmit_buffer[0] =
        Get_Write_Command(
            static_cast<uint8_t>(MAGNETOMETER_REGISTERS::STATUS));

    current_transmit_buffer[1] =
        static_cast<uint8_t>(
            MAGNETOMETER_STATUS_COMMANDS::MEAS_MAGNETOMETER_DONE) |
        static_cast<uint8_t>(
            MAGNETOMETER_STATUS_COMMANDS::MEAS_TEMP_DONE);

    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);

    HAL_StatusTypeDef code =
        HAL_SPI_Transmit_DMA(spi, current_transmit_buffer, 2);

    if (code != HAL_OK)
	{
		return code;
	}

    Change_Buffer_Pointers();

    if (current_state == MAGNETOMETER_INTERNAL_STATES::CLEAR_INTERRUPT_1)
    {
    	current_state = MAGNETOMETER_INTERNAL_STATES::RESET;
    }
    else if (current_state == MAGNETOMETER_INTERNAL_STATES::CLEAR_INTERRUPT_2)
    {
    	current_state = MAGNETOMETER_INTERNAL_STATES::SET;
    }
    return code;
}

bool Magnetometer::Assert_ID_Register()
{
    current_transmit_buffer[0] =
        Get_Read_Command(
            static_cast<uint8_t>(MAGNETOMETER_REGISTERS::PRODUCT_ID));

    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);

    HAL_StatusTypeDef code =
        HAL_SPI_TransmitReceive(
            spi,
            current_transmit_buffer,
            current_receive_buffer,
            2,
            HAL_MAX_DELAY);

    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);

    bool ok = current_receive_buffer[1] == TARGET_PRODUCT_ID;

    Change_Buffer_Pointers();

    return (code == HAL_OK) && (ok == true);
}

#pragma endregion

uint8_t Magnetometer::Get_Update_Rate_Command()
{
    uint8_t command =
        static_cast<uint8_t>(MAGNETOMETER_CONTROL_1_COMMANDS::BW_100HZ);

    switch (update_rate)
    {
        case MAGNETOMETER_UPDATE_RATES::Hz_100:
            command =
                static_cast<uint8_t>(
                    MAGNETOMETER_CONTROL_1_COMMANDS::BW_100HZ);
            break;

        case MAGNETOMETER_UPDATE_RATES::Hz_225:
            command =
                static_cast<uint8_t>(
                    MAGNETOMETER_CONTROL_1_COMMANDS::BW_225HZ);
            break;

        case MAGNETOMETER_UPDATE_RATES::Hz_580:
            command =
                static_cast<uint8_t>(
                    MAGNETOMETER_CONTROL_1_COMMANDS::BW_580HZ);
            break;
    }

    return command;
}

bool Magnetometer::Store_Measurement(
    uint8_t *received_data,
    uint8_t size,
    Raw_Magnetic_Value& meas)
{
    if (size != MAGNETOMETER_BUFFER_SIZE)
    {
        return false;
    }

    meas.x = (static_cast<uint32_t>(received_data[1]) << 8) |
             static_cast<uint32_t>(received_data[2]);

    meas.y = (static_cast<uint32_t>(received_data[3]) << 8) |
             static_cast<uint32_t>(received_data[4]);

    meas.z = (static_cast<uint32_t>(received_data[5]) << 8) |
             static_cast<uint32_t>(received_data[6]);

    return true;
}

bool Magnetometer::Parse_Data()
{
    current_measurement.mag_x =
        (static_cast<float>(set_raw_measurement.x) -
         static_cast<float>(reset_raw_measurement.x)) /
        (2.0f * MAG_SENSITIVITY);

    current_measurement.mag_y =
        (static_cast<float>(set_raw_measurement.y) -
         static_cast<float>(reset_raw_measurement.y)) /
        (2.0f * MAG_SENSITIVITY);

    current_measurement.mag_z =
        (static_cast<float>(set_raw_measurement.z) -
         static_cast<float>(reset_raw_measurement.z)) /
        (2.0f * MAG_SENSITIVITY);

    data_ready = true;
    return true;
}


bool Magnetometer::Get_Measurement(Magnetometer_Measurement &meas)
{
    if (data_ready)
    {
        meas = current_measurement;
        data_ready = false;
        return true;
    }

    return false;
}
