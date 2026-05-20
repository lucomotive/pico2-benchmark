#!/bin/bash
set -e

TARGET="pico"
PROJECT=$1
PRECISION=$2

cat /dev/ttyACM0 | tee "$TARGET/results/$PROJECT-$PRECISION".csv
