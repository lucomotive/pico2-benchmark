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
from prompt_toolkit import PromptSession
from prompt_toolkit.completion import WordCompleter
from prompt_toolkit.formatted_text import HTML
from prompt_toolkit.patch_stdout import patch_stdout
from prompt_toolkit.history import FileHistory
from prompt_toolkit import prompt



ser = serial.Serial('/dev/ttyACM0', baudrate=115200, timeout=1)


def receive_results(name, precision):
    #with open(f"results/{name}-{precision}-test-algorihtm.csv", "w") as f:
    while True:
        line = ser.readline().decode().strip()
        match line.split(" "):
            case ["SOF", *args]:
                print("Starting writing results to file...")
                break
            case ["ERROR", *args]:
                print(line)
                return
            case anything if anything:
                print(line)
                #f.write(line + "\n")

    with open(f"results/{name}-{precision}.csv", "w") as f:
        # begin writing
        while True:
            line = ser.readline().decode().strip()
            match line:
                case "EOF":
                    print("Benchmark finished")
                    break
                case "Out of memory":
                    print(line)
                    print("Please reboot pico")
                    break
                case anything if anything:
                    f.write(line + "\n")
                    f.flush()






def help():
    print(f'''Usage: run --benchmark=<qr|lu|matmul|...> --precision=<float|double>

'exit' or ctrl-c to quit application''')

async def main():
    session = PromptSession(
        completer=WordCompleter(['exit', 'help', 'run']),
        history=FileHistory('.console_history'),
    )

    # patch_stdout stops incoming pico lines from breaking the input prompt
    with patch_stdout():
        while True:
            try:
                cmd = await session.prompt_async(
                    HTML("<ansigreen>pico</ansigreen> <b>></b> ")
                )
                parts = cmd.strip().split(" ")
                if not parts:
                     continue

                match parts:
                    case ['run', *args]:
                        allowedArgs = [
                            "benchmark",
                            "precision",
                            "iterations",
                            "min-dimension",
                            "max-dimension"
                        ]
                        requiredArgs = [
                            "benchmark"
                        ]
                        defaultArgs = {
                            "precision": "float",
                            "min-dimension": 5,
                            "max-dimension": 90,
                            "iterations": 1000
                        }

                        # merge defaultArgs with provided args with | (duplicates prioritise right side)
                        argsDict = defaultArgs | dict((arg.lstrip("-").split("=", 1)) for arg in args)

                        # parse to int
                        argsDict["iterations"] = int(argsDict["iterations"])
                        argsDict["min-dimension"] = int(argsDict["min-dimension"])
                        argsDict["max-dimension"] = int(argsDict["max-dimension"])

                        missing = [item for item in requiredArgs if item not in argsDict]
                        if missing:
                            print(f"The following args are missing: {missing}")
                            continue
                        invalid = [item for item in argsDict if item not in allowedArgs]
                        if invalid:
                            print(f"The following args are invaild: {invalid}")
                            continue

                        message = json.dumps(argsDict) + "\n"
                        ser.write(message.encode())
                        receive_results(name=argsDict["benchmark"], precision=argsDict["precision"])

                    case ['exit']:
                        print("Exiting...")
                        break
                    case _:
                        help()
            except (EOFError, KeyboardInterrupt):
                print("Exiting...")
                break

asyncio.run(main())
