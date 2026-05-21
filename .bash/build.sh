#!/bin/bash
set -e

BUILD_DIR="build"
TARGET=$1
PROJECT=$2
PRECISION=$3

echo "Building $PROJECT with $PRECISION for target $TARGET"

# build process
cmake -B "$BUILD_DIR" -S . -DCMAKE_BUILD_TYPE=Release -DTARGET_PLATFORM=${TARGET}
cmake --build "$BUILD_DIR" -j$(nproc) --target "$PROJECT-$PRECISION"

if [[ $TARGET == "pico" || $TARGET == "pico2" ]]; then
    # find uf2 file
    UF2=$(ls "$BUILD_DIR/benchmarks/$PROJECT/$PROJECT-$PRECISION".uf2 2>/dev/null | head -1)
    if [ -z "$UF2" ]; then
        echo "No .uf2 file found"
        exit 1
    fi

    echo "Flashing $UF2..."
    picotool load "$UF2" --force --execute --verify

    sleep 3
    echo "Waiting for pico to start..."
fi

project-run $TARGET $PROJECT $PRECISION
