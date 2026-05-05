# this script communicates with the pico
# use: run --benchmark=<...> --precision=<float|double>
#
# also available:
# --min-dimension -> min width/height of calculated matrices
# --max-dimension -> max width/height of calculated matrices
# --iterations    -> how many measurements to perform

import asyncio
import serial
import json
import sys
import argparse


ser = serial.Serial('/dev/ttyACM0', baudrate=115200, timeout=1)


def receive_results():
    file = None

    while True:
        line = ser.readline()
        decoded = line.decode().strip()
        if not decoded:
            continue
        match decoded.split(" "):
            case ["SOF", type, name, precision]:
                if file:
                    print("Received start of file signal, but file already open")
                    continue
                print("Received start of file signal. Opening file...")
                file = open(f"results/{name.upper()}-{precision.upper()}-{type.upper()}.csv", "w")
            case ["EOF", *_]:
                if not file:
                    print("Received end of file signal, but no file open")
                    continue
                print("Received end of file signal. Closing file...")
                file.close()
                file = None
            case ["EOT"]:
                # end of transmission
                if file:
                    file.close()
                break
            case ["Out", "of", "memory"]:
                print(decoded)
                print("Please reboot pico")
                break
            case anything if anything:
                print(decoded)
                if file:
                    file.write(decoded + "\n")
                    file.flush()
    # close file after loop
    if file:
        file.close()





def main():
    parser = argparse.ArgumentParser(prog='console')
    parser.add_argument('--file', help='file in json format specifying the benchmark parameters', type=str, required=True)
    args = parser.parse_args(sys.argv[1:])


    file = open(args.file, "r")
    content = file.read()
    file.close()

    # check for valid json. also force json to be a one-liner
    j = json.loads(content)
    message = json.dumps(j)
    print(f"Sending to pico...\n{content}")
    ser.write(f"{message}\n".encode())
    receive_results()


main()
