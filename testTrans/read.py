import serial
import time
import sys

# Configuration
# /dev/rfcomm0 is the standard binding port for Linux Bluetooth Serial
SERIAL_PORT = '/dev/rfcomm0'
BAUD_RATE = 115200

def read_bluetooth_data():
    print(f"Attempting to connect to {SERIAL_PORT}...")
    
    try:
        # Initialize serial connection
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"Connected to {SERIAL_PORT}!")
        
        # Clear buffer to avoid reading old data
        ser.reset_input_buffer()

        while True:
            if ser.in_waiting > 0:
                # Read line, decode bytes to string, strip whitespace
                line = ser.readline().decode('utf-8').strip()
                
                if line:
                    try:
                        # Parse the CSV format "val1,val2"
                        parts = line.split(',')
                        if len(parts) == 2:
                            val1 = int(parts[0])
                            val2 = int(parts[1])
                            
                            # Do something with your integers here
                            print(f"Received -> Integer 1: {val1} | Integer 2: {val2}")
                        else:
                            print(f"Malformed packet: {line}")
                            
                    except ValueError:
                        print(f"Error parsing data: {line}")
                        
    except serial.SerialException as e:
        print(f"\nCould not open serial port {SERIAL_PORT}.")
        print("Make sure you have paired the ESP32 and bound it using 'rfcomm'.")
        print(f"System Error: {e}")
    except KeyboardInterrupt:
        print("\nProgram stopped by user.")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("Serial connection closed.")

if __name__ == "__main__":
    read_bluetooth_data()
