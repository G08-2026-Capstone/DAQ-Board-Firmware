import serial, time

port = "COM8"   # change to your STM32's COM port
ser = serial.Serial(port, baudrate=115200, timeout=1)

# Baudrate here is ignored for USB CDC, but pyserial needs a number.

total = 0
start = time.time()

while True:
    data = ser.read(16384)  # read up to 4 KB
    total += len(data)
    elapsed = time.time() - start
    if elapsed >= 1.0:  # once per second
        print(f"{(total / elapsed / 1024)*(8/1000):.1f} Mb/s")
        total = 0
        start = time.time()
