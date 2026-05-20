cmake_minimum_required(VERSION 3.13)


# use pico sdk
set(PICO_BOARD pico2)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# init project
project(pico-benchmark CXX C ASM)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()



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


# include packages
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)
find_package(Eigen3 REQUIRED NO_MODULE)


macro(add_pico_benchmark NAME)
    add_executable(${NAME} ${ARGN})
    target_link_libraries(${NAME}
        pico_stdlib
        pico_rand
        pico_time
        Eigen3::Eigen
    )
    target_include_directories(${NAME} PRIVATE include)
    target_include_directories(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    pico_enable_stdio_usb(${NAME} 1)
    pico_enable_stdio_uart(${NAME} 0)
    pico_add_extra_outputs(${NAME})
endmacro()

add_subdirectory(benchmarks/alloc-heap)
add_subdirectory(benchmarks/alloc-stack)
add_subdirectory(benchmarks/det)
add_subdirectory(benchmarks/eigen-values)
add_subdirectory(benchmarks/inverse)
add_subdirectory(benchmarks/llt)
add_subdirectory(benchmarks/lu)
add_subdirectory(benchmarks/matmul)
add_subdirectory(benchmarks/qr)
add_subdirectory(benchmarks/rank)
add_subdirectory(benchmarks/read-flash)
