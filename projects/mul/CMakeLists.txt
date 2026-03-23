cmake_minimum_required(VERSION 3.13)

# must be before include!!
set(PICO_BOARD pico2)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_RELEASE "-O3")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Auto-symlink compile_commands.json to project root
execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
#include(pico_sdk_import.cmake)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)


project(mul C CXX ASM)

# EIGEN FINDEN: Erlaube Suche auf dem Host-System (wichtig für Cross-Compiling)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)
find_package(Eigen3 REQUIRED NO_MODULE)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(mul
    main.cpp
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(mul
    pico_stdlib
    pico_rand
    pico_time
    Eigen3::Eigen
)

# Standard-Ausgabe über USB (Serial)
pico_enable_stdio_usb(mul 1)
pico_enable_stdio_uart(mul 0)



# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(mul)
