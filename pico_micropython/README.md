# Pico MicroPython Test Scripts

Test scripts cho Raspberry Pi Pico với MicroPython.
Tham khảo kiến trúc từ [Biotron firmware](https://github.com/Playtronica/biotron-firmware).

## Yêu cầu

- Raspberry Pi Pico với **MicroPython** đã flash
- `mpremote`: `pip install mpremote`

## Sơ đồ mạch

### Kênh cây (GPIO 26 / ADC0)
```
3.3V ──[Kẹp lá 1]── CÂY ──[Kẹp lá 2]──┬── GPIO 26
                                         │
                                       [10M]  (thử 4.7M hoặc 1M nếu signal yếu)
                                         │
                                        GND
```

### Kênh ánh sáng (GPIO 27 / ADC1)
```
3.3V ──[LDR 5mm]──┬── GPIO 27
                   │
                 [10k]
                   │
                  GND
```

## Chạy test

```bash
# Flash MicroPython trước (giữ BOOTSEL + cắm USB -> kéo .uf2 vào RPI-RP2)
# Tải: https://micropython.org/download/RPI_PICO/

# Test LDR
mpremote run test_ldr.py

# Test plant (dùng ngón tay thay cây)
mpremote run test_plant.py

# Test cả 2 kênh + EMA filter
mpremote run test_both.py
```

## Thứ tự debug

1. `test_ldr.py` — che tay vào LDR, số phải thay đổi
2. `test_plant.py` — chạm 2 ngón tay vào 2 đầu dây, số phải tăng
3. Nếu plant signal yếu → đổi trở 10M → 4.7M → 1M
4. `test_both.py` — test tổng hợp với EMA + trigger detection
