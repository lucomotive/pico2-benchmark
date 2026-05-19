cmake_minimum_required(VERSION 3.13)


# use pico sdk
set(PICO_BOARD pico2)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# init project
project(pico-benchmark CXX C ASM)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# invlude packages
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)
find_package(Eigen3 REQUIRED NO_MODULE)

# compiler settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_RELEASE   "-O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)


macro(add_pico_benchmark NAME)
    add_executable(${NAME} ${ARGN})
    target_link_libraries(${NAME}
        pico_stdlib
        pico_rand
        pico_time
        Eigen3::Eigen
    )
    #target_include_directories(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    pico_enable_stdio_usb(${NAME} 1)
    pico_enable_stdio_uart(${NAME} 0)
    pico_add_extra_outputs(${NAME})
endmacro()


add_subdirectory(qr)
