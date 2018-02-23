################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/wave/model/higher-tx-tag.cc \
../src/wave/model/ocb-wifi-mac.cc \
../src/wave/model/vendor-specific-action.cc \
../src/wave/model/wave-mac-low.cc 

OBJS += \
./src/wave/model/higher-tx-tag.o \
./src/wave/model/ocb-wifi-mac.o \
./src/wave/model/vendor-specific-action.o \
./src/wave/model/wave-mac-low.o 

CC_DEPS += \
./src/wave/model/higher-tx-tag.d \
./src/wave/model/ocb-wifi-mac.d \
./src/wave/model/vendor-specific-action.d \
./src/wave/model/wave-mac-low.d 


# Each subdirectory must supply rules for building sources it contributes
src/wave/model/%.o: ../src/wave/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


