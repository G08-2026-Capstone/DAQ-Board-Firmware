import serial, time
import threading


def throughputTest(ser: serial.Serial):
    print("reading start")
    total = 0
    start = time.perf_counter()

    command = bytes([1]) #  Send start command
    print(f"sending {command}")
    ser.write(command)

    startTest = start = time.perf_counter()
    while time.perf_counter() - startTest < 20:
        data = ser.read(65536)
        total += len(data)
        elapsed = time.perf_counter() - start
        if elapsed >= 1.0:
            mbps = (total * 8) / 1_000_000 / elapsed  # megabits/s
            print(f"{mbps:.2f} Mb/s")
            total = 0
            start = time.perf_counter()
    
    command = bytes([0]) #  Send start command
    print(f"sending {command}")
    ser.write(command)

def write(ser: serial.Serial):
    print("write start")
    while True:
        command = bytes([int(input())])
        print(f"sending {command}")
        ser.write(command)


def main():
    port = "COM8"
    ser = serial.Serial(port, baudrate=115200, timeout=0)

    throughputTest(ser)

main()