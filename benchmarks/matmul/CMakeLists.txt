add_benchmark(matmul-float float.cpp)
add_benchmark(matmul-double double.cpp)

add_compile_definitions(PICO_CXX_DISABLE_ALLOCATION_OVERRIDES=1)
