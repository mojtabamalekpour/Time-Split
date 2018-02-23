################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../build/src/emu/bindings/ns3module.cc.6.o 

CC_SRCS += \
../build/src/emu/bindings/ns3module.cc 

OBJS += \
./build/src/emu/bindings/ns3module.o 

CC_DEPS += \
./build/src/emu/bindings/ns3module.d 


# Each subdirectory must supply rules for building sources it contributes
build/src/emu/bindings/%.o: ../build/src/emu/bindings/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


