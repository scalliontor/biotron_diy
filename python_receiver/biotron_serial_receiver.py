import serial
import time
import argparse

def process_data(line):
    # Expected format: T:12.34,L:56.78
    try:
        parts = line.split(',')
        touch_str = parts[0].split(':')[1]
        light_str = parts[1].split(':')[1]
        
        touch_val = float(touch_str)
        light_val = float(light_str)
        
        # Visualize Touch (Inverted so closer = larger bar)
        touch_display = max(100 - touch_val, 0)
        touch_bar = "█" * int(touch_display / 4)
        
        # Visualize Light
        light_bar = "☀" * int(light_val / 100)
        
        print(f"[🌿 TOUCH] {touch_val:6.2f} | {touch_bar}")
        print(f"[☀️ LIGHT] {light_val:6.2f} | {light_bar}")
    except (IndexError, ValueError) as e:
        # Ignore lines that don't match our specific pattern (e.g. boot messages)
        pass

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", default="/dev/ttyUSB0", help="Serial port to connect to")
    parser.add_argument("--baud", type=int, default=115200, help="Baud rate")
    args = parser.parse_args()

    print("=" * 50)
    print("🌱 Biotron Python Serial Receiver Started")
    print(f"🔌 Connecting to {args.port} at {args.baud} baud")
    print("=" * 50)
    
    try:
        ser = serial.Serial(args.port, args.baud, timeout=1)
        time.sleep(2) # Wait for arduino to reset
        print("Waiting for data from ESP32...\n")
        
        while True:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').strip()
                process_data(line)
    except serial.SerialException as e:
        print(f"Error opening serial port: {e}")
    except KeyboardInterrupt:
        print("\nReceiver stopped.")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
