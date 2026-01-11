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
    print("Enter message to send (integer/hex for byte, or string):")
    while True:
        user_input = input("> ")
        try:
            # Try to parse as integer (supports 0x prefix)
            command = bytes([int(user_input, 0)])
        except ValueError:
            # If not an integer or out of byte range, send as string
            command = user_input.encode('utf-8')

        print(f"sending {command}")
        ser.write(command)

def send_control_signal(ser: serial.Serial, start: bool):
    """Sends 0x1 to start or 0x0 to stop the device."""
    command = bytes([1]) if start else bytes([0])
    print(f"Sending {'start' if start else 'stop'} command: {command}")
    ser.write(command)


def main():
    port = "COM8"
    ser = serial.Serial(port, baudrate=115200, timeout=0)

    write(ser)

main()