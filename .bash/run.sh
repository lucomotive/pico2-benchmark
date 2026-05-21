#!/bin/bash
set -e

TARGET=$1
PROJECT=$2
PRECISION=$3

cat /dev/ttyACM0 | tee "results/$TARGET/$PROJECT-$PRECISION".csv
