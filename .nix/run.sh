#!/bin/bash
set -e

BUILD_DIR="build"

# build process
cmake -B "$BUILD_DIR" -S .
cmake --build "$BUILD_DIR" -j$(nproc)

# find uf2 file
UF2=$(ls "$BUILD_DIR/"*.uf2 2>/dev/null | head -1)
if [ -z "$UF2" ]; then
    echo "No .uf2 file found"
    exit 1
fi

echo "Flashing $UF2..."
picotool load "$UF2" --force --execute
