cmake_minimum_required(VERSION 3.5)
set(ARDUINO_CPU atmega328)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME hpglDriver)
project(${PROJECT_NAME})

link_directories($ENV{UserProfile}/Documents/Arduino/libraries/)

set(${PROJECT_NAME}_SRCS hpglDriver.cpp calibration.cpp HPGLPlotter.cpp)
set(${PROJECT_NAME}_HDRS calibration.h HPGLPlotter.h configuration.h)
set(${PROJECT_NAME}_BOARD nano)
set(${PROJECT_NAME}_PORT com6)
set(${PROJECT_NAME}_SERIAL putty -serial @SERIAL_PORT@ -sercfg 9600,8,n,1,X)

generate_arduino_firmware(${CMAKE_PROJECT_NAME})
