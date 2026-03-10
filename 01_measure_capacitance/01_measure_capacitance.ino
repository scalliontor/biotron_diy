#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

#define TOUCH_PIN 4
#define ALPHA 0.1

// --- CẤU HÌNH WIFI ---
const char* ssid = "fustic.studio";
const char* password = "phuthinh";

// --- CẤU HÌNH OSC (TouchDesigner IP) ---
IPAddress outIp(192, 168, 0, 172); // <- Thây đổi thành IP máy tính chạy TouchDesigner
const unsigned int outPort = 8000; // <- Port cấu hình trong OSC In CHOP
WiFiUDP Udp;

float smooth = 0;
float baseline = 0;

void setup() {
    Serial.begin(115200);

    // Kết nối WiFi
    Serial.printf("\nConnecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.localIP());

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

    // Drifting baseline logic (Low-pass filter for environmental changes)
    if (smooth > baseline - 2.0) { 
        // Nếu ngườii đang không chạm (không có đột biến giảm), cho baseline từ từ bám theo smooth
        baseline = baseline * 0.999 + smooth * 0.001; 
    }

    // Tay chạm/gần → smooth giảm → delta dương
    float delta = baseline - smooth;
    
    // Map delta ra 0-100 với độ lợi vừa phải (không quá gắt như * 2.0 để lấy cả proximity)
    // Hệ số 0.5 - 1.0 giúp lấy được khoảng cách khi người đi tới gần cây thay vì max 100 liền
    float value = constrain((delta / baseline) * 100.0 * 0.8, 0.0, 100.0);

    Serial.printf("[🌿 TOUCH] %.2f\n", value);

    // Gửi OSC qua mạng
    OSCMessage msg("/biotron/touch");
    msg.add((float)value);
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();

    delay(20);
}