#!/bin/bash
set -e

PROJECT=$1
PRECISION=$2

cat /dev/ttyACM0 | tee "results/$PROJECT-$PRECISION".csv
