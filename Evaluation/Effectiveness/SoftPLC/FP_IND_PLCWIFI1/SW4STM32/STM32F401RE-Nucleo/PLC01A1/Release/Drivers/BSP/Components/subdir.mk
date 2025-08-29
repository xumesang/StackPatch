################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/Components/CLT01-38SQ7/CLT01_38S.c \
C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/Components/ISO8200BQ/ISO8200BQ.c \
C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/Components/VNI8200XP/VNI8200XP.c 

OBJS += \
./Drivers/BSP/Components/CLT01_38S.o \
./Drivers/BSP/Components/ISO8200BQ.o \
./Drivers/BSP/Components/VNI8200XP.o 

C_DEPS += \
./Drivers/BSP/Components/CLT01_38S.d \
./Drivers/BSP/Components/ISO8200BQ.d \
./Drivers/BSP/Components/VNI8200XP.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/CLT01_38S.o: C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/Components/CLT01-38SQ7/CLT01_38S.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F401xE -DUSE_STM32F4XX_NUCLEO -DUSART_PRINT_MSG -DPLC01A1 -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Projects/Multi/Applications/FP_IND_PLCWIFI1/Inc" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Middlewares/ST/STM32_SPWF01SA" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/STM32F4xx-Nucleo" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/X-NUCLEO/X-NUCLEO-IDW01M1" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/Components/VNI8200XP" -I../../../../../../../../Middlewares/ST/STM32_LADDER_LYBRAY -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/CMSIS/Include" -I../../../../../../../../Drivers/BSP/Common/PLC01 -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Middlewares/ST/STM32_SPWF01SA/Inc" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Middlewares/ST/STM32_SPWF01SA/Utils" -I../../../../../../../../Drivers/BSP/Common/OUT01 -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/X-NUCLEO/X-NUCLEO-OUT01A1" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/X-NUCLEO/X-NUCLEO-PLC01A1" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/Components/ISO8200BQ" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/Components/CLT01-38SQ7"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/BSP/Components/ISO8200BQ.o: C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/Components/ISO8200BQ/ISO8200BQ.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F401xE -DUSE_STM32F4XX_NUCLEO -DUSART_PRINT_MSG -DPLC01A1 -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Projects/Multi/Applications/FP_IND_PLCWIFI1/Inc" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Middlewares/ST/STM32_SPWF01SA" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/STM32F4xx-Nucleo" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/X-NUCLEO/X-NUCLEO-IDW01M1" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/Components/VNI8200XP" -I../../../../../../../../Middlewares/ST/STM32_LADDER_LYBRAY -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/CMSIS/Include" -I../../../../../../../../Drivers/BSP/Common/PLC01 -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Middlewares/ST/STM32_SPWF01SA/Inc" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Middlewares/ST/STM32_SPWF01SA/Utils" -I../../../../../../../../Drivers/BSP/Common/OUT01 -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/X-NUCLEO/X-NUCLEO-OUT01A1" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/X-NUCLEO/X-NUCLEO-PLC01A1" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/Components/ISO8200BQ" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/Components/CLT01-38SQ7"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/BSP/Components/VNI8200XP.o: C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/Components/VNI8200XP/VNI8200XP.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F401xE -DUSE_STM32F4XX_NUCLEO -DUSART_PRINT_MSG -DPLC01A1 -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Projects/Multi/Applications/FP_IND_PLCWIFI1/Inc" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Middlewares/ST/STM32_SPWF01SA" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/STM32F4xx-Nucleo" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/X-NUCLEO/X-NUCLEO-IDW01M1" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/Components/VNI8200XP" -I../../../../../../../../Middlewares/ST/STM32_LADDER_LYBRAY -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/CMSIS/Include" -I../../../../../../../../Drivers/BSP/Common/PLC01 -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Middlewares/ST/STM32_SPWF01SA/Inc" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Middlewares/ST/STM32_SPWF01SA/Utils" -I../../../../../../../../Drivers/BSP/Common/OUT01 -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/X-NUCLEO/X-NUCLEO-OUT01A1" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/X-NUCLEO/X-NUCLEO-PLC01A1" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/Components/ISO8200BQ" -I"C:/TestAc6/en.fp-ind-plcwifi1/STM32CubeFunctionPack_PLCWIFI1_V1.0.1/Drivers/BSP/Components/CLT01-38SQ7"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


