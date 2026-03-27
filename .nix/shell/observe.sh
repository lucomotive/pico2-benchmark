set -e

if [ -z "$1" ]; then
    echo "Usage: pico-observe <project>"
    exit 1
fi

PROJECT=$1


#tio --log /dev/ttyACM0 --log-file results/$PROJECT/benchmark.csv
mkdir -p results/$PROJECT
cat /dev/ttyACM0 | tee results/$PROJECT/benchmark.csv
