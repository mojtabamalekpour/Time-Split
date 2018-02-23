################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/wave/examples/wave-simple-80211p.cc 

OBJS += \
./src/wave/examples/wave-simple-80211p.o 

CC_DEPS += \
./src/wave/examples/wave-simple-80211p.d 


# Each subdirectory must supply rules for building sources it contributes
src/wave/examples/%.o: ../src/wave/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


