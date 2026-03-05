#define TOUCH_PIN 4
#define ALPHA 0.05

float smooth = 0;
float baseline = 0;

void setup() {
    Serial.begin(115200);
    
    // Tăng độ nhạy Touch (như đã phát hiện ở bước trước)
    touchSetCycles(0x1000, 0x1000);
    
    // Calib baseline lúc khởi động - đừng chạm cây
    Serial.println("Calibrating baseline... Please don't touch the plant.");
    float sum = 0;
    for (int i = 0; i < 100; i++) {
        sum += touchRead(TOUCH_PIN);
        delay(10);
    }
    baseline = sum / 100.0;
    smooth = baseline;
    Serial.printf("Baseline calibrated: %.2f\n", baseline);
}

void loop() {
    int raw = touchRead(TOUCH_PIN);
    
    // EMA filter
    smooth = ALPHA * raw + (1.0 - ALPHA) * smooth;
    
    // Chỉ đo thay đổi so với baseline (Delta)
    // Tay chạm/gần -> smooth giảm -> delta tăng
    float delta = baseline - smooth;
    
    // Map delta ra 0-100. Hệ số 3 là độ lợi (gain), có thể chỉnh tùy thực tế
    float value = delta * 3.0; 
    
    // Constrain giá trị trong khoảng 0-100
    if (value < 0) value = 0;
    if (value > 100) value = 100;
    
    // In ra Serial chuẩn định dạng cho Receiver
    Serial.printf("[🌿 TOUCH] %.2f\n", value);
    
    delay(20);
}
