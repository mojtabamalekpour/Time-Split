################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/sixlowpan/model/sixlowpan-header.cc \
../src/sixlowpan/model/sixlowpan-net-device.cc 

OBJS += \
./src/sixlowpan/model/sixlowpan-header.o \
./src/sixlowpan/model/sixlowpan-net-device.o 

CC_DEPS += \
./src/sixlowpan/model/sixlowpan-header.d \
./src/sixlowpan/model/sixlowpan-net-device.d 


# Each subdirectory must supply rules for building sources it contributes
src/sixlowpan/model/%.o: ../src/sixlowpan/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


