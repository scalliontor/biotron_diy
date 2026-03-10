// Biotron - Đo Trở Kháng Sinh Học bằng RC charge timing
//
// CÁCH CẮM ĐIỆN CỰC ĐÚNG:
//   Điện cực A (GPIO25/32): cắm vào THÂN CÂY (hoặc kẹp vào lá)
//   Điện cực B (GND):       cắm vào ĐẤT (hoặc kẹp vào lá khác, cách xa)
//   → Dòng điện BUỘC phải đi qua cây, không đi tắt qua đất
//
// Sơ đồ: GPIO25 ──[1MΩ]──┬── Điện cực A (THÂN CÂY / LÁ)
//         GPIO32 ─────────┘
//         GND ────────────────── Điện cực B (ĐẤT / LÁ KHÁC)

#define PIN_DRIVE 

#define PIN_READ  32
#define ALPHA 0.15

// Ngưỡng ADC để coi là "đã nạp đủ" (khoảng 80% của 3.3V)
#define CHARGE_THRESHOLD 3200

// Timeout (microseconds) - nếu quá lâu thì coi như mạch hở
#define TIMEOUT_US 100000

float smooth = 0;
bool firstRun = true;

void setup() {
    pinMode(PIN_DRIVE, OUTPUT);
    digitalWrite(PIN_DRIVE, LOW);
    pinMode(PIN_READ, INPUT_PULLDOWN);
    Serial.begin(115200);
    delay(500);
    Serial.println("=== Biotron Resistance Sensor Ready ===");
    Serial.println("Dien cuc A (GPIO25/32) → cam vao THAN CAY");
    Serial.println("Dien cuc B (GND)       → cam vao DAT");
    Serial.println("=========================================");
}

// Đo thời gian nạp RC (microseconds)
unsigned long measureChargeTime() {
    // Bước 1: Xả tụ hoàn toàn
    digitalWrite(PIN_DRIVE, LOW);
    delay(5);
    
    // Bước 2: Bắt đầu nạp và đo thời gian
    unsigned long startTime = micros();
    digitalWrite(PIN_DRIVE, HIGH);
    
    while (analogRead(PIN_READ) < CHARGE_THRESHOLD) {
        if (micros() - startTime > TIMEOUT_US) {
            digitalWrite(PIN_DRIVE, LOW);
            return TIMEOUT_US;
        }
    }
    
    unsigned long chargeTime = micros() - startTime;
    digitalWrite(PIN_DRIVE, LOW);
    return chargeTime;
}

void loop() {
    unsigned long chargeTime = measureChargeTime();
    
    float raw = (float)chargeTime;
    
    // EMA filter
    if (firstRun) {
        smooth = raw; // khởi tạo ngay lần đầu, không đợi hội tụ
        firstRun = false;
    } else {
        smooth = ALPHA * raw + (1.0 - ALPHA) * smooth;
    }
    
    // Log scale: 10μs - 100000μs → 0 - 100
    float value = 0;
    if (smooth > 10) {
        value = (log(smooth) - log(10.0)) / (log(100000.0) - log(10.0)) * 100.0;
    }
    if (value < 0) value = 0;
    if (value > 100) value = 100;

    // Hiển thị cả giá trị mapped VÀ thời gian nạp thực (μs) để debug
    Serial.printf("[🌿 RESIST] %.2f | %luus\n", value, chargeTime);
    delay(50);
}
