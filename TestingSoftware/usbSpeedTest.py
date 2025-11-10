import serial, time
import threading


port = "COM8"
ser = serial.Serial(port, baudrate=115200, timeout=0)


def read(ser: serial.Serial):
    print("reading start")
    total = 0
    start = time.perf_counter()

    while True:
        data = ser.read(65536)
        total += len(data)
        elapsed = time.perf_counter() - start
        if elapsed >= 1.0:
            mbps = (total * 8) / 1_000_000 / elapsed  # megabits/s
            print(f"{mbps:.2f} Mb/s")
            total = 0
            start = time.perf_counter()

def write(ser: serial.Serial):
    print("write start")
    while True:
        command = bytes([int(input())])
        print(f"sending {command}")
        ser.write(command)

def main():

    read_thread = threading.Thread(target=read, args=(ser,))
    read_thread.start()

    write_thread = threading.Thread(target=write, args=(ser,))
    write_thread.start()

main()