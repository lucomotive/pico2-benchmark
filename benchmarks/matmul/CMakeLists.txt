add_benchmark(matmul-float float.cpp)
add_benchmark(matmul-double double.cpp)



# target_compile_definitions(matmul-float PRIVATE
#    PICO_MALLOC_PANIC=0
# )
