import serial
import time
import sys

# Configuration
SERIAL_PORT = '/dev/rfcomm0'
BAUD_RATE = 115200

def read_bluetooth_data():
    print(f"Attempting to connect to {SERIAL_PORT}...")
    
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"Connected to {SERIAL_PORT}!")
        
        ser.reset_input_buffer()

        while True:
            if ser.in_waiting > 0:
                # Read line, decode bytes to string, strip whitespace
                line = ser.readline().decode('utf-8').strip()
                
                if line:
                    try:
                        # Parse the CSV format "load1,load2,a1,a2,a3,a4"
                        parts = line.split(',')
                        
                        # We now expect 6 values
                        if len(parts) == 6:
                            # Load cell readings
                            load1 = int(parts[0])
                            load2 = int(parts[1])
                            
                            # Analog readings
                            a1 = int(parts[2]) # Pin 32
                            a2 = int(parts[3]) # Pin 33
                            a3 = int(parts[4]) # Pin 34
                            a4 = int(parts[5]) # Pin 35
                            
                            # Display data
                            print(f"Load: {load1:<8} {load2:<8} | Analog: {a1:<5} {a2:<5} {a3:<5} {a4:<5}")
                        else:
                            print(f"Malformed packet (len={len(parts)}): {line}")
                            
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
