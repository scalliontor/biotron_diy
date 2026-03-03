# Biotron DIY Project

Dự án này cung cấp 2 phương pháp đo lường sinh lý học thực vật để biến cây trồng thành một thiết bị đầu vào (input device) phục vụ cho Interactive Art / Biofeedback.

## Cài đặt chung

### Yêu cầu hệ thống
- **Phần cứng**: ESP32, Cáp MicroUSB/Type-C truyền data, Chậu cây. 
- **Phần mềm**: Python 3.7+, `arduino-cli` (với core `esp32:esp32` đã được cài đặt).

### Cài đặt thư viện Python
Mở Terminal và cài đặt thư viện giao tiếp Serial:
```bash
pip install pyserial
```

---

## Cách 1: Đo Điện Dung (Capacitive Touch)
*Nhạy với việc con người tiến lại gần hoặc chạm trực tiếp vào lá cây.*

### Phần cứng
Chỉ cần 2 dây (có thể dùng kẹp cá sấu):
- `GPIO 4` ──── nối tới thân cây / lá cây (bọc giấy bạc nếu cần).
- `GND`    ──── cắm xuống đất ẩm (Lưu ý: Nếu giá trị trả về bị bão hòa về 0, hãy rút dây GND này ra khỏi đất và cho người dùng tự chạm vào mạch GND).

### Biên dịch và Nạp Code
Mở Terminal tại thư mục dự án và chạy lần lượt:

1. **Biên dịch:**
```bash
arduino-cli compile --fqbn esp32:esp32:esp32 01_measure_capacitance
```

2. **Nạp Code (Upload):**
*(Lưu ý sửa cổng `/dev/ttyUSB0` thành cổng thực tế của mạch trên máy bạn - ví dụ `COM3` trên Windows)*
```bash
arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32 01_measure_capacitance
```

### Chạy Python Receiver
```bash
python 01_measure_capacitance/receiver_capacitance.py --port /dev/ttyUSB0
```
Bạn sẽ thấy đồ họa thanh Bar hiển thị độ chạm `[🌿 TOUCH]` trực quan trên Terminal. Phím tắt để dừng: `Ctrl+C`.

---

## Cách 2: Đo Trở Kháng Sinh Học (Biological Resistance)
*Nhạy với trạng thái sinh học của cây (lượng nước, trao đổi chất), không nhạy với sự có mặt của con người.*

### Phần cứng cần có
- 1 Điện trở **1MΩ**
- Kẹp cá sấu / Que cắm điện cực (2 cái)

### Sơ đồ đấu nối
```
ESP32
├── GPIO 25 ── [Điện trở 1MΩ] ─┬── Điện cực A (cắm vào cây)
├── GPIO 32 ───────────────────┘        ~~cây~~
└── GND     ────────────────────── Điện cực B (cắm vào cây, cách A 5cm)
```

### Biên dịch và Nạp Code
Mở Terminal tại thư mục dự án và chạy lần lượt:

1. **Biên dịch:**
```bash
arduino-cli compile --fqbn esp32:esp32:esp32 02_measure_resistance
```

2. **Nạp Code (Upload):**
```bash
arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32 02_measure_resistance
```

### Chạy Python Receiver
```bash
python 02_measure_resistance/receiver_resistance.py --port /dev/ttyUSB0
```
Bạn sẽ thấy đồ họa thanh Bar hiển thị chỉ số trở kháng `[⚡ RESIST]` trực quan trên Terminal. Phím tắt để dừng: `Ctrl+C`.

---
*Lưu ý: Luôn đảm bảo bạn đã tắt script Python (`Ctrl+C`) trước khi chạy lệnh nạp code `upload`, nếu không cổng Serial sẽ báo lỗi `Device or resource busy`.*
