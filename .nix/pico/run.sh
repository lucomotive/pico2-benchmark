#!/bin/bash
set -e

TARGET=$1
PROJECT=$2

cat /dev/ttyACM0 | tee "$TARGET/results/$PROJECT".csv
