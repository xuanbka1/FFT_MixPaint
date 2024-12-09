################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/user_lib/appPcHandle.c \
../Core/user_lib/board_peripheral.c \
../Core/user_lib/buzzer.c \
../Core/user_lib/driver_rtc.c \
../Core/user_lib/eeprom_24c32.c \
../Core/user_lib/myEepromUtility.c \
../Core/user_lib/my_debug.c \
../Core/user_lib/notifyHandler.c \
../Core/user_lib/test.c 

OBJS += \
./Core/user_lib/appPcHandle.o \
./Core/user_lib/board_peripheral.o \
./Core/user_lib/buzzer.o \
./Core/user_lib/driver_rtc.o \
./Core/user_lib/eeprom_24c32.o \
./Core/user_lib/myEepromUtility.o \
./Core/user_lib/my_debug.o \
./Core/user_lib/notifyHandler.o \
./Core/user_lib/test.o 

C_DEPS += \
./Core/user_lib/appPcHandle.d \
./Core/user_lib/board_peripheral.d \
./Core/user_lib/buzzer.d \
./Core/user_lib/driver_rtc.d \
./Core/user_lib/eeprom_24c32.d \
./Core/user_lib/myEepromUtility.d \
./Core/user_lib/my_debug.d \
./Core/user_lib/notifyHandler.d \
./Core/user_lib/test.d 


# Each subdirectory must supply rules for building sources it contributes
Core/user_lib/%.o Core/user_lib/%.su Core/user_lib/%.cyclo: ../Core/user_lib/%.c Core/user_lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I"F:/TRONSON_DRV8840/TRONSON_DRV8840/MIX_PAIN_FINAL/Core/Utilities" -I"F:/TRONSON_DRV8840/TRONSON_DRV8840/MIX_PAIN_FINAL/Core/user_lib" -I"F:/TRONSON_DRV8840/TRONSON_DRV8840/MIX_PAIN_FINAL/Core/user_lib" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-user_lib

clean-Core-2f-user_lib:
	-$(RM) ./Core/user_lib/appPcHandle.cyclo ./Core/user_lib/appPcHandle.d ./Core/user_lib/appPcHandle.o ./Core/user_lib/appPcHandle.su ./Core/user_lib/board_peripheral.cyclo ./Core/user_lib/board_peripheral.d ./Core/user_lib/board_peripheral.o ./Core/user_lib/board_peripheral.su ./Core/user_lib/buzzer.cyclo ./Core/user_lib/buzzer.d ./Core/user_lib/buzzer.o ./Core/user_lib/buzzer.su ./Core/user_lib/driver_rtc.cyclo ./Core/user_lib/driver_rtc.d ./Core/user_lib/driver_rtc.o ./Core/user_lib/driver_rtc.su ./Core/user_lib/eeprom_24c32.cyclo ./Core/user_lib/eeprom_24c32.d ./Core/user_lib/eeprom_24c32.o ./Core/user_lib/eeprom_24c32.su ./Core/user_lib/myEepromUtility.cyclo ./Core/user_lib/myEepromUtility.d ./Core/user_lib/myEepromUtility.o ./Core/user_lib/myEepromUtility.su ./Core/user_lib/my_debug.cyclo ./Core/user_lib/my_debug.d ./Core/user_lib/my_debug.o ./Core/user_lib/my_debug.su ./Core/user_lib/notifyHandler.cyclo ./Core/user_lib/notifyHandler.d ./Core/user_lib/notifyHandler.o ./Core/user_lib/notifyHandler.su ./Core/user_lib/test.cyclo ./Core/user_lib/test.d ./Core/user_lib/test.o ./Core/user_lib/test.su

.PHONY: clean-Core-2f-user_lib

