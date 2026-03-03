#define PIN_DRIVE 25
#define PIN_READ  32
#define ALPHA 0.05

float smooth = 0;

void setup() {
    pinMode(PIN_DRIVE, OUTPUT);
    digitalWrite(PIN_DRIVE, LOW);
    Serial.begin(115200);
}

void loop() {
    // Chỉ bật điện trong tích tắc để tránh hỏng cây
    digitalWrite(PIN_DRIVE, HIGH);
    delayMicroseconds(100);
    int raw = analogRead(PIN_READ);
    digitalWrite(PIN_DRIVE, LOW);

    // EMA filter
    smooth = ALPHA * raw + (1.0 - ALPHA) * smooth;

    // Map về 0-100
    // Dùng kiểu float để chia tỷ lệ mịn hơn thay vì map() mặc định
    float value = (smooth - 0.0) * (100.0 - 0.0) / (4095.0 - 0.0) + 0.0;
    
    // Constrain
    if (value < 0) value = 0;
    if (value > 100) value = 100;

    Serial.printf("[🌿 RESIST] %.2f\n", value);
    delay(20);
}
