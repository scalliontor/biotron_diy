#define TOUCH_PIN 4
#define ALPHA 0.05  // EMA - chỉnh nhỏ lại nếu muốn mượt hơn

float smooth = 0;

void setup() {
    Serial.begin(115200);
    
    // Thêm hàm tăng độ nhạy Touch (như đã phát hiện ở bước trước)
    touchSetCycles(0x1000, 0x1000);
    
    // Khởi tạo giá trị ban đầu cho EMA
    smooth = (float)touchRead(TOUCH_PIN);
}

void loop() {
    int raw = touchRead(TOUCH_PIN);
    
    // EMA filter
    smooth = ALPHA * raw + (1.0 - ALPHA) * smooth;
    
    // Map thành 0-100 (chỉnh 2 số này theo thực tế)
    // Dùng kiểu float để chia tỷ lệ mịn hơn (Arduino map() mặc định dùng số nguyên int)
    float value = (smooth - 15.0) * (0.0 - 100.0) / (40.0 - 15.0) + 100.0;
    
    // Constrain giá trị trong khoảng 0-100
    if (value < 0) value = 0;
    if (value > 100) value = 100;
    
    // In ra Serial
    Serial.printf("[🌿 TOUCH] %.2f\n", value);
    
    delay(20);
}
