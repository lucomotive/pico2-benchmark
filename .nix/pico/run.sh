#!/bin/bash
set -e

TARGET="pico"
PROJECT=$1

cat /dev/ttyACM0 | tee "$TARGET/results/$PROJECT".csv
