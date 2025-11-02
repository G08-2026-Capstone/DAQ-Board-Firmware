import serial, time

port = "COM8"
ser = serial.Serial(port, baudrate=115200, timeout=0)

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
