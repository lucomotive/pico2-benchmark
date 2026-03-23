cmake_minimum_required(VERSION 3.13)
project(bachelorarbeit)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Auto-symlink compile_commands.json to project root
execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)

# Makes include/ available to all subprojects
include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(projects/add_sub)
