################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/TravellingSalesmanProblem.cpp \
../src/TravellingSalesmanProblem_test.cpp \
../src/UndirectedGraph.cpp \
../src/UndirectedGraph_test.cpp \
../src/main.cpp 

OBJS += \
./src/TravellingSalesmanProblem.o \
./src/TravellingSalesmanProblem_test.o \
./src/UndirectedGraph.o \
./src/UndirectedGraph_test.o \
./src/main.o 

CPP_DEPS += \
./src/TravellingSalesmanProblem.d \
./src/TravellingSalesmanProblem_test.d \
./src/UndirectedGraph.d \
./src/UndirectedGraph_test.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


