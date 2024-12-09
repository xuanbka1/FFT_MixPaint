################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Utilities/ringbuffer.c 

OBJS += \
./Core/Utilities/ringbuffer.o 

C_DEPS += \
./Core/Utilities/ringbuffer.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Utilities/%.o Core/Utilities/%.su Core/Utilities/%.cyclo: ../Core/Utilities/%.c Core/Utilities/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I"F:/TRONSON_DRV8840/TRONSON_DRV8840/MIX_PAIN_FINAL/Core/Utilities" -I"F:/TRONSON_DRV8840/TRONSON_DRV8840/MIX_PAIN_FINAL/Core/user_lib" -I"F:/TRONSON_DRV8840/TRONSON_DRV8840/MIX_PAIN_FINAL/Core/user_lib" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Utilities

clean-Core-2f-Utilities:
	-$(RM) ./Core/Utilities/ringbuffer.cyclo ./Core/Utilities/ringbuffer.d ./Core/Utilities/ringbuffer.o ./Core/Utilities/ringbuffer.su

.PHONY: clean-Core-2f-Utilities

