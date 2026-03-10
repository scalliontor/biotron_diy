#define TOUCH_PIN 4
#define ALPHA 0.1

float smooth = 0;
float baseline = 0;

void setup() {
    Serial.begin(115200);

    touchSetCycles(0x800, 0x800);

    // Calib baseline - đừng chạm
    float sum = 0;
    for (int i = 0; i < 50; i++) {
        sum += touchRead(TOUCH_PIN);
        delay(10);
    }
    baseline = sum / 50.0;
    smooth = baseline;
    Serial.printf("Baseline: %.2f\n", baseline);
}

void loop() {
    int raw = touchRead(TOUCH_PIN);
    smooth = ALPHA * raw + (1.0 - ALPHA) * smooth;

    // Tay chạm → smooth giảm → delta dương
    float delta = baseline - smooth;
    float value = constrain((delta / baseline) * 100.0 * 2.0, 0.0, 100.0);

    // In ra Serial chỉ TÍNH RA SỐ VALUE (để TouchDesigner đọc dễ dàng qua Serial CHOP)
    Serial.println(value);

    delay(20);
}