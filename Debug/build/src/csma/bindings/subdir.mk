################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../build/src/csma/bindings/ns3module.cc.5.o 

CC_SRCS += \
../build/src/csma/bindings/ns3module.cc 

OBJS += \
./build/src/csma/bindings/ns3module.o 

CC_DEPS += \
./build/src/csma/bindings/ns3module.d 


# Each subdirectory must supply rules for building sources it contributes
build/src/csma/bindings/%.o: ../build/src/csma/bindings/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


