"""
Biotron DIY - Test LDR (Photoresistor)
Hardware:
  - LDR 5mm: 3.3V -> LDR -> GPIO27
  - 10k resistor: GPIO27 -> GND
Run: mpremote run test_ldr.py
"""
from machine import ADC
import time

ldr = ADC(27)  # GPIO 27 = ADC1

print("=== LDR Test ===")
print("Che tay vao LDR de test phan hoi")
print("Ctrl+C de dung\n")

while True:
    val = ldr.read_u16() >> 4  # 16-bit -> 12-bit (0-4095)
    bar = "#" * (val // 100)
    print(f"Light: {val:4d} |{bar}")
    time.sleep_ms(100)
