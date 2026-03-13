import serial
import json
import argparse

def draw(channels, values, baselines):
    print("\033[H\033[J", end="")  # clear terminal
    print("=== BIOTRON MPR121 DEBUG ===\n")
    print(f"  {'CHAN':<7} {'FILTER':>6}  {'BASE':>6}  {'DIFF':>6}  STATUS")
    print(f"  {'----':<7} {'------':>6}  {'----':>6}  {'----':>6}  ------")
    for i, (t, v, b) in enumerate(zip(channels, values, baselines)):
        diff = b - v
        touch_str = "🌿 TOUCH" if t else ""
        print(f"  ELE{i:02d}   {v:6d}  {b:6d}  {diff:6d}  {touch_str}")
    print(f"\n  FILTER = gia tri cam bien")
    print(f"  BASE   = gia tri nen (tu dong calibrate)")
    print(f"  DIFF   = BASE - FILTER (duong = dang cham)")
    print(f"\n  Neu FILTER rat thap (<20) = kiem tra day cap!")
    print(f"\nCtrl+C de dung")

parser = argparse.ArgumentParser()
parser.add_argument("--port", default="COM3")
parser.add_argument("--baud", type=int, default=115200)
args = parser.parse_args()

print(f"Ket noi {args.port}...")

with serial.Serial(args.port, args.baud, timeout=1) as ser:
    try:
        while True:
            line = ser.readline().decode("utf-8", errors="ignore").strip()
            if not line.startswith("{"):
                continue
            data = json.loads(line)
            baselines = data.get("base", [0]*12)
            draw(data["touch"], data.get("val", [0]*12), baselines)
    except KeyboardInterrupt:
        print("\nDa dung.")
