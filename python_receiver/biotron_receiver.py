import argparse
import math
from pythonosc import dispatcher
from pythonosc import osc_server

def touch_handler(address, *args):
    """
    Callback function that handles incoming Touch sensor OSC messages.
    """
    if len(args) == 0:
        return
        
    value = args[0]
    # Creating a simple visual bar for the terminal output based on data
    # Normal touch values for ESP32 usually range from 0 to 100
    # Lower value means stronger touch. We invert it for display.
    display_val = max(100 - value, 0)
    bar_length = int(display_val / 4)
    bar = "█" * bar_length
    print(f"[🌿 TOUCH] {value:6.2f} | {bar}")

def light_handler(address, *args):
    """
    Callback function that handles incoming Light sensor OSC messages.
    """
    if len(args) == 0:
        return
        
    value = args[0]
    # Creating a visual bar for the terminal output
    # Light values on ESP32 12-bit ADC range from 0 to 4095
    bar_length = int(value / 100)
    bar = "☀" * bar_length
    print(f"[☀️ LIGHT] {value:6.2f} | {bar}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--ip",
        default="0.0.0.0", help="The ip to listen on")
    parser.add_argument("--port",
        type=int, default=8000, help="The port to listen on")
    args = parser.parse_args()

    # Create a dispatcher to map incoming OSC addresses to handler functions
    disp = dispatcher.Dispatcher()
    disp.map("/biotron/touch", touch_handler)
    disp.map("/biotron/light", light_handler)
    
    # Optional: A catch-all handler for unknown addresses
    # disp.set_default_handler(lambda address, *args: print(f"DEFAULT {address}: {args}"))

    server = osc_server.ThreadingOSCUDPServer(
        (args.ip, args.port), disp)
        
    print("="*50)
    print(f"🌱 Biotron Python OSC Receiver Started")
    print(f"📡 Listening on {server.server_address}")
    print("="*50)
    print("Waiting for data from ESP32...\n")
    
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nReceiver stopped.")
