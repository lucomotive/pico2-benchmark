#!/usr/bin/env bash
set -e # Exit on error

echo "Starting custom build script..."

# Example build steps
mkdir -p build
cd build
cmake ../blink
#make

echo "Build finished!"
