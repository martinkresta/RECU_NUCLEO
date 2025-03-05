################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Martin/GIT/UHA_COMMON/Src/BMS.c \
C:/Martin/GIT/UHA_COMMON/Src/COM.c \
C:/Martin/GIT/UHA_COMMON/Src/ELM.c \
C:/Martin/GIT/UHA_COMMON/Src/LOG.c \
C:/Martin/GIT/UHA_COMMON/Src/MCAN.c \
C:/Martin/GIT/UHA_COMMON/Src/METEO.c \
C:/Martin/GIT/UHA_COMMON/Src/OW.c \
C:/Martin/GIT/UHA_COMMON/Src/PID.c \
C:/Martin/GIT/UHA_COMMON/Src/RTC.c \
C:/Martin/GIT/UHA_COMMON/Src/SENSIRION.c \
C:/Martin/GIT/UHA_COMMON/Src/TEMP.c \
C:/Martin/GIT/UHA_COMMON/Src/UHAMON.c \
C:/Martin/GIT/UHA_COMMON/Src/UI.c \
C:/Martin/GIT/UHA_COMMON/Src/VARS.c \
C:/Martin/GIT/UHA_COMMON/Src/circbuf.c \
C:/Martin/GIT/UHA_COMMON/Src/watchdog.c 

OBJS += \
./Common/Src/BMS.o \
./Common/Src/COM.o \
./Common/Src/ELM.o \
./Common/Src/LOG.o \
./Common/Src/MCAN.o \
./Common/Src/METEO.o \
./Common/Src/OW.o \
./Common/Src/PID.o \
./Common/Src/RTC.o \
./Common/Src/SENSIRION.o \
./Common/Src/TEMP.o \
./Common/Src/UHAMON.o \
./Common/Src/UI.o \
./Common/Src/VARS.o \
./Common/Src/circbuf.o \
./Common/Src/watchdog.o 

C_DEPS += \
./Common/Src/BMS.d \
./Common/Src/COM.d \
./Common/Src/ELM.d \
./Common/Src/LOG.d \
./Common/Src/MCAN.d \
./Common/Src/METEO.d \
./Common/Src/OW.d \
./Common/Src/PID.d \
./Common/Src/RTC.d \
./Common/Src/SENSIRION.d \
./Common/Src/TEMP.d \
./Common/Src/UHAMON.d \
./Common/Src/UI.d \
./Common/Src/VARS.d \
./Common/Src/circbuf.d \
./Common/Src/watchdog.d 


# Each subdirectory must supply rules for building sources it contributes
Common/Src/BMS.o: C:/Martin/GIT/UHA_COMMON/Src/BMS.c Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../../UHA_COMMON/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Src/COM.o: C:/Martin/GIT/UHA_COMMON/Src/COM.c Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../../UHA_COMMON/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Src/ELM.o: C:/Martin/GIT/UHA_COMMON/Src/ELM.c Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../../UHA_COMMON/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Src/LOG.o: C:/Martin/GIT/UHA_COMMON/Src/LOG.c Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../../UHA_COMMON/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Src/MCAN.o: C:/Martin/GIT/UHA_COMMON/Src/MCAN.c Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../../UHA_COMMON/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Src/METEO.o: C:/Martin/GIT/UHA_COMMON/Src/METEO.c Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../../UHA_COMMON/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Src/OW.o: C:/Martin/GIT/UHA_COMMON/Src/OW.c Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../../UHA_COMMON/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Src/PID.o: C:/Martin/GIT/UHA_COMMON/Src/PID.c Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../../UHA_COMMON/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Src/RTC.o: C:/Martin/GIT/UHA_COMMON/Src/RTC.c Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../../UHA_COMMON/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Src/SENSIRION.o: C:/Martin/GIT/UHA_COMMON/Src/SENSIRION.c Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../../UHA_COMMON/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Src/TEMP.o: C:/Martin/GIT/UHA_COMMON/Src/TEMP.c Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../../UHA_COMMON/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Src/UHAMON.o: C:/Martin/GIT/UHA_COMMON/Src/UHAMON.c Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../../UHA_COMMON/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Src/UI.o: C:/Martin/GIT/UHA_COMMON/Src/UI.c Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../../UHA_COMMON/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Src/VARS.o: C:/Martin/GIT/UHA_COMMON/Src/VARS.c Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../../UHA_COMMON/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Src/circbuf.o: C:/Martin/GIT/UHA_COMMON/Src/circbuf.c Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../../UHA_COMMON/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/Src/watchdog.o: C:/Martin/GIT/UHA_COMMON/Src/watchdog.c Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../../UHA_COMMON/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Common-2f-Src

clean-Common-2f-Src:
	-$(RM) ./Common/Src/BMS.cyclo ./Common/Src/BMS.d ./Common/Src/BMS.o ./Common/Src/BMS.su ./Common/Src/COM.cyclo ./Common/Src/COM.d ./Common/Src/COM.o ./Common/Src/COM.su ./Common/Src/ELM.cyclo ./Common/Src/ELM.d ./Common/Src/ELM.o ./Common/Src/ELM.su ./Common/Src/LOG.cyclo ./Common/Src/LOG.d ./Common/Src/LOG.o ./Common/Src/LOG.su ./Common/Src/MCAN.cyclo ./Common/Src/MCAN.d ./Common/Src/MCAN.o ./Common/Src/MCAN.su ./Common/Src/METEO.cyclo ./Common/Src/METEO.d ./Common/Src/METEO.o ./Common/Src/METEO.su ./Common/Src/OW.cyclo ./Common/Src/OW.d ./Common/Src/OW.o ./Common/Src/OW.su ./Common/Src/PID.cyclo ./Common/Src/PID.d ./Common/Src/PID.o ./Common/Src/PID.su ./Common/Src/RTC.cyclo ./Common/Src/RTC.d ./Common/Src/RTC.o ./Common/Src/RTC.su ./Common/Src/SENSIRION.cyclo ./Common/Src/SENSIRION.d ./Common/Src/SENSIRION.o ./Common/Src/SENSIRION.su ./Common/Src/TEMP.cyclo ./Common/Src/TEMP.d ./Common/Src/TEMP.o ./Common/Src/TEMP.su ./Common/Src/UHAMON.cyclo ./Common/Src/UHAMON.d ./Common/Src/UHAMON.o ./Common/Src/UHAMON.su ./Common/Src/UI.cyclo ./Common/Src/UI.d ./Common/Src/UI.o ./Common/Src/UI.su ./Common/Src/VARS.cyclo ./Common/Src/VARS.d ./Common/Src/VARS.o ./Common/Src/VARS.su ./Common/Src/circbuf.cyclo ./Common/Src/circbuf.d ./Common/Src/circbuf.o ./Common/Src/circbuf.su ./Common/Src/watchdog.cyclo ./Common/Src/watchdog.d ./Common/Src/watchdog.o ./Common/Src/watchdog.su

.PHONY: clean-Common-2f-Src

