cmake_minimum_required (VERSION 3.10)
project (Choke)

#----------------
# AVR basic setup
#----------------
set (AVRDUDE avrdude)
set (AVRCPP avr-g++)
set (AVROBJCPY avr-objcopy)
set (AVRBOARD atmega328p)
set (CPUFREQ 16000000)
set (AVRPORT "/dev/ttyACM0")

#--------------------
# Project definitions
#--------------------
set (Choke_VERSION_MAJOR 0)
set (Choke_VERSION_MINOR 1)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_BUILD_TYPE Debug)

#-------------
# C++ compiler
#-------------
set (CMAKE_CXX_COMPILER ${AVRCPP})

#-----------------
# Add include/libs
#-----------------
message("${CMAKE_SOURCE_DIR}")
include_directories ("${CMAKE_SOURCE_DIR}/libs/avr-stl/include"
                     "${CMAKE_SOURCE_DIR}/tools/simavr/simavr/sim"
                     "${CMAKE_SOURCE_DIR}/tools/simavr/simavr/sim/avr")
link_directories("${CMAKE_SOURCE_DIR}/libs/avr-stl")

#-----------------
# Compiler options
#-----------------
add_definitions("-DF_CPU=${CPUFREQ}")
add_definitions("-mmcu=${AVRBOARD}")
add_definitions("-fpack-struct")
add_definitions("-fshort-enums")
add_definitions("-ffunction-sections")
add_definitions("-fdata-sections")

set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -mmcu=${AVRBOARD} -Wl,-gc-sections")

#-------------
# Source files
#-------------
add_subdirectory (src)
add_executable (choke
                src/main.cpp)

#add_custom_command(COMMAND get_events)
add_custom_target(hex ALL ${AVROBJCPY} -j .text -j .data -O ihex choke choke.hex)
add_custom_target(avrdude ${AVRDUDE} -c arduino -p ${AVRBOARD} -P ${AVRPORT} -b 115200 -v -U flash:w:choke.hex DEPENDS choke)
