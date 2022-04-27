################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/background_image.c \
../src/game.c \
../src/graphics.c \
../src/lander_image.c \
../src/lcd.c \
../src/main.c \
../src/midi.c \
../src/midi_main.c \
../src/platform_image.c \
../src/syscalls.c \
../src/system_stm32f0xx.c \
../src/wavetable.c 

OBJS += \
./src/background_image.o \
./src/game.o \
./src/graphics.o \
./src/lander_image.o \
./src/lcd.o \
./src/main.o \
./src/midi.o \
./src/midi_main.o \
./src/platform_image.o \
./src/syscalls.o \
./src/system_stm32f0xx.o \
./src/wavetable.o 

C_DEPS += \
./src/background_image.d \
./src/game.d \
./src/graphics.d \
./src/lander_image.d \
./src/lcd.d \
./src/main.d \
./src/midi.d \
./src/midi_main.d \
./src/platform_image.d \
./src/syscalls.d \
./src/system_stm32f0xx.d \
./src/wavetable.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F0 -DSTM32F091RCTx -DDEBUG -DSTM32F091 -DUSE_STDPERIPH_DRIVER -I"/Users/andrewswanback/Documents/workspace/362-Mini-Project/StdPeriph_Driver/inc" -I"/Users/andrewswanback/Documents/workspace/362-Mini-Project/inc" -I"/Users/andrewswanback/Documents/workspace/362-Mini-Project/CMSIS/device" -I"/Users/andrewswanback/Documents/workspace/362-Mini-Project/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


