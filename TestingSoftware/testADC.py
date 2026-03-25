import serial
import struct
import sys
import math

# --- Welford's Algorithm Class for Running Statistics ---
class RunningStats:
    def __init__(self):
        self.n = 0
        self.mean = 0.0
        self.m2 = 0.0

    def push(self, x):
        self.n += 1
        delta = x - self.mean
        self.mean += delta / self.n
        delta2 = x - self.mean
        self.m2 += delta * delta2

    def get_stats(self):
        if self.n < 2:
            return self.mean, 0.0, 0.0
        variance = self.m2 / self.n  # Population variance
        return self.mean, variance, math.sqrt(variance)

# --- ADC Configuration ---
V_REF = 1.2
GAIN = 1.0 
SCALING = V_REF / (GAIN * (2**23))
OFFSET_CALIBRATION = [0,0,0]  # Hard-coded offset values for each channel

PACKET_SIZE = 16 
STRUCT_FORMAT = '<Iiii'

def main():
    port = "COM8"
    
    # Initialize one tracker per channel (CH0, CH1, CH2)
    stats_trackers = [RunningStats() for _ in range(3)]
    
    try:
        ser = serial.Serial(port, baudrate=115200, timeout=1)
        ser.reset_input_buffer()
        print(f"Connected to {port}. Polling data...")
    except serial.SerialException as e:
        print(f"Error connecting: {e}")
        return

    ser.write(bytes([1])) 

    try:
        while True:
            if ser.in_waiting >= PACKET_SIZE:
                raw_packet = ser.read(PACKET_SIZE)
                
                # Unpack and convert
                data = struct.unpack(STRUCT_FORMAT, raw_packet)
                counter = data[0]
            
                # Apply scaling AND subtract the hard-coded offset
                voltages = [(x * SCALING) - OFFSET_CALIBRATION[i] for i, x in enumerate(data[1:])]

                # Update stats for each channel
                for i in range(3):
                    stats_trackers[i].push(voltages[i])

                # Get latest stats
                s0 = stats_trackers[0].get_stats() # Mean, Var, Std
                s1 = stats_trackers[1].get_stats()
                s2 = stats_trackers[2].get_stats()

                # Print formatted output
                sys.stdout.write(
                    f"\rPkt: {counter:04d} | "
                    f"CH0: {voltages[0]:6.4f}V (μ: {s0[0]:6.4f}, σ: {s0[2]:6.4f}) | "
                    f"CH1: {voltages[1]:6.4f}V (μ: {s1[0]:6.4f}, σ: {s1[2]:6.4f}) | "
                    f"CH2: {voltages[2]:6.4f}V (μ: {s2[0]:6.4f}, σ: {s2[2]:6.4f})"
                )
                sys.stdout.flush()
                
    except KeyboardInterrupt:
        print("\nStopping...")
    finally:
        ser.write(bytes([0])) 
        ser.close()

if __name__ == "__main__":
    main()