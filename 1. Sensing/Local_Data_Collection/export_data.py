# python export_data.py -p COM3 -b 115200 -d ./data -l kendo_data

import argparse
import os
import time
import serial
import serial.tools.list_ports
import re

# Settings
DEFAULT_BAUD = 115200
DEFAULT_LABEL = "sensor_data"

# Match sensor data
DATA_PATTERN = re.compile(r'^-?\d+(\.\d+)?(,-?\d+(\.\d+)?){7}$')

# Write CSV data
def write_csv(data, out_path):
    try:
        with open(out_path, 'a') as file:
            if os.stat(out_path).st_size == 0:
                file.write("timestamp,accelX,accelY,accelZ,gyroX,gyroY,gyroZ,roll,pitch\n")
            
            timestamp = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
            file.write(f"{timestamp},{data}\n")
        print("Data written:", data)
    except IOError as e:
        print("ERROR:", e)

# Command line arguments
parser = argparse.ArgumentParser(description="Serial Data Collection CSV")
parser.add_argument('-p', '--port', required=True, help="Serial port to connect to")
parser.add_argument('-b', '--baud', type=int, default=DEFAULT_BAUD, help="Baud rate (default = 115200)")
parser.add_argument('-d', '--directory', type=str, default=".", help="Output directory for the file")
parser.add_argument('-l', '--label', type=str, default=DEFAULT_LABEL, help="Filename for the CSV")

# Check avaiable serial port
print("\nAvailable serial ports:")
available_ports = serial.tools.list_ports.comports()
for port, desc, hwid in sorted(available_ports):
    print(f"  {port} : {desc} [{hwid}]")

# Parse arguments
args = parser.parse_args()
port = args.port
baud = args.baud
out_dir = args.directory
label = args.label

# Set output path
out_path = os.path.join(out_dir, f"{label}.csv")

# Configure serial port
ser = serial.Serial(port, baudrate=baud)

# Create output directory
os.makedirs(out_dir, exist_ok=True)

# 10-second countdown before starting data collection
print("Starting data recording in:")
for i in range(10, 0, -1):
    print(f"{i} seconds...")
    time.sleep(1)

print("Recording started. Listening to serial port...")

# Main loop for reading serial data
try:
    while True:
        if ser.in_waiting > 0:
        # Read raw data from serial
            raw_line = ser.readline()

            try:
                # Decode
                line = raw_line.decode('utf-8', errors='ignore').strip()
                print("Decoded Line:", line)

                # Check if the line matches the expected sensor data format
                if DATA_PATTERN.match(line):
                    write_csv(line, out_path)
                else:
                    print("Filtered out non-data line:", line)
            except Exception as e:
                print("Error decoding line:", e)


except KeyboardInterrupt: # Ctrl+c to stop
    pass

print("Closing serial port")
ser.close()
