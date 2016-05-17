cmake_minimum_required(VERSION 3.5)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME hpglDriver)
project(${PROJECT_NAME})

link_directories($ENV{UserProfile}/Documents/Arduino/libraries)

set(${PROJECT_NAME}_SRCS hpglDriver.cpp calibration.cpp HPGLPlotter.cpp)
set(${PROJECT_NAME}_HDRS calibration.h HPGLPlotter.h)
set(${PROJECT_NAME}_BOARD nano328)

generate_arduino_firmware(${CMAKE_PROJECT_NAME})
