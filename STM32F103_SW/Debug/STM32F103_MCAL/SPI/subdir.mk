################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32F103_MCAL/SPI/SPI.c 

OBJS += \
./STM32F103_MCAL/SPI/SPI.o 

C_DEPS += \
./STM32F103_MCAL/SPI/SPI.d 


# Each subdirectory must supply rules for building sources it contributes
STM32F103_MCAL/SPI/%.o STM32F103_MCAL/SPI/%.su: ../STM32F103_MCAL/SPI/%.c STM32F103_MCAL/SPI/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -I"F:/Embedded/LearnInDepth/WS/Ultrasonic_Servo_Gate/STM32F103_MCAL" -I"F:/Embedded/LearnInDepth/WS/Ultrasonic_Servo_Gate/Application" -I"F:/Embedded/LearnInDepth/WS/Ultrasonic_Servo_Gate/HAL/LCD" -I"F:/Embedded/LearnInDepth/WS/Ultrasonic_Servo_Gate/STM32F103_MCAL" -I"F:/Embedded/LearnInDepth/WS/Ultrasonic_Servo_Gate/STM32F103_MCAL/EXTI" -I"F:/Embedded/LearnInDepth/WS/Ultrasonic_Servo_Gate/STM32F103_MCAL/GPIO" -I"F:/Embedded/LearnInDepth/WS/Ultrasonic_Servo_Gate/STM32F103_MCAL/I2C" -I"F:/Embedded/LearnInDepth/WS/Ultrasonic_Servo_Gate/STM32F103_MCAL/RCC" -I"F:/Embedded/LearnInDepth/WS/Ultrasonic_Servo_Gate/STM32F103_MCAL/SPI" -I"F:/Embedded/LearnInDepth/WS/Ultrasonic_Servo_Gate/STM32F103_MCAL/UART" -I"F:/Embedded/LearnInDepth/WS/Ultrasonic_Servo_Gate/HAL/Servo" -I"F:/Embedded/LearnInDepth/WS/Ultrasonic_Servo_Gate/STM32F103_MCAL/Timer2_4" -I"F:/Embedded/LearnInDepth/WS/Ultrasonic_Servo_Gate/HAL/Ultrasonic" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-STM32F103_MCAL-2f-SPI

clean-STM32F103_MCAL-2f-SPI:
	-$(RM) ./STM32F103_MCAL/SPI/SPI.d ./STM32F103_MCAL/SPI/SPI.o ./STM32F103_MCAL/SPI/SPI.su

.PHONY: clean-STM32F103_MCAL-2f-SPI

