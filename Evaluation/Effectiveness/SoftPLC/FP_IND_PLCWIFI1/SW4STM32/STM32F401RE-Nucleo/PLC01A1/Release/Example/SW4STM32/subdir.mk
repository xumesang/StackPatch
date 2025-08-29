################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Projects/Multi/Applications/FP_IND_PLCWIFI1/SW4STM32/STM32F401RE-Nucleo/startup_stm32f401xe.s 

OBJS += \
./Example/SW4STM32/startup_stm32f401xe.o 


# Each subdirectory must supply rules for building sources it contributes
Example/SW4STM32/startup_stm32f401xe.o: C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Projects/Multi/Applications/FP_IND_PLCWIFI1/SW4STM32/STM32F401RE-Nucleo/startup_stm32f401xe.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


