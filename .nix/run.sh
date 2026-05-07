#!/bin/bash
set -e

BUILD_DIR="build"
# tracks last successful flash
STAMP="$BUILD_DIR/.flash_stamp"

# build process
cmake -B "$BUILD_DIR" -S .
cmake --build "$BUILD_DIR" -j$(nproc)

# find uf2 file
UF2=$(ls "$BUILD_DIR/"*.uf2 2>/dev/null | head -1)
if [ -z "$UF2" ]; then
    echo "No .uf2 file found"
    exit 1
fi

# compare stamp with .uf2 file
# flash only if .uf2 is newer that stamp
if [ "$UF2" -nt "$STAMP" ]; then
    echo "Flashing $UF2..."
    picotool load "$UF2" --force --execute
    touch "$STAMP"
    echo "Waiting for pico to start..."
    sleep 3
else
    echo "Build unchanged, skipping flash..."
fi
