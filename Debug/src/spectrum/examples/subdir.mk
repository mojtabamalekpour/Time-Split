################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/spectrum/examples/adhoc-aloha-ideal-phy-matrix-propagation-loss-model.cc \
../src/spectrum/examples/adhoc-aloha-ideal-phy-with-microwave-oven.cc \
../src/spectrum/examples/adhoc-aloha-ideal-phy.cc 

OBJS += \
./src/spectrum/examples/adhoc-aloha-ideal-phy-matrix-propagation-loss-model.o \
./src/spectrum/examples/adhoc-aloha-ideal-phy-with-microwave-oven.o \
./src/spectrum/examples/adhoc-aloha-ideal-phy.o 

CC_DEPS += \
./src/spectrum/examples/adhoc-aloha-ideal-phy-matrix-propagation-loss-model.d \
./src/spectrum/examples/adhoc-aloha-ideal-phy-with-microwave-oven.d \
./src/spectrum/examples/adhoc-aloha-ideal-phy.d 


# Each subdirectory must supply rules for building sources it contributes
src/spectrum/examples/%.o: ../src/spectrum/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


