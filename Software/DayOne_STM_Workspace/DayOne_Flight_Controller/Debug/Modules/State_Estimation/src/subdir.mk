################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Modules/State_Estimation/src/Kalman_Filter.cpp \
../Modules/State_Estimation/src/State_Estimator.cpp 

OBJS += \
./Modules/State_Estimation/src/Kalman_Filter.o \
./Modules/State_Estimation/src/State_Estimator.o 

CPP_DEPS += \
./Modules/State_Estimation/src/Kalman_Filter.d \
./Modules/State_Estimation/src/State_Estimator.d 


# Each subdirectory must supply rules for building sources it contributes
Modules/State_Estimation/src/%.o Modules/State_Estimation/src/%.su Modules/State_Estimation/src/%.cyclo: ../Modules/State_Estimation/src/%.cpp Modules/State_Estimation/src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F413xx -c -I../Core/Inc -I"C:/Users/arekr/OneDrive/Pulpit/Github/Quadcopter_Full_Implementation/Software/DayOne_STM_Workspace/DayOne_Flight_Controller/Modules" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Modules-2f-State_Estimation-2f-src

clean-Modules-2f-State_Estimation-2f-src:
	-$(RM) ./Modules/State_Estimation/src/Kalman_Filter.cyclo ./Modules/State_Estimation/src/Kalman_Filter.d ./Modules/State_Estimation/src/Kalman_Filter.o ./Modules/State_Estimation/src/Kalman_Filter.su ./Modules/State_Estimation/src/State_Estimator.cyclo ./Modules/State_Estimation/src/State_Estimator.d ./Modules/State_Estimation/src/State_Estimator.o ./Modules/State_Estimation/src/State_Estimator.su

.PHONY: clean-Modules-2f-State_Estimation-2f-src

