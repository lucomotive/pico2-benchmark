set -e

if [ -z "$1" ]; then
    echo "Usage: pico-observe <project>"
    exit 1
fi

PROJECT=$1

tio /dev/ttyACM0 | tee results/$PROJECT/benchmark.csv
