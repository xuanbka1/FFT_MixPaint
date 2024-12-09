################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MixColor/Driver/driver_buzzer.c \
../MixColor/Driver/driver_memory.c \
../MixColor/Driver/driver_timer.c 

OBJS += \
./MixColor/Driver/driver_buzzer.o \
./MixColor/Driver/driver_memory.o \
./MixColor/Driver/driver_timer.o 

C_DEPS += \
./MixColor/Driver/driver_buzzer.d \
./MixColor/Driver/driver_memory.d \
./MixColor/Driver/driver_timer.d 


# Each subdirectory must supply rules for building sources it contributes
MixColor/Driver/%.o MixColor/Driver/%.su MixColor/Driver/%.cyclo: ../MixColor/Driver/%.c MixColor/Driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I"E:/RTOS_ARM/STM32F103/STM32F103VET6/TRONSON_DRV8840/MixColor/Hardware" -I"E:/RTOS_ARM/STM32F103/STM32F103VET6/TRONSON_DRV8840/MixColor/Driver" -I"E:/RTOS_ARM/STM32F103/STM32F103VET6/TRONSON_DRV8840/Core/user_lib" -I"E:/RTOS_ARM/STM32F103/STM32F103VET6/TRONSON_DRV8840/Core/user_lib" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-MixColor-2f-Driver

clean-MixColor-2f-Driver:
	-$(RM) ./MixColor/Driver/driver_buzzer.cyclo ./MixColor/Driver/driver_buzzer.d ./MixColor/Driver/driver_buzzer.o ./MixColor/Driver/driver_buzzer.su ./MixColor/Driver/driver_memory.cyclo ./MixColor/Driver/driver_memory.d ./MixColor/Driver/driver_memory.o ./MixColor/Driver/driver_memory.su ./MixColor/Driver/driver_timer.cyclo ./MixColor/Driver/driver_timer.d ./MixColor/Driver/driver_timer.o ./MixColor/Driver/driver_timer.su

.PHONY: clean-MixColor-2f-Driver

