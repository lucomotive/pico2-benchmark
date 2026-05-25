#!/bin/bash
set -e

BUILD_DIR="build"
TARGET=$1
PROJECT=$2
PRECISION=$3

if [[ $TARGET == "pico" || $TARGET == "pico2" || $TARGET == "pico2-psram" ]]; then
    mkdir -p results/$TARGET
    cat /dev/ttyACM0 | tee "results/$TARGET/$PROJECT-$PRECISION".csv
elif [[ $TARGET == "host" ]]; then
    mkdir -p results/$TARGET
    ./$BUILD_DIR/benchmarks/$PROJECT/$PROJECT-$PRECISION | tee "results/$TARGET/$PROJECT-$PRECISION".csv
fi
