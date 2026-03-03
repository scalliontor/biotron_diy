import serial
import time
import argparse

def process_data(line):
    # Expected format: [🌿 RESIST] 85.00
    try:
        if "[🌿 RESIST]" in line:
            parts = line.split(']')
            resist_str = parts[1].strip()
            resist_val = float(resist_str)
            
            # Visualize Resistance (Value is 0-100)
            bar_length = int(resist_val / 4)
            resist_bar = "⚡" * bar_length
            
            print(f"[🌿 RESIST] {resist_val:6.2f} | {resist_bar}")
    except (IndexError, ValueError) as e:
        # Ignore lines that don't match our specific pattern (e.g. boot messages)
        pass

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", default="/dev/ttyUSB0", help="Serial port to connect to")
    parser.add_argument("--baud", type=int, default=115200, help="Baud rate")
    args = parser.parse_args()

    print("=" * 50)
    print("🌱 Biological Resistance Receiver Started")
    print(f"🔌 Connecting to {args.port} at {args.baud} baud")
    print("=" * 50)
    
    try:
        ser = serial.Serial(args.port, args.baud, timeout=1)
        time.sleep(2) # Wait for arduino to reset
        print("Waiting for data from ESP32...\n")
        
        while True:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                if line:
                    process_data(line)
    except serial.SerialException as e:
        print(f"Error opening serial port: {e}")
    except KeyboardInterrupt:
        print("\nReceiver stopped.")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
