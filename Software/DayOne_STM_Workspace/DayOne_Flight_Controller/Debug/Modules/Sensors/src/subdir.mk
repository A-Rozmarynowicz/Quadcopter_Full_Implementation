################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Modules/Sensors/src/IMU.cpp \
../Modules/Sensors/src/Magnetometer.cpp 

OBJS += \
./Modules/Sensors/src/IMU.o \
./Modules/Sensors/src/Magnetometer.o 

CPP_DEPS += \
./Modules/Sensors/src/IMU.d \
./Modules/Sensors/src/Magnetometer.d 


# Each subdirectory must supply rules for building sources it contributes
Modules/Sensors/src/%.o Modules/Sensors/src/%.su Modules/Sensors/src/%.cyclo: ../Modules/Sensors/src/%.cpp Modules/Sensors/src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F413xx -c -I../Core/Inc -I"C:/Users/arekr/OneDrive/Pulpit/Github/Quadcopter_Full_Implementation/Software/DayOne_STM_Workspace/DayOne_Flight_Controller/Modules" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Modules-2f-Sensors-2f-src

clean-Modules-2f-Sensors-2f-src:
	-$(RM) ./Modules/Sensors/src/IMU.cyclo ./Modules/Sensors/src/IMU.d ./Modules/Sensors/src/IMU.o ./Modules/Sensors/src/IMU.su ./Modules/Sensors/src/Magnetometer.cyclo ./Modules/Sensors/src/Magnetometer.d ./Modules/Sensors/src/Magnetometer.o ./Modules/Sensors/src/Magnetometer.su

.PHONY: clean-Modules-2f-Sensors-2f-src

