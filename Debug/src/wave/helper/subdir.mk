################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/wave/helper/wave-mac-helper.cc \
../src/wave/helper/wifi-80211p-helper.cc 

OBJS += \
./src/wave/helper/wave-mac-helper.o \
./src/wave/helper/wifi-80211p-helper.o 

CC_DEPS += \
./src/wave/helper/wave-mac-helper.d \
./src/wave/helper/wifi-80211p-helper.d 


# Each subdirectory must supply rules for building sources it contributes
src/wave/helper/%.o: ../src/wave/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


