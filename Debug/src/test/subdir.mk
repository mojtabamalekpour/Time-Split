################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/test/csma-system-test-suite.cc \
../src/test/global-routing-test-suite.cc \
../src/test/mobility-test-suite.cc \
../src/test/static-routing-test-suite.cc 

OBJS += \
./src/test/csma-system-test-suite.o \
./src/test/global-routing-test-suite.o \
./src/test/mobility-test-suite.o \
./src/test/static-routing-test-suite.o 

CC_DEPS += \
./src/test/csma-system-test-suite.d \
./src/test/global-routing-test-suite.d \
./src/test/mobility-test-suite.d \
./src/test/static-routing-test-suite.d 


# Each subdirectory must supply rules for building sources it contributes
src/test/%.o: ../src/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


