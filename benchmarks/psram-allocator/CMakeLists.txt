add_benchmark(psram-allocator-float float.cpp)
add_benchmark(psram-allocator-double double.cpp)


add_compile_definitions(PICO_CXX_DISABLE_ALLOCATION_OVERRIDES=1)
