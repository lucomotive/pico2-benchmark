import serial
import time

ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
time.sleep(1)  # let Pico boot

def send_command(cmd):
    ser.write((cmd + '\n').encode())
    response = ser.readline().decode().strip()
    print(f"Pico: {response}")




print('Welcome to pico2 eigen benchmark.')
while True:
    print('Choose a benchmark to run...')
    choice = input()

    send_command(choice)
