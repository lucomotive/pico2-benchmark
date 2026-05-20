#!/bin/bash
set -e

BUILD_DIR="build"
TARGET="pico"
PROJECT=$1
PRECISION=$2

# build process
cmake -B "$TARGET/$BUILD_DIR" -S "$TARGET" -DCMAKE_BUILD_TYPE=Release
cmake --build "$TARGET/$BUILD_DIR" -j$(nproc) --target "$PROJECT-$PRECISION"

# find uf2 file
UF2=$(ls "$TARGET/$BUILD_DIR/$PROJECT/$PROJECT-$PRECISION".uf2 2>/dev/null | head -1)
if [ -z "$UF2" ]; then
    echo "No .uf2 file found"
    exit 1
fi

echo "Flashing $UF2..."
picotool load "$UF2" --force --execute --verify

sleep 3
echo "Waiting for pico to start..."

pico-run $PROJECT $PRECISION
