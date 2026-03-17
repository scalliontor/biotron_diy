"""
Biotron DIY - Test Plant channel (ADC voltage divider)
Hardware:
  - 3.3V -> Clip leaf 1 -> PLANT -> Clip leaf 2 -> GPIO26
  - 10M resistor (or 4.7M / 1M): GPIO26 -> GND
  - No plant? Use fingers to bridge the two clips.
Run: mpremote run test_plant.py
"""
from machine import ADC
import time

plant = ADC(26)  # GPIO 26 = ADC0

print("=== Plant Test ===")
print("Cham 2 ngon tay vao 2 dau day de test (thay cho la cay)")
print("Ctrl+C de dung\n")

while True:
    val = plant.read_u16() >> 4  # 0-4095
    bar = "#" * (val // 100)
    print(f"Plant: {val:4d} |{bar}")
    time.sleep_ms(100)
