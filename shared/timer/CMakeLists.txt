add_library(timer STATIC
    timer.cpp
)

target_include_directories(timer PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(timer
    pico_stdlib
)
