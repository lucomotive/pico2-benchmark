#!/bin/bash
set -e

TARGET=$1
PROJECT=$2
PRECISION=$3

if [ $TARGET -eq "pico" || $TARGET -eq "pico2" ]; then
    cat /dev/ttyACM0 | tee "results/$TARGET/$PROJECT-$PRECISION".csv
elif [ $TARGET -eq "host" ]; then
    ./$BUILD_DIR/benchmarks/$PROJECT/$PROJECT-$PRECISION | tee "results/$TARGET/$PROJECT-$PRECISION".csv
fi
