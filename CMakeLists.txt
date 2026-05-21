cmake_minimum_required(VERSION 3.13)


# host | pico | pico2
set(TARGET_PLATFORM "pico")


if((TARGET_PLATFORM STREQUAL "pico") OR (TARGET_PLATFORM STREQUAL "pico2"))
  # use pico sdk
  set(PICO_BOARD ${TARGET_PLATFORM})
  include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
endif()

# init project
project(pico-benchmark CXX C ASM)

if((TARGET_PLATFORM STREQUAL "pico") OR (TARGET_PLATFORM STREQUAL "pico2"))
  # initialize the Raspberry Pi Pico SDK
  pico_sdk_init()
endif()


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



# set flags for code
if(TARGET_PLATFORM STREQUAL "pico")
  add_compile_definitions(BUILD_PICO)
elseif(TARGET_PLATFORM STREQUAL "host")
  add_compile_definitions(BUILD_HOST)
endif()



# find packages
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)
find_package(Eigen3 REQUIRED NO_MODULE)

# include packages
set(COMMON_LIBS
    Eigen3::Eigen
)
set(PICO_LIBS
    pico_stdlib
    pico_rand
    pico_time
)
macro(add_benchmark NAME)
  add_executable(${NAME} ${ARGN})
  if((TARGET_PLATFORM STREQUAL "pico") OR (TARGET_PLATFORM STREQUAL "pico2"))
    target_link_libraries(${NAME} ${COMMON_LIBS} ${PICO_LIBS})
  elseif(TARGET_PLATFORM STREQUAL "host")
    target_link_libraries(${NAME} ${COMMON_LIBS})
  endif()

  target_include_directories(${NAME} PRIVATE include)
  target_include_directories(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

  if((TARGET_PLATFORM STREQUAL "pico") OR (TARGET_PLATFORM STREQUAL "pico2"))
    pico_enable_stdio_usb(${NAME} 1)
    pico_enable_stdio_uart(${NAME} 0)
    pico_add_extra_outputs(${NAME})
  endif()

endmacro()

add_subdirectory(benchmarks/alloc-heap)
add_subdirectory(benchmarks/alloc-stack)
add_subdirectory(benchmarks/det)
add_subdirectory(benchmarks/det-stack)
add_subdirectory(benchmarks/eigen-values)
add_subdirectory(benchmarks/inverse)
add_subdirectory(benchmarks/llt)
add_subdirectory(benchmarks/lu)
add_subdirectory(benchmarks/matmul)
add_subdirectory(benchmarks/matmul-stack)
add_subdirectory(benchmarks/qr)
add_subdirectory(benchmarks/rank)
add_subdirectory(benchmarks/read-flash)
