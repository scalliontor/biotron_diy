"""
Biotron DIY - Test ca 2 kenh Plant + LDR voi EMA filter
Tham khao: Biotron firmware global.c::filter_freq()

Hardware:
  Plant channel:
    3.3V -> [Clip] -> PLANT -> [Clip] -> GPIO26
    10M (hoac 4.7M / 1M) resistor: GPIO26 -> GND

  Light channel:
    3.3V -> LDR 5mm -> GPIO27
    10k resistor: GPIO27 -> GND

Run: mpremote run test_both.py
"""
from machine import ADC
import time

plant = ADC(26)
ldr   = ADC(27)

# EMA filter - ref: filter_freq() trong biotron firmware global.c
# alpha nho = muot hon, tre hon (Biotron dung k=filterPercent tuong duong 1-alpha)
ALPHA = 0.05

def ema(val, prev, alpha=ALPHA):
    return alpha * val + (1.0 - alpha) * prev

ema_p = 0.0
ema_l = 0.0
TRIGGER_THRESHOLD = 50  # nguong delta de detect su kien

print("=== Biotron DIY - Dual Channel Test ===")
print("P=Plant(GPIO26)  L=Light(GPIO27)  dP=Delta(trigger detection)")
print("-" * 60)

while True:
    p = plant.read_u16() >> 4   # 0-4095
    l = ldr.read_u16()   >> 4   # 0-4095

    ema_p = ema(p, ema_p)
    ema_l = ema(l, ema_l)

    delta_p = p - ema_p

    trigger = " <<< TRIGGER!" if abs(delta_p) > TRIGGER_THRESHOLD else ""
    print(f"P:{p:4d} EMA:{ema_p:6.1f}  |  L:{l:4d} EMA:{ema_l:6.1f}  |  dP:{delta_p:+6.1f}{trigger}")
    time.sleep_ms(100)
