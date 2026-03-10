import serial
import time
import argparse

def process_data(line):
    # Expected format: [🌿 TOUCH] 75.00
    try:
        if "[🌿 TOUCH]" in line:
            parts = line.split(']')
            touch_str = parts[1].strip()
            touch_val = float(touch_str)
            
            # Visualize Touch (Value is 0-100)
            bar_length = int(touch_val / 4)
            touch_bar = "█" * bar_length
            
            print(f"[🌿 TOUCH] {touch_val:6.2f} | {touch_bar}")
    except (IndexError, ValueError) as e:
        # Ignore lines that don't match our specific pattern (e.g. boot messages)
        pass

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", default="/dev/ttyUSB0", help="Serial port to connect to")
    parser.add_argument("--baud", type=int, default=115200, help="Baud rate")
    args = parser.parse_args()

    print("=" * 50)
    print("🌱 Capacitance Receiver Started")
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
