################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Ellipse.cpp \
../src/TrackingPoints.cpp \
../src/main.cpp \
../src/xmlParser.cpp 

OBJS += \
./src/Ellipse.o \
./src/TrackingPoints.o \
./src/main.o \
./src/xmlParser.o 

CPP_DEPS += \
./src/Ellipse.d \
./src/TrackingPoints.d \
./src/main.d \
./src/xmlParser.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


