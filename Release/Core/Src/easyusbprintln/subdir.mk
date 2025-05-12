################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/easyusbprintln/easyusbprintln.c 

C_DEPS += \
./Core/Src/easyusbprintln/easyusbprintln.d 

OBJS += \
./Core/Src/easyusbprintln/easyusbprintln.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/easyusbprintln/%.o Core/Src/easyusbprintln/%.su Core/Src/easyusbprintln/%.cyclo: ../Core/Src/easyusbprintln/%.c Core/Src/easyusbprintln/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-easyusbprintln

clean-Core-2f-Src-2f-easyusbprintln:
	-$(RM) ./Core/Src/easyusbprintln/easyusbprintln.cyclo ./Core/Src/easyusbprintln/easyusbprintln.d ./Core/Src/easyusbprintln/easyusbprintln.o ./Core/Src/easyusbprintln/easyusbprintln.su

.PHONY: clean-Core-2f-Src-2f-easyusbprintln

