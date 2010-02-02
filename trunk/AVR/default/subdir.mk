################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../adc.c \
../adc_driver.c \
../main.c \
../ueberloader.c 

OBJS += \
./adc.o \
./adc_driver.o \
./main.o \
./ueberloader.o 

C_DEPS += \
./adc.d \
./adc_driver.d \
./main.d \
./ueberloader.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -mmcu=atxmega32a4 -DF_CPU=3200000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


