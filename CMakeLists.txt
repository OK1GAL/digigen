# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/ok1gal/pico/pico-sdk")

set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(digigen C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(digigen  digigen.c 
                        si5351.c
                        tx_handling.c
                        RTTY.c
                        CW.c
                        console.c
                        )

pico_set_program_name(digigen "digigen")
pico_set_program_version(digigen "0.1")

pico_enable_stdio_uart(digigen 0)
pico_enable_stdio_usb(digigen 1)

# Add the standard library to the build
target_link_libraries(digigen
        pico_stdlib
        pico_multicore
        )

# Add the standard include files to the build
target_include_directories(digigen PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries(digigen 
        hardware_i2c
        hardware_timer
        hardware_clocks
        )

pico_add_extra_outputs(digigen)

