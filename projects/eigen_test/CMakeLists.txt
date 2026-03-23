cmake_minimum_required(VERSION 3.13)

# must be before include!!
set(PICO_BOARD pico2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
#include(pico_sdk_import.cmake)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)


project(eigen_test C CXX ASM)


# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# EIGEN FINDEN: Erlaube Suche auf dem Host-System (wichtig für Cross-Compiling)
#set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)
#set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/eigen-3.4" ${CMAKE_PREFIX_PATH})
#include_directories(${CMAKE_SOURCE_DIR}/eigen-3.4)
target_include_directories(eigen_test PRIVATE ${CMAKE_SOURCE_DIR}/eigen-3.4)
#find_package(Eigen3 REQUIRED NO_MODULE)

# rest of your project
add_executable(eigen_test
    main.c
)

# Add pico_stdlib library which aggregates commonly used features
# Add pico time to measure benchmarks
# Add Eigen3 library
target_link_libraries(eigen_test
    pico_stdlib
    pico_time
    Eigen3::Eigen
)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(eigen_test)
