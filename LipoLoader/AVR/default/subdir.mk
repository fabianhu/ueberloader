################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../adc.c \
../main.c \
../menu.c \
../pwm.c \
../touchpad.c \
../ueberloader.c \
../usart.c 

OBJS += \
./adc.o \
./main.o \
./menu.o \
./pwm.o \
./touchpad.o \
./ueberloader.o \
./usart.o 

C_DEPS += \
./adc.d \
./main.d \
./menu.d \
./pwm.d \
./touchpad.d \
./ueberloader.d \
./usart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -mmcu=atxmega32a4 -DF_CPU=32000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


