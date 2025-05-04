################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/rpi-display/rpiDisplaySceneBuilder.cpp \
../Core/Src/rpi-display/rpiSceneBuilderUser.cpp 

C_SRCS += \
../Core/Src/rpi-display/dmatransmitter.c \
../Core/Src/rpi-display/rpiDisplayShapes.c 

C_DEPS += \
./Core/Src/rpi-display/dmatransmitter.d \
./Core/Src/rpi-display/rpiDisplayShapes.d 

OBJS += \
./Core/Src/rpi-display/dmatransmitter.o \
./Core/Src/rpi-display/rpiDisplaySceneBuilder.o \
./Core/Src/rpi-display/rpiDisplayShapes.o \
./Core/Src/rpi-display/rpiSceneBuilderUser.o 

CPP_DEPS += \
./Core/Src/rpi-display/rpiDisplaySceneBuilder.d \
./Core/Src/rpi-display/rpiSceneBuilderUser.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/rpi-display/%.o Core/Src/rpi-display/%.su Core/Src/rpi-display/%.cyclo: ../Core/Src/rpi-display/%.c Core/Src/rpi-display/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/rpi-display/%.o Core/Src/rpi-display/%.su Core/Src/rpi-display/%.cyclo: ../Core/Src/rpi-display/%.cpp Core/Src/rpi-display/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-rpi-2d-display

clean-Core-2f-Src-2f-rpi-2d-display:
	-$(RM) ./Core/Src/rpi-display/dmatransmitter.cyclo ./Core/Src/rpi-display/dmatransmitter.d ./Core/Src/rpi-display/dmatransmitter.o ./Core/Src/rpi-display/dmatransmitter.su ./Core/Src/rpi-display/rpiDisplaySceneBuilder.cyclo ./Core/Src/rpi-display/rpiDisplaySceneBuilder.d ./Core/Src/rpi-display/rpiDisplaySceneBuilder.o ./Core/Src/rpi-display/rpiDisplaySceneBuilder.su ./Core/Src/rpi-display/rpiDisplayShapes.cyclo ./Core/Src/rpi-display/rpiDisplayShapes.d ./Core/Src/rpi-display/rpiDisplayShapes.o ./Core/Src/rpi-display/rpiDisplayShapes.su ./Core/Src/rpi-display/rpiSceneBuilderUser.cyclo ./Core/Src/rpi-display/rpiSceneBuilderUser.d ./Core/Src/rpi-display/rpiSceneBuilderUser.o ./Core/Src/rpi-display/rpiSceneBuilderUser.su

.PHONY: clean-Core-2f-Src-2f-rpi-2d-display

