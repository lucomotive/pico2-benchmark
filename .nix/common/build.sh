#!/bin/bash
set -e

BUILD_DIR="build"
TARGET="common"

# build process
cmake -B "$TARGET/$BUILD_DIR" -S $TARGET -DCMAKE_BUILD_TYPE=Release
cmake --build "$TARGET/$BUILD_DIR" -j$(nproc)
