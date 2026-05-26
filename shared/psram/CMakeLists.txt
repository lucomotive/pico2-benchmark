add_library(psram STATIC
    psram.c
)

target_include_directories(psram PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(psram
    pico_stdlib
    hardware_spi
)

# sources
# https://github.com/micropython/micropython/blob/293d17e4463142588ce4fd45f82b0bd628735676/ports/rp2/rp2_psram.c
# https://github.com/tjko/fanpico/blob/main/src/psram.c
